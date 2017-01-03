/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RNS.cc,v 1.3 1998/02/28 13:10:27 nmcl Exp $
 */

#include <os/fcntl.h>
#include <os/ctype.h>
#include <os/stdlib.h>
#include <os/stdio.h>
#include <os/string.h>
#include <os/windows.h>
#include <os/winreg.h>

#ifndef GANDIVA_H_
#   include <Gandiva.h>
#endif

#ifndef COMMON_THREAD_THREAD_H_
#  include <Common/Thread/Thread.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef COMMON_UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef COMMON_UID_H_
#   include <Common/Uid.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#   include <Common/ObjectName.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

#ifndef INTERFACE_NAMESERVICE_H_
#   include <Interface/NameService.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_H_
#   include <Implementation/NameService.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_RNS_H_
#   include <Implementation/NameService/RNS.h>
#endif

#ifndef RNS_KEY
#define RNS_KEY HKEY_CURRENT_USER
#endif

#ifndef RNS_SUBKEY
#define RNS_SUBKEY "RNS_KEY"
#endif

#ifndef RNS_KEYCLASS
#define RNS_KEYCLASS "RNS_CLASS"
#endif

#ifndef RNS_MAXATTRIBUTE
#define RNS_MAXATTRIBUTE 1024
#endif

