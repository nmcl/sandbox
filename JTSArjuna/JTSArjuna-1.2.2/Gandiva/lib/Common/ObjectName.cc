/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectName.cc,v 1.10 1997/06/02 10:27:23 nmcl Exp $
 */

#include <os/stddef.h>
#include <os/string.h>
#include <os/iostream.h>

#ifndef COMMON_UID_H_
#   include <Common/Uid.h>
#endif

#ifndef COMMON_UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#   include <Common/ObjectName.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

#ifndef INTERFACE_BUFFER_H_
#   include <Interface/Buffer.h>
#endif

#ifndef INTERFACE_NAMESERVICE_H_
#   include <Interface/NameService.h>
#endif

ObjectName* ObjectName::_invalid = (ObjectName*) 0;

static const char* _invalidString = "invalid";
static const char* nameServiceString = "NameServiceImple";

const char* nameDelim = ":";
const char* nullString = "";


Boolean ObjectName::attributeType(const char *attrName,
				  AttributeType &attrType)
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->attributeType(_name, attrName, attrType);

    return res;
}

Boolean ObjectName::firstAttributeName(char *&attrName)
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->firstAttributeName(_name, attrName);

    return res;
}

Boolean ObjectName::nextAttributeName(const char *attrName,
				      char *&nextAttrName)
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->nextAttributeName(_name, attrName, nextAttrName);

    return res;
}

Boolean ObjectName::getAttribute(const char *attrName, long &value) const
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->getAttribute(_name, attrName, value);

    if (! res)
        cerr << "WARNING: ObjectName::getAttribute(\"" << attrName
	     << "\", long&) on " <<  *this << " failed" << endl;

    return res;
}

Boolean ObjectName::getAttribute(const char *attrName,
				 unsigned long &value) const
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->getAttribute(_name, attrName, value);

    if (! res)
        cerr << "WARNING: ObjectName::getAttribute(\"" << attrName
	     << "\", unsigned long&) on " <<  *this << " failed" << endl;

    return res;
}

Boolean ObjectName::getAttribute(const char *attrName, char *&value) const
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->getAttribute(_name, attrName, value);

    if (! res)
        cerr << "WARNING: ObjectName::getAttribute(\"" << attrName
	     << "\", char*&) on " <<  *this << " failed" << endl;

    return res;
}

Boolean ObjectName::getAttribute(const char *attrName, ObjectName &value) const
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->getAttribute(_name, attrName, value);

    if (! res)
        cerr << "WARNING: ObjectName::getAttribute(\"" << attrName
	     << "\", ObjectName&) on " <<  *this << " failed" << endl;

    return res;
}

Boolean ObjectName::getAttribute(const char *attrName, ClassName &value) const
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->getAttribute(_name, attrName, value);

    if (! res)
        cerr << "WARNING: ObjectName::getAttribute(\"" << attrName
	     << "\", ClassName&) on " <<  *this << " failed" << endl;

    return res;
}

Boolean ObjectName::getAttribute(const char *attrName, Uid &value) const
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->getAttribute(_name, attrName, value);

    if (! res)
        cerr << "WARNING: ObjectName::getAttribute(\"" << attrName
	     << "\", Uid&) on " <<  *this << " failed" << endl;

    return res;
}

ObjectName ObjectName::getObjectNameAttribute(const char *attrName) const
{
    Boolean res = FALSE;
    ObjectName value(ObjectName::invalid());

    if (_nameService != NULL)
        res = _nameService->getAttribute(_name, attrName, value);

    if (! res)
    {
        cerr << "WARNING: ObjectName::getObjectNameAttribute(\""
	     << attrName << "\") on " <<  *this << " failed" << endl;

        value = ObjectName::invalid();
    }

    return value;
}

ClassName ObjectName::getClassNameAttribute(const char *attrName) const
{
    Boolean res = FALSE;
    ClassName value(ClassName::invalid());

    if (_nameService != NULL)
        res = _nameService->getAttribute(_name, attrName, value);

    if (! res)
    {
        cerr << "WARNING: ObjectName::getClassNameAttribute(\""
	     << attrName << "\") on " <<  *this << " failed" << endl;

        value = ClassName::invalid();
    }

    return value;
}

Uid ObjectName::getUidAttribute(const char *attrName) const
{
    Boolean res = FALSE;
    Uid value(Uid::invalid());

    if (_nameService != NULL)
        res = _nameService->getAttribute(_name, attrName, value);

    if (! res)
    {
        cerr << "WARNING: ObjectName::getUidAttribute(\"" << attrName
	     << "\") on " <<  *this << " failed" << endl;

        value = Uid::invalid();
    }

    return value;
}

Boolean ObjectName::setAttribute(const char *attrName, long value)
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->setAttribute(_name, attrName, value);

    return res;
}

