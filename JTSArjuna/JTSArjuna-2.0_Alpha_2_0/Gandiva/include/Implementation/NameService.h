/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NameService.h,v 1.6 1996/01/02 13:12:54 nsmw Exp $
 */

#ifndef IMPLEMENTATION_NAMESERVICE_H_
#define IMPLEMENTATION_NAMESERVICE_H_

#ifndef OS_STDDEF_H_
#   include <os/stddef.h>
#endif

class NameServiceImple;

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

class Uid;

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

class Buffer;

#ifndef INTERFACE_NAMESERVICE_H_
#   include <Interface/NameService.h>
#endif

class NameServiceImple : virtual public Resource
{
public:
    virtual Boolean attributeType(const char *objName, const char *attrName, ObjectName::AttributeType &attrType) = 0;
    virtual Boolean firstAttributeName(const char *objName, char *&attrName) = 0;
    virtual Boolean nextAttributeName(const char *objName, const char *attrName, char *&nextAttrName) = 0;

    virtual Boolean getAttribute(const char *objName, const char *attrName, long &value) = 0;
    virtual Boolean getAttribute(const char *objName, const char *attrName, unsigned long &value) = 0;
    virtual Boolean getAttribute(const char *objName, const char *attrName, char *&value) = 0;
    virtual Boolean getAttribute(const char *objName, const char *attrName, ObjectName &value) = 0;
    virtual Boolean getAttribute(const char *objName, const char *attrName, ClassName &value) = 0;
    virtual Boolean getAttribute(const char *objName, const char *attrName, Uid &value) = 0;

    virtual Boolean setAttribute(char *&objName, const char *attrName, long value) = 0;
    virtual Boolean setAttribute(char *&objName, const char *attrName, unsigned long value) = 0;
    virtual Boolean setAttribute(char *&objName, const char *attrName, const char *value) = 0;
    virtual Boolean setAttribute(char *&objName, const char *attrName, const ObjectName &value) = 0;
    virtual Boolean setAttribute(char *&objName, const char *attrName, const ClassName &value) = 0;
    virtual Boolean setAttribute(char *&objName, const char *attrName, const Uid &value) = 0;

    virtual Boolean removeAttribute(char *&objName, const char *attrName) = 0;
    virtual Boolean uniqueAttributeName(const char *objName, char *&attrName) = 0;

    virtual Boolean uniqueObjectName(ObjectName &objectName) = 0;

    virtual NameServiceImple* clone () = 0;
    
    static  const ClassName &name();
    virtual const ClassName &className() const;

    virtual Resource *control(const ClassName &ctrlInterName) = 0;

    static  NameServiceImple *castup(Resource *resource);
    virtual void             *castup(const ClassName &className) const;

protected:
    NameServiceImple();
    virtual ~NameServiceImple();
};

#endif