Boolean RNSNameServiceImple::attributeType(const char *objName,
										   const char *attrName,
										   ObjectName::AttributeType &attrType)
{
    Boolean res  = FALSE;
    PHKEY registry = NULL;

    res = openRegistryForReading(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
		char  *keyString = appendStrings(objName, "@", attrName);
		DWORD lptype = 0;
		BYTE data;
		DWORD datalen = 0;
		LONG result = RegQueryValueEx(*registry, keyString, NULL, &lptype, &data, &datalen);

		/*
		 * Since we only read the first character we're likely to get ERROR_MORE_DATA.
		 */
		 		
		if ((result == ERROR_MORE_DATA) || (result == ERROR_SUCCESS))
		{
			if (data == '#')
				attrType = ObjectName::SIGNED_NUMBER;
			else if (data == '~')
				attrType = ObjectName::UNSIGNED_NUMBER;
			else if (data == '$')
				attrType = ObjectName::STRING;
			else if (data == '%')
				attrType = ObjectName::OBJECTNAME;
			else if (data == '&')
				attrType = ObjectName::CLASSNAME;
			else if (data == '+')
				attrType = ObjectName::UID;
			else
				res = FALSE;
		}
		else
			res = FALSE;

        ::delete[] keyString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::firstAttributeName(const char *objName,
												char *&attrName)
{
    Boolean res  = FALSE;
    PHKEY registry = NULL;

    res = openRegistryForReading(registry);

    if (attrName != NULL)
    {
        ::delete [] attrName;
        attrName = NULL;
    }

    if (res && (registry != NULL) && (objName != NULL))
    {
        int objNameLen = ::strlen(objName);
		DWORD index = 0;
        Boolean found = FALSE;
		Boolean finished = FALSE;
		char key[RNS_MAXATTRIBUTE];
		DWORD keySize = 0;
		FILETIME accessTime;

		res = FALSE;

		if (RegEnumKeyEx(*registry, index, key, &keySize, NULL, NULL, NULL,
						 &accessTime) == ERROR_SUCCESS)
		{
	        while (!finished && !found)
			{
				char *separator = ::strchr(key, '@');

				if ((separator != NULL) && (separator[0] == '@'))
					found = (((separator - key) == objNameLen) &&
							 (::strncmp(key, objName, separator - key) == 0));

				if (!found)
				{
					index++;

					LONG result = RegEnumKeyEx(*registry, index, key, &keySize, NULL, NULL,
											   NULL, &accessTime);

					if ((result == ERROR_NO_MORE_ITEMS) || (result == ERROR_SUCCESS))
						finished = TRUE;
				}
				else
				{
					attrName = ::new char[::strlen(separator + 1) + 1];
					::strcpy(attrName, separator + 1);
					res = TRUE;
				}
			}
        }
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::nextAttributeName(const char *objName,
											   const char *attrName,
											   char *&nextAttrName)
{
    Boolean res  = FALSE;
	PHKEY registry = NULL;

    res = openRegistryForReading(registry);

    if (nextAttrName != NULL)
    {
        ::delete [] nextAttrName;
        nextAttrName = NULL;
    }

    if (res && (registry != NULL) && (objName != NULL))
    {
        Boolean found      = FALSE;
		char    *keyString = appendStrings(objName, "@", attrName);
		DWORD index = 0;
		Boolean finished = FALSE;
		char key[RNS_MAXATTRIBUTE];
		DWORD keySize = 0;
		FILETIME accessTime;

		res = (RegEnumKeyEx(*registry, index, key, &keySize, NULL, NULL, NULL,
							&accessTime) == ERROR_SUCCESS);

		while (res && !found)
		{
            found = (::strcmp(key, keyString) == 0);

			res = (RegEnumKeyEx(*registry, index, key, &keySize, NULL, NULL, NULL,
								&accessTime) == ERROR_SUCCESS);
        }

        ::delete [] keyString;

		if (res)
		{
			res = FALSE;
			found = FALSE;

			Boolean finished = FALSE;

			while (!finished && !found)
			{
				char *seperator = ::strchr(key, '@');

				if ((seperator != NULL) && (seperator[0] == '@'))
					found = ::strncmp(key, objName, seperator - key) == 0;

				if (!found)
				{
					index++;

					LONG result = RegEnumKeyEx(*registry, index, key, &keySize, NULL, NULL,
											   NULL, &accessTime);

					if ((result == ERROR_NO_MORE_ITEMS) || (result == ERROR_SUCCESS))
						finished = TRUE;
				}
				else
				{
					nextAttrName = ::new char[::strlen(seperator + 1) + 1];
					::strcpy(nextAttrName, seperator + 1);

					res = TRUE;
				}
			}
		}
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::getAttribute(const char *objName,
										  const char *attrName, long &value)
{
    Boolean res  = FALSE;
	PHKEY registry = NULL;

    res = openRegistryForReading(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
		char  *keyString = appendStrings(objName, "@", attrName);
		DWORD lptype = 0;
		DWORD datalen = 0;

		/*
		 * We have to call this function twice: the first time is simply to get the
		 * size of the data we are about to obtain.
		 */

		LONG result = RegQueryValueEx(*registry, keyString, NULL, &lptype, NULL, &datalen);

		if ((result == ERROR_SUCCESS) && (datalen > 0))
		{
			BYTE* data = ::new BYTE[datalen];  // includes null terminating character.

			if (RegQueryValueEx(*registry, keyString, NULL, &lptype, data, &datalen) == ERROR_SUCCESS)
			{
				if ((data != NULL) && (data[0] == '#') &&
					(isdigit(data[1]) || ((data[1] == '-') && isdigit(data[2]))))
					value = atol((const char*) data[1]);
				else
					res = FALSE;
			}
			else
				res = FALSE;

			if (data)
				::delete [] data;
		}
		else
			res = FALSE;

        ::delete[] keyString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::getAttribute(const char *objName,
										  const char *attrName,
										  unsigned long &value)
{
    Boolean res  = FALSE;
    PHKEY registry = NULL;

    res = openRegistryForReading(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
		char  *keyString = appendStrings(objName, "@", attrName);
		DWORD lptype = 0;
		DWORD datalen = 0;

		/*
		 * We have to call this function twice: the first time is simply to get the
		 * size of the data we are about to obtain.
		 */

		LONG result = RegQueryValueEx(*registry, keyString, NULL, &lptype, NULL, &datalen);

		if ((result == ERROR_SUCCESS) && (datalen > 0))
		{
			BYTE* data = ::new BYTE[datalen];  // includes null terminating character.

			if (RegQueryValueEx(*registry, keyString, NULL, &lptype, data, &datalen) == ERROR_SUCCESS)
			{
				if ((data != NULL) && (data[0] == '~') && isdigit(data[1]))
					value = atol((const char*) data[1]);
				else
					res = FALSE;
			}
			else
				res = FALSE;

			if (data)
				::delete [] data;
		}
		else
			res = FALSE;

        ::delete[] keyString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::getAttribute(const char *objName,
										  const char *attrName, char *&value)
{
    Boolean res  = FALSE;
    PHKEY registry = NULL;

    res = openRegistryForReading(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
		char  *keyString = appendStrings(objName, "@", attrName);
		DWORD lptype = 0;
		DWORD datalen = 0;

		/*
		 * We have to call this function twice: the first time is simply to get the
		 * size of the data we are about to obtain.
		 */

		LONG result = RegQueryValueEx(*registry, keyString, NULL, &lptype, NULL, &datalen);

		if ((result == ERROR_SUCCESS) && (datalen > 0))
		{
			BYTE* data = ::new BYTE[datalen];  // includes null terminating character.

			if (RegQueryValueEx(*registry, keyString, NULL, &lptype, data, &datalen) == ERROR_SUCCESS)
			{
				if ((data != NULL) && (data[0] == '$'))
				{
					if (value != NULL)
						::delete[] value;

					value = ::new char[::strlen((const char*) data[1])+1];
					::strcpy(value, (const char*) data[1]);
				}
				else
					res = FALSE;
			}
			else
				res = FALSE;

			if (data)
				::delete [] data;
		}
		else
			res = FALSE;

        ::delete[] keyString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::getAttribute(const char *objName,
										  const char *attrName,
										  ObjectName &value)
{
    Boolean res  = FALSE;
    PHKEY registry = NULL;

    res = openRegistryForReading(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
		char  *keyString = appendStrings(objName, "@", attrName);
		DWORD lptype = 0;
		DWORD datalen = 0;

		/*
		 * We have to call this function twice: the first time is simply to get the
		 * size of the data we are about to obtain.
		 */

		LONG result = RegQueryValueEx(*registry, keyString, NULL, &lptype, NULL, &datalen);

		if ((result == ERROR_SUCCESS) && (datalen > 0))
		{
			BYTE* data = ::new BYTE[datalen];  // includes null terminating character.

			if (RegQueryValueEx(*registry, keyString, NULL, &lptype, data, &datalen) == ERROR_SUCCESS)
			{
				if ((data != NULL) && (data[0] == '%'))
				{
					ObjectName objectName((const char*) data[1]);

					value = objectName;
				}
				else
					res = FALSE;
			}
			else
				res = FALSE;

			if (data)
				::delete [] data;
		}
		else
			res = FALSE;

        ::delete[] keyString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::getAttribute(const char *objName,
										  const char *attrName,
										  ClassName &value)
{
    Boolean res  = FALSE;
    PHKEY registry = NULL;

    res = openRegistryForReading(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
		char  *keyString = appendStrings(objName, "@", attrName);
		DWORD lptype = 0;
		DWORD datalen = 0;

		/*
		 * We have to call this function twice: the first time is simply to get the
		 * size of the data we are about to obtain.
		 */

		LONG result = RegQueryValueEx(*registry, keyString, NULL, &lptype, NULL, &datalen);

		if ((result == ERROR_SUCCESS) && (datalen > 0))
		{
			BYTE* data = ::new BYTE[datalen];  // includes null terminating character.

			if (RegQueryValueEx(*registry, keyString, NULL, &lptype, data, &datalen) == ERROR_SUCCESS)
			{
				if ((data != NULL) && (data[0] == '&'))
				{
					ClassName className((const char*) data[1]);

					value = className;
				}
				else
					res = FALSE;
			}
			else
				res = FALSE;

			if (data)
				::delete [] data;
		}
		else
			res = FALSE;

        ::delete[] keyString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::getAttribute(const char *objName,
										  const char *attrName, Uid &value)
{
    Boolean res  = FALSE;
    PHKEY registry = NULL;

    res = openRegistryForReading(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
		char  *keyString = appendStrings(objName, "@", attrName);
		DWORD lptype = 0;
		DWORD datalen = 0;

		/*
		 * We have to call this function twice: the first time is simply to get the
		 * size of the data we are about to obtain.
		 */

		LONG result = RegQueryValueEx(*registry, keyString, NULL, &lptype, NULL, &datalen);

		if ((result == ERROR_SUCCESS) && (datalen > 0))
		{
			BYTE* data = ::new BYTE[datalen];  // includes null terminating character.

			if (RegQueryValueEx(*registry, keyString, NULL, &lptype, data, &datalen) == ERROR_SUCCESS)
			{
				if ((data != NULL) && (data[0] == '+'))
				{
					Uid uid((const char*) data[1]);

					value = uid;
				}
				else
					res = FALSE;
			}
			else
				res = FALSE;

			if (data)
				::delete [] data;
		}
		else
			res = FALSE;

        ::delete[] keyString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::setAttribute(char *&objName,
										  const char *attrName, long value)
{
    Boolean res  = FALSE;
    PHKEY   registry = NULL;

    res = openRegistryForWriting(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
		char  *keyString   = appendStrings(objName, "@", attrName);
		char  dataString[32];

        sprintf(dataString, "#%ld", value);

		res = (RegSetValueEx(*registry, keyString, 0, REG_SZ,
							 (const BYTE*) dataString, ::strlen(dataString)) == ERROR_SUCCESS);

		::delete [] keyString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::setAttribute(char *&objName,
										  const char *attrName,
										  unsigned long value)
{
    Boolean res  = FALSE;
    PHKEY   registry = NULL;

    res = openRegistryForWriting(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
		char  *keyString   = appendStrings(objName, "@", attrName);
		char  dataString[32];

        sprintf(dataString, "~%lu", value);


		res = (RegSetValueEx(*registry, keyString, 0, REG_SZ,
							 (const BYTE*) dataString, ::strlen(dataString)) == ERROR_SUCCESS);

		::delete [] keyString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::setAttribute(char *&objName,
										  const char *attrName,
										  const char *value)
{
    Boolean res  = FALSE;
    PHKEY   registry = NULL;

    res = openRegistryForWriting(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
		char  *keyString   = appendStrings(objName, "@", attrName);

		res = (RegSetValueEx(*registry, keyString, 0, REG_SZ,
							 (const BYTE*) value, ::strlen(value)) == ERROR_SUCCESS);

		::delete [] keyString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::setAttribute(char *&objName,
										  const char *attrName,
										  const ObjectName &value)
{
    Boolean res  = FALSE;
    PHKEY registry = NULL;

    res = openRegistryForWriting(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
        char  *valueString = value.stringForm();
		char  *keyString   = appendStrings(objName, "@", attrName);
        char  *dataString  = ::new char[strlen(valueString) + 2];

        sprintf(dataString, "%%%s", valueString);

		res = (RegSetValueEx(*registry, keyString, 0, REG_SZ,
							 (const BYTE*) dataString, ::strlen(dataString)) == ERROR_SUCCESS);

        ::delete[] valueString;
        ::delete[] keyString;
        ::delete[] dataString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::setAttribute(char *&objName,
										  const char *attrName,
										  const ClassName &value)
{
    Boolean res  = FALSE;
	PHKEY registry = NULL;

    res = openRegistryForWriting(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
        char  *valueString = value.stringForm();
		char  *keyString   = appendStrings(objName, "@", attrName);
        char  *dataString  = ::new char[strlen(valueString) + 2];

        sprintf(dataString, "&%s", valueString);

		res = (RegSetValueEx(*registry, keyString, 0, REG_SZ,
							 (const BYTE*) dataString, ::strlen(dataString)) == ERROR_SUCCESS);

	    ::delete[] valueString;
        ::delete[] keyString;
        ::delete[] dataString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::setAttribute(char *&objName,
										  const char *attrName,
										  const Uid &value)
{
    Boolean res  = FALSE;
    PHKEY registry = NULL;

    res = openRegistryForWriting(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
        char  *valueString = value.stringForm();
		char  *keyString   = appendStrings(objName, "@", attrName);
        char  *dataString  = ::new char[strlen(valueString) + 2];

        sprintf(dataString, "+%s", valueString);

		res = (RegSetValueEx(*registry, keyString, 0, REG_SZ,
							 (const BYTE*) dataString, ::strlen(dataString)) == ERROR_SUCCESS);

        ::delete[] valueString;
        ::delete[] keyString;
        ::delete[] dataString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::removeAttribute(char *&objName,
											 const char *attrName)
{
    Boolean res  = FALSE;
    PHKEY registry = NULL;

    res = openRegistryForWriting(registry);

    if (res && (registry != NULL) && (objName != NULL) && (attrName != NULL))
    {
		char  *keyString = appendStrings(objName, "@", attrName);	

		res = (RegDeleteValue(*registry, keyString) == ERROR_SUCCESS);

        ::delete[] keyString;
    }

    closeRegistry(registry);

    return res;
}

Boolean RNSNameServiceImple::uniqueAttributeName(const char *, char *&attrName)
{
    Uid uid;

    attrName = uid.stringForm();

    return TRUE;
}

Boolean RNSNameServiceImple::uniqueObjectName(ObjectName &objectName)
{
    Uid uid;

    char *uniqueString           = uid.stringForm();
    char *uniqueObjectNameString = appendStrings("RNS:", uniqueString);

    ObjectName uniqueObjectName(uniqueObjectNameString);

    objectName = uniqueObjectName;

    if (uniqueString != NULL)
        ::delete[] uniqueString;

    if (uniqueObjectNameString != NULL)
        ::delete[] uniqueObjectNameString;

    return TRUE;
}

static const int maxOpenRetries  = 100;
static const int openRetryPeriod = 100; /* usec */

Boolean RNSNameServiceImple::openRegistryForReading (PHKEY& registry)
{
    int  openRetries = 0;
	LPDWORD lpdwDisposition = 0;
	long result = RegCreateKeyEx(RNS_KEY, RNS_SUBKEY, 0, RNS_KEYCLASS, REG_OPTION_NON_VOLATILE,
								 KEY_READ, NULL, registry, lpdwDisposition);

	while ((result != ERROR_SUCCESS) && (openRetries < maxOpenRetries))
	{
		Thread::sleep(openRetryPeriod);

		result = RegCreateKeyEx(RNS_KEY, RNS_SUBKEY, 0, RNS_KEYCLASS, REG_OPTION_NON_VOLATILE,
							    KEY_READ, NULL, registry, lpdwDisposition);

		openRetries++;
	}
	

	return (result == ERROR_SUCCESS);
}

Boolean RNSNameServiceImple::openRegistryForWriting (PHKEY& registry)
{
    int    openRetries = 0;
	LPDWORD lpdwDisposition = 0;
	long result = RegCreateKeyEx(RNS_KEY, RNS_SUBKEY, 0, RNS_KEYCLASS, REG_OPTION_NON_VOLATILE,
								 KEY_ALL_ACCESS, NULL, registry, lpdwDisposition);

	while ((result != ERROR_SUCCESS) && (openRetries < maxOpenRetries))
	{
		Thread::sleep(openRetryPeriod);

		result = RegCreateKeyEx(RNS_KEY, RNS_SUBKEY, 0, RNS_KEYCLASS, REG_OPTION_NON_VOLATILE,
							    KEY_ALL_ACCESS, NULL, registry, lpdwDisposition);

		openRetries++;
	}
	
	return (result == ERROR_SUCCESS);
}

Boolean RNSNameServiceImple::closeRegistry (PHKEY registry)
{
	if (registry != NULL)
	{
		/*
		 * Flush before closing to make sure any changes
		 * have been recorded.
		 */

		(void) RegFlushKey(registry);
		(void) RegCloseKey(registry);
	}

    return TRUE;
}

const ClassName &RNSNameServiceImple::name()
{
    return Gandiva::Implementation::NameService::RNS::name();
}

const ClassName &RNSNameServiceImple::className() const
{
    return Gandiva::Implementation::NameService::RNS::name();
}

Resource *RNSNameServiceImple::create()
{
    return new RNSNameServiceImple;
}

NameServiceImple *RNSNameServiceImple::clone()
{
    return NameServiceImple::castup(create());
}

Resource *RNSNameServiceImple::control(const ClassName &)
{
    return NULL;
}

RNSNameServiceImple::RNSNameServiceImple()
{
}

RNSNameServiceImple::~RNSNameServiceImple()
{
}

RNSNameServiceImple *RNSNameServiceImple::castup(Resource *resource)
{
    if (resource != NULL)
        return (RNSNameServiceImple *) resource->castup(RNSNameServiceImple::name());
    else
        return NULL;
}

void *RNSNameServiceImple::castup(const ClassName &className) const
{
    if (className == RNSNameServiceImple::name())
        return (void *) this;
    else
        return NameServiceImple::castup(className);
}