Boolean ObjectName::setAttribute(const char *attrName, unsigned long value)
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->setAttribute(_name, attrName, value);

    return res;
}

Boolean ObjectName::setAttribute(const char *attrName, const char *value)
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->setAttribute(_name, attrName, value);

    return res;
}

Boolean ObjectName::setAttribute(const char *attrName, const ObjectName &value)
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->setAttribute(_name, attrName, value);

    return res;
}

Boolean ObjectName::setAttribute(const char *attrName, const ClassName &value)
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->setAttribute(_name, attrName, value);

    return res;
}

Boolean ObjectName::setAttribute(const char *attrName, const Uid &value)
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->setAttribute(_name, attrName, value);

    return res;
}

Boolean ObjectName::removeAttribute(const char *attrName)
{
    Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->removeAttribute(_name, attrName);

    return res;
}

Boolean ObjectName::uniqueAttributeName(char *&attrName)
{
   Boolean res = FALSE;

    if (_nameService != NULL)
        res = _nameService->uniqueAttributeName(_name, attrName);

    return res;
}

Boolean ObjectName::uniqueObjectName(const char *mech, ObjectName &objectName)
{
    Boolean res = FALSE;

    NameService *nameService = NULL;

    objectName = ObjectName::invalid();

    if ((mech != NULL) && (strcmp(mech, _invalidString) != 0))
    {
        char *nameServiceMech = new char[strlen(mech) + strlen(nameServiceString) + 1];

        strcpy(nameServiceMech, mech);
        strcat(nameServiceMech, nameServiceString);

        ClassName nameServiceMechClassName(nameServiceMech);
        nameService = NameService::create(nameServiceMechClassName);

        Resource::ref(nameService);

        if (nameService != NULL)
            res = nameService->uniqueObjectName(objectName);

        if (nameServiceMech != NULL)
            ::delete [] nameServiceMech;

        Resource::unref(nameService);
    }

    return res;
}

ObjectName &ObjectName::operator= (const ObjectName &objectName)
{
    if (this == &objectName)
	return *this;
    
    Resource::unref(_nameService);
    _nameService = NULL;

    if (_mech != NULL)
    {
        ::delete [] _mech;
        _mech = NULL;
    }

    if (_name != NULL)
    {
        ::delete [] _name;
        _name = NULL;
    }

    if (objectName._mech != NULL)
    {
        _mech = new char[strlen(objectName._mech) + 1];
        strcpy(_mech, objectName._mech);
	if (objectName._nameService)
	{
	    _nameService = new NameService(*objectName._nameService);
	    Resource::ref(_nameService);
	}
    }

    if (objectName._name != NULL)
    {
        _name = new char[strlen(objectName._name) + 1];
        strcpy(_name, objectName._name);
    }

    initNameService();

    return *this;
}

Boolean ObjectName::operator== (const ObjectName &objectName) const
{
    return ((_name == objectName._name) || 
            ((_name != NULL) && (objectName._name != NULL) && (strcmp(_name, objectName._name) == 0))) &&
           ((_mech == objectName._mech) ||
            ((_mech != NULL) && (objectName._mech != NULL) && (strcmp(_mech, objectName._mech) == 0)));
}

Boolean ObjectName::operator!= (const ObjectName &objectName) const
{
    return ((_name != objectName._name) && 
            ((_name == NULL) || (objectName._name == NULL) || (strcmp(_name, objectName._name) != 0))) ||
           ((_mech != objectName._mech) &&
            ((_mech == NULL) || (objectName._mech == NULL) || (strcmp(_mech, objectName._mech) != 0)));
}

ObjectName::ObjectName(const char *objectName)
		      : _mech(0),
			_name(0),
			_nameService(0)
{
    if (objectName == NULL)
    {
       _mech = ::new char [::strlen(_invalidString) + 1];
       ::strcpy(_mech, _invalidString);
       _name = ::new char [::strlen(_invalidString) + 1];
       ::strcpy(_name, _invalidString);
    }
    else
    {
        char *colon = ::strstr(objectName, nameDelim);
    
        if (colon != NULL)
        {
            int mechLength = colon - objectName;
            int nameLength = ::strlen(objectName) - mechLength - ::strlen(nameDelim);

	    _mech = ::new char[mechLength + 1];

	    ::strncpy(_mech, objectName, mechLength);
	    _mech[mechLength] = '\0';
	    
	    if (nameLength > 0)
	    {
		_name = ::new char[nameLength + 1];

		::strncpy(_name, colon + ::strlen(nameDelim), nameLength);
		_name[nameLength] = '\0';
	    }
	    else
	    {
		_name = ::new char[::strlen(nullString)+1];
		::strcpy(_name, nullString);
	    }
	}
    }

    initNameService();
}

