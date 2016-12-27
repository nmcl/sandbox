/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NameService.h,v 1.7 1996/01/02 13:12:59 nsmw Exp $
 */

#ifndef INTERFACE_NAMESERVICE_H_
#define INTERFACE_NAMESERVICE_H_

class Uid;

class ClassName;

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#   include <Common/ObjectName.h>
#endif

class NameServiceImple;

class NameService : virtual public Resource
{
public:
    Boolean attributeType(const char *objName, const char *attrName,
			  ObjectName::AttributeType &attrType);
    Boolean firstAttributeName(const char *objName, char *&attrName);
    Boolean nextAttributeName(const char *objName, const char *attrName,
			      char *&nextAttrName);

    Boolean getAttribute(const char *objName, const char *attrName, long &value);
    Boolean getAttribute(const char *objName, const char *attrName, unsigned long &value);
    Boolean getAttribute(const char *objName, const char *attrName, char *&value);
    Boolean getAttribute(const char *objName, const char *attrName, ObjectName &value);
    Boolean getAttribute(const char *objName, const char *attrName, ClassName &value);
    Boolean getAttribute(const char *objName, const char *attrName, Uid &value);

    Boolean setAttribute(char *&objName, const char *attrName, long value);
    Boolean setAttribute(char *&objName, const char *attrName, unsigned long value);
    Boolean setAttribute(char *&objName, const char *attrName, const char *value);
    Boolean setAttribute(char *&objName, const char *attrName, const ObjectName &value);
    Boolean setAttribute(char *&objName, const char *attrName, const ClassName &value);
    Boolean setAttribute(char *&objName, const char *attrName, const Uid &value);

    Boolean removeAttribute(char *&objName, const char *attrName);
    Boolean uniqueAttributeName(const char *objName, char *&attrName);

    Boolean uniqueObjectName(ObjectName &objectName);

    static  const ClassName &name();
    virtual const ClassName &className() const;
    virtual const ClassName &impleClassName() const;

    static  NameService *create(const ObjectName &objectName);
    static  NameService *create(const ClassName &nameServiceImpleName);

    virtual Resource *control(const ClassName &ctrlInterName);

    NameService& operator= (const NameService&);
    
    NameService(const ObjectName &objectName);
    NameService(const ClassName &nameServiceImpleName);
    NameService(const NameService &nameService);
    NameService(NameServiceImple *_nameServiceImple);    
    virtual ~NameService();

    static  NameService *castup(Resource *resource);
    virtual void        *castup(const ClassName &className) const;

private:
    NameServiceImple *_imple;
};

#endif
