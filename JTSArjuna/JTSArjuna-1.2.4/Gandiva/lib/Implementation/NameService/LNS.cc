/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LNS.cc,v 1.15 1998/01/12 13:10:11 nmcl Exp $
 */

#include <os/sys/stat.h>
#include <os/fcntl.h>
#include <os/ctype.h>
#include <os/stdlib.h>
#include <os/stdio.h>
#include <os/string.h>
#include <os/ndbm.h>

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

#ifndef IMPLEMENTATION_NAMESERVICE_LNS_H_
#   include <Implementation/NameService/LNS.h>
#endif

#ifndef LNS_ROOT
#define LNS_ROOT "/usr/tmp/LNS_root"
#endif

#ifndef LNS_GET_FLAGS
#define LNS_GET_FLAGS (O_RDONLY | O_CREAT)
#endif

#ifndef LNS_SET_FLAGS
#define LNS_SET_FLAGS (O_RDWR | O_CREAT)
#endif

#ifndef LNS_PERM
#define LNS_PERM (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)
#endif

Boolean LNSNameServiceImple::attributeType(const char *objName,
					   const char *attrName,
					   ObjectName::AttributeType &attrType)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForReading(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
	char  *keyString = appendStrings(objName, "@", attrName);
        datum data;
        datum key;

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        data = dbm_fetch(dbm, key);

        if (data.dptr != NULL)
        {
            if (data.dptr[0] == '#')
                attrType = ObjectName::SIGNED_NUMBER;
            else if (data.dptr[0] == '~')
                attrType = ObjectName::UNSIGNED_NUMBER;
            else if (data.dptr[0] == '$')
                attrType = ObjectName::STRING;
            else if (data.dptr[0] == '%')
                attrType = ObjectName::OBJECTNAME;
            else if (data.dptr[0] == '&')
                attrType = ObjectName::CLASSNAME;
            else if (data.dptr[0] == '+')
                attrType = ObjectName::UID;
            else
                res = FALSE;
        }
        else
            res = FALSE;

        delete[] keyString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::firstAttributeName(const char *objName,
						char *&attrName)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForReading(dbm);

    if (attrName != NULL)
    {
        delete attrName;
        attrName = NULL;
    }

    if (res && (dbm != NULL) && (objName != NULL))
    {
        datum key;

        int objNameLen = strlen(objName);

        Boolean found = FALSE;

	res = FALSE;
        key = dbm_firstkey(dbm);
        while ((key.dptr != NULL) && (! found))
	{
            char *seperator = strchr(key.dptr, '@');

            if ((seperator != NULL) && (seperator[0] == '@'))
                found = (((seperator - key.dptr) == objNameLen) &&
                         (strncmp(key.dptr, objName, seperator - key.dptr) == 0));

            if (! found)
                key = dbm_nextkey(dbm);
            else
	    {
                attrName = new char[strlen(seperator + 1) + 1];
                strcpy(attrName, seperator + 1);
		res = TRUE;
	    }
        }
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::nextAttributeName(const char *objName,
					       const char *attrName,
					       char *&nextAttrName)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForReading(dbm);

    if (nextAttrName != NULL)
    {
        delete nextAttrName;
        nextAttrName = NULL;
    }

    if (res && (dbm != NULL) && (objName != NULL))
    {
        datum key;

        Boolean found      = FALSE;
	char    *keyString = appendStrings(objName, "@", attrName);

        key = dbm_firstkey(dbm);
        while ((key.dptr != NULL) && (! found))
	{
            found = strcmp(key.dptr, keyString) == 0;

            key = dbm_nextkey(dbm);
        }

        delete[] keyString;

	res = FALSE;
        found = FALSE;
        while ((key.dptr != NULL) && (! found))
	{
            char *seperator = strchr(key.dptr, '@');

            if ((seperator != NULL) && (seperator[0] == '@'))
                found = strncmp(key.dptr, objName, seperator - key.dptr) == 0;

            if (! found)
                key = dbm_nextkey(dbm);
            else
	    {
                nextAttrName = new char[strlen(seperator + 1) + 1];
                strcpy(nextAttrName, seperator + 1);

		res = TRUE;
	    }
        }
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::getAttribute(const char *objName,
					  const char *attrName, long &value)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForReading(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
	char  *keyString = appendStrings(objName, "@", attrName);
        datum data;
        datum key;

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        data = dbm_fetch(dbm, key);

        if ((data.dptr != NULL) && (data.dptr[0] == '#') &&
            (isdigit(data.dptr[1]) || ((data.dptr[1] == '-') && isdigit(data.dptr[2]))))
            value = atol(&(data.dptr[1]));
	else
	    res = FALSE;

        delete[] keyString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::getAttribute(const char *objName,
					  const char *attrName,
					  unsigned long &value)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForReading(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
	char  *keyString = appendStrings(objName, "@", attrName);
        datum data;
        datum key;

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        data = dbm_fetch(dbm, key);

        if ((data.dptr != NULL) && (data.dptr[0] == '~') && isdigit(data.dptr[1]))
            value = atol(&(data.dptr[1]));
	else
	    res = FALSE;

        delete[] keyString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::getAttribute(const char *objName,
					  const char *attrName, char *&value)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForReading(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
	char  *keyString = appendStrings(objName, "@", attrName);
        datum data;
        datum key;

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        data = dbm_fetch(dbm, key);

        if ((data.dptr != NULL) && (data.dptr[0] == '$'))
        {
            if (value != NULL)
                delete[] value;

            value = new char[data.dsize];
	    strcpy(value, &(data.dptr[1]));
	}
	else
	    res = FALSE;

        delete[] keyString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::getAttribute(const char *objName,
					  const char *attrName,
					  ObjectName &value)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForReading(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
	char  *keyString = appendStrings(objName, "@", attrName);
        datum data;
        datum key;

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        data = dbm_fetch(dbm, key);

        if ((data.dptr != NULL) && (data.dptr[0] == '%'))
        {
            ObjectName objectName(&(data.dptr[1]));

            value = objectName;
        }
	else
	    res = FALSE;

        delete[] keyString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::getAttribute(const char *objName,
					  const char *attrName,
					  ClassName &value)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForReading(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
	char  *keyString = appendStrings(objName, "@", attrName);
        datum data;
        datum key;

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        data = dbm_fetch(dbm, key);

        if ((data.dptr != NULL) && (data.dptr[0] == '&'))
        {
            ClassName className(&(data.dptr[1]));

            value = className;
        }
	else
	    res = FALSE;

        delete[] keyString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::getAttribute(const char *objName,
					  const char *attrName, Uid &value)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForReading(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
	char  *keyString = appendStrings(objName, "@", attrName);
        datum data;
        datum key;

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        data = dbm_fetch(dbm, key);

        if ((data.dptr != NULL) && (data.dptr[0] == '+'))
        {
            Uid uid(&(data.dptr[1]));

            value = uid;
        }
	else
	    res = FALSE;

        delete[] keyString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::setAttribute(char *&objName,
					  const char *attrName, long value)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForWriting(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
	char  *keyString = appendStrings(objName, "@", attrName);
        char  dataString[32];
        datum key;
        datum data;

        sprintf(dataString, "#%ld", value);

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        data.dptr  = dataString;
        data.dsize = strlen(dataString) + 1;
	
        res = (dbm_store(dbm, key, data, DBM_REPLACE) == 0);
	
        delete[] keyString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::setAttribute(char *&objName,
					  const char *attrName,
					  unsigned long value)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForWriting(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
	char  *keyString = appendStrings(objName, "@", attrName);
        char  dataString[32];
        datum key;
        datum data;

        sprintf(dataString, "~%lu", value);

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        data.dptr  = dataString;
        data.dsize = strlen(dataString) + 1;
	
        res = (dbm_store(dbm, key, data, DBM_REPLACE) == 0);

        delete[] keyString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::setAttribute(char *&objName,
					  const char *attrName,
					  const char *value)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForWriting(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
	char  *keyString  = appendStrings(objName, "@", attrName);
        char  *dataString = new char[strlen(value) + 2];
        datum key;
        datum data;

        sprintf(dataString, "$%s", value);

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        data.dptr  = dataString;
        data.dsize = strlen(dataString) + 1;

        res = (dbm_store(dbm, key, data, DBM_REPLACE) == 0);

        delete[] keyString;
        delete[] dataString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::setAttribute(char *&objName,
					  const char *attrName,
					  const ObjectName &value)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForWriting(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
        char  *valueString = value.stringForm();
	char  *keyString   = appendStrings(objName, "@", attrName);
        char  *dataString  = new char[strlen(valueString) + 2];
        datum key;
        datum data;

        sprintf(dataString, "%%%s", valueString);

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        data.dptr  = dataString;
        data.dsize = strlen(dataString) + 1;

        res = (dbm_store(dbm, key, data, DBM_REPLACE) == 0);

        delete[] valueString;
        delete[] keyString;
        delete[] dataString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::setAttribute(char *&objName,
					  const char *attrName,
					  const ClassName &value)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForWriting(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
        char  *valueString = value.stringForm();
	char  *keyString   = appendStrings(objName, "@", attrName);
        char  *dataString  = new char[strlen(valueString) + 2];
        datum key;
        datum data;

        sprintf(dataString, "&%s", valueString);

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        data.dptr  = dataString;
        data.dsize = strlen(dataString) + 1;

        res = (dbm_store(dbm, key, data, DBM_REPLACE) == 0);

        delete[] valueString;
        delete[] keyString;
        delete[] dataString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::setAttribute(char *&objName,
					  const char *attrName,
					  const Uid &value)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForWriting(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
        char  *valueString = value.stringForm();
	char  *keyString   = appendStrings(objName, "@", attrName);
        char  *dataString  = new char[strlen(valueString) + 2];
        datum key;
        datum data;

        sprintf(dataString, "+%s", valueString);

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        data.dptr  = dataString;
        data.dsize = strlen(dataString) + 1;

        res = (dbm_store(dbm, key, data, DBM_REPLACE) == 0);

        delete[] valueString;
        delete[] keyString;
        delete[] dataString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::removeAttribute(char *&objName,
					     const char *attrName)
{
    Boolean res  = FALSE;
    DBM     *dbm = NULL;

    res = openDBForWriting(dbm);

    if (res && (dbm != NULL) && (objName != NULL) && (attrName != NULL))
    {
	char  *keyString = appendStrings(objName, "@", attrName);	
        datum key;

        key.dptr  = keyString;
        key.dsize = strlen(keyString) + 1;

        res = (dbm_delete(dbm, key) == 0);

        delete[] keyString;
    }

    closeDB(dbm);

    return res;
}

Boolean LNSNameServiceImple::uniqueAttributeName(const char *, char *&attrName)
{
    Uid uid;

    attrName = uid.stringForm();

    return TRUE;
}

Boolean LNSNameServiceImple::uniqueObjectName(ObjectName &objectName)
{
    Uid uid;

    char *uniqueString           = uid.stringForm();
    char *uniqueObjectNameString = appendStrings("LNS:", uniqueString);

    ObjectName uniqueObjectName(uniqueObjectNameString);

    objectName = uniqueObjectName;

    if (uniqueString != NULL)
        delete[] uniqueString;

    if (uniqueObjectNameString != NULL)
        delete[] uniqueObjectNameString;

    return TRUE;
}

static const int maxOpenRetries  = 100;
static const int openRetryPeriod = 100; /* usec */

Boolean LNSNameServiceImple::openDBForReading(DBM *&dbm)
{
    int    openRetries = 0;
    mode_t oldMode     = umask(! (S_IRWXU | S_IRWXG | S_IRWXO));

    dbm = NULL;

    dbm = dbm_open(LNS_ROOT, LNS_GET_FLAGS, LNS_PERM);

    while ((dbm == NULL) && (openRetries < maxOpenRetries))
    {
        if ((openRetries % 2) == 0)
            dbm = dbm_open(LNS_ROOT, LNS_SET_FLAGS, LNS_PERM);
        else
            dbm = dbm_open(LNS_ROOT, LNS_GET_FLAGS, LNS_PERM);
        
        if (dbm == NULL)
            Thread::sleep(openRetryPeriod);

        openRetries++;
    }

    umask(oldMode);

    return (dbm != NULL);
}

Boolean LNSNameServiceImple::openDBForWriting(DBM *&dbm)
{
    int    openRetries = 0;
    mode_t oldMode     = umask(! (S_IRWXU | S_IRWXG | S_IRWXO));

    dbm = NULL;

    while ((dbm == NULL) && (openRetries < maxOpenRetries))
    {
        dbm = dbm_open(LNS_ROOT, LNS_SET_FLAGS, LNS_PERM);

        if (dbm == NULL)
            Thread::sleep(openRetryPeriod);

        openRetries++;
    }

    umask(oldMode);

    return (dbm != NULL);
}

Boolean LNSNameServiceImple::closeDB(DBM *dbm)
{
    if (dbm != NULL)
        dbm_close(dbm);

    return TRUE;
}

const ClassName &LNSNameServiceImple::name()
{
    return Gandiva::Implementation::NameService::LNS::name();
}

const ClassName &LNSNameServiceImple::className() const
{
    return Gandiva::Implementation::NameService::LNS::name();
}

Resource *LNSNameServiceImple::create()
{
    return new LNSNameServiceImple;
}

NameServiceImple *LNSNameServiceImple::clone()
{
    return NameServiceImple::castup(create());
}

Resource *LNSNameServiceImple::control(const ClassName &)
{
    return NULL;
}

LNSNameServiceImple::LNSNameServiceImple()
{
}

LNSNameServiceImple::~LNSNameServiceImple()
{
}

LNSNameServiceImple *LNSNameServiceImple::castup(Resource *resource)
{
    if (resource != NULL)
        return (LNSNameServiceImple *) resource->castup(LNSNameServiceImple::name());
    else
        return NULL;
}

void *LNSNameServiceImple::castup(const ClassName &className) const
{
    if (className == LNSNameServiceImple::name())
        return (void *) this;
    else
        return NameServiceImple::castup(className);
}