ObjectName::ObjectName(const ObjectName &objectName)
		      : _mech(0),
			_name(0),
			_nameService(0)
{
    if (objectName._mech != NULL)
    {
        _mech = ::new char[strlen(objectName._mech) + 1];
        ::strcpy(_mech, objectName._mech);
	if (objectName._nameService)
	{
	    _nameService = new NameService(*objectName._nameService);
	    Resource::ref(_nameService);
	}
    }
    else
    {
       _mech = ::new char [::strlen(_invalidString) + 1];
       ::strcpy(_mech, _invalidString);
    }

    if (objectName._name != NULL)
    {
        _name = ::new char[::strlen(objectName._name) + 1];
        ::strcpy(_name, objectName._name);
    }
    else
    {
       _name = ::new char [::strlen(_invalidString) + 1];
       ::strcpy(_name, _invalidString);
    }

    initNameService();
}

ObjectName::ObjectName (NameService *nameService, const char *objectName)
		       : _mech(0),
			 _name(0),
			 _nameService(0)
{
    if ((nameService) && (objectName))
    {
	_nameService = nameService;
	Resource::ref(_nameService);

	char* fullMechName = _nameService->impleClassName().stringForm();
	char* appendedString = ::strstr(fullMechName, nameServiceString);
	
	_mech = ::new char[appendedString - fullMechName +1];
	::strncpy(_mech, fullMechName, appendedString-fullMechName);
	_mech[appendedString-fullMechName] = '\0';
	::delete [] fullMechName;

	_name = ::new char[::strlen(objectName)+1];
	::strcpy(_name, objectName);
    }
    else
    {
	if (!nameService)
	    cerr << "ERROR: ObjectName::ObjectName - no name service given."
		 << endl;
	
	if (!objectName)
	    cerr << "ERROR: ObjectName::ObjectName - no object name give."
		 << endl;
	
       _mech = ::new char [::strlen(_invalidString) + 1];
       ::strcpy(_mech, _invalidString);
       _name = ::new char [::strlen(_invalidString) + 1];
	::strcpy(_name, _invalidString);
    }
}

ObjectName::~ObjectName()
{
    if (_mech != NULL)
        ::delete [] _mech;

    if (_name != NULL)
        ::delete [] _name;

    Resource::unref(_nameService);
}

char* ObjectName::getMechanismName () const
{
    if (!_mech)
        return (char*) 0;
    
    char* ptr = ::new char[::strlen(_mech)+1];
    ::strcpy(ptr, _mech);

    return ptr;
}

char *ObjectName::stringForm () const
{
    char* ptr1 = ((_mech) ? ::appendStrings(_mech, nameDelim) : 0);
    char* ptr2 = ::appendStrings(ptr1, _name);
    
    if (ptr1)
	::delete [] ptr1;

    return ptr2;
}

const ObjectName &ObjectName::invalid ()
{
    if (!_invalid)
	_invalid = new ObjectName("invalid:invalid");
    
    return *_invalid;
}

Boolean ObjectName::pack(Buffer &buff) const
{
    return buff.pack(_mech) && buff.pack(_name);
}

Boolean ObjectName::unpack(Buffer &buff)
{
    Boolean res = TRUE;

    Resource::unref(_nameService);
    _nameService = NULL;

    res = buff.unpack(_mech) && buff.unpack(_name);

    if (res)
        initNameService();

    return res;
}

void ObjectName::print(ostream &ostr) const
{
    ostr << "<ObjectName:" << _mech << nameDelim << _name << '>';
}

void ObjectName::initNameService()
{
    if (_nameService)
	return;

    if ((_mech != NULL) && (::strcmp(_mech, _invalidString) != 0))
    {
	char* nameServiceMech = ::appendStrings(_mech, nameServiceString);

        ClassName nameServiceMechClassName(nameServiceMech);

        _nameService = NameService::create(nameServiceMechClassName);

        Resource::ref(_nameService);

        if (nameServiceMech != NULL)
            ::delete [] nameServiceMech;
    }
    else
        _nameService = NULL;
}

ostream &operator<< (ostream &ostr, const ObjectName &objectName)
{
    objectName.print(ostr);

    return ostr;
}

ostream& operator<< (ostream& strm, const ObjectName::AttributeType t)
{
    switch (t)
    {
    case ObjectName::SIGNED_NUMBER:
	strm << "SIGNED_NUMBER";
	break;
    case ObjectName::UNSIGNED_NUMBER:
	strm << "UNSIGNED_NUMBER";
	break;
    case ObjectName::STRING:
	strm << "STRING";
	break;
    case ObjectName::OBJECTNAME:
	strm << "OBJECTNAME";
	break;
    case ObjectName::CLASSNAME:
	strm << "CLASSNAME";
	break;
    case ObjectName::UID:
	strm << "UID";
	break;
    }

    return strm;
}
