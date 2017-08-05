/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectName.h,v 1.6 1996/01/02 13:12:37 nsmw Exp $
 */

#ifndef COMMON_OBJECTNAME_H_
#define COMMON_OBJECTNAME_H_

#ifndef OS_IOSTREAM_H_
#   include <os/iostream.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

#ifndef COMMON_UID_H_
#   include <Common/Uid.h>
#endif

class Buffer;

class NameService;

class ObjectName
{
public:
    enum AttributeType { SIGNED_NUMBER, UNSIGNED_NUMBER, STRING, OBJECTNAME, CLASSNAME, UID };

    ObjectName(const char *objectNameString);
    ObjectName(const ObjectName &objectName);
    ObjectName(NameService *nameService, const char *name);
    virtual ~ObjectName();

    Boolean attributeType(const char *attributeName,
			  AttributeType &attributeType);
    Boolean firstAttributeName(char *&attributeName);
    Boolean nextAttributeName(const char *attributeName,
			      char *&nextAttributeName);

    Boolean getAttribute(const char *attributeName, long &value) const;
    Boolean getAttribute(const char *attributeName, unsigned long &value) const;
    Boolean getAttribute(const char *attributeName, char *&value) const;
    Boolean getAttribute(const char *attributeName, ObjectName &value) const;
    Boolean getAttribute(const char *attributeName, ClassName &value) const;
    Boolean getAttribute(const char *attributeName, Uid &value) const;

    ObjectName getObjectNameAttribute(const char *attributeName) const;
    ClassName  getClassNameAttribute(const char *attributeName) const;
    Uid        getUidAttribute(const char *attributeName) const;

    Boolean setAttribute(const char *attributeName, long value);
    Boolean setAttribute(const char *attributeName, unsigned long value);
    Boolean setAttribute(const char *attributeName, const char *value);
    Boolean setAttribute(const char *attributeName, const ObjectName &value);
    Boolean setAttribute(const char *attributeName, const ClassName &value);
    Boolean setAttribute(const char *attributeName, const Uid &value);

    Boolean removeAttribute(const char *attributeName);

    Boolean uniqueAttributeName(char *&attributeName);

    static Boolean uniqueObjectName(const char *mech, ObjectName &objectName);

    ObjectName &operator= (const ObjectName &objectName);

    Boolean operator== (const ObjectName &objectName) const;
    Boolean operator!= (const ObjectName &objectName) const;

    char *stringForm() const;

    char *getMechanismName() const;

    static const ObjectName &invalid();

    Boolean pack(Buffer &buff) const;
    Boolean unpack(Buffer &buff);

    void print(ostream &ostr) const;

private:
    static ObjectName* _invalid;

    void initNameService();

    char        *_mech;
    char        *_name;

    NameService *_nameService;
};

extern ostream &operator<< (ostream &ostr, const ObjectName &objectName);

extern ostream &operator<< (ostream &ostr, const ObjectName::AttributeType AttributeType);

#endif
