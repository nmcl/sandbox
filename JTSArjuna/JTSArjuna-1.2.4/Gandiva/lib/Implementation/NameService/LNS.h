/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LNS.h,v 1.8 1996/02/08 13:21:10 nsmw Exp $
 */

#ifndef IMPLEMENTATION_NAMESERVICE_LNS_H_
#define IMPLEMENTATION_NAMESERVICE_LNS_H_

#ifndef OS_STDDEF_H_
#   include <os/stddef.h>
#endif

#ifndef OS_NDBM_H_
#   include <os/ndbm.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#   include <Common/ObjectName.h>
#endif

class Uid;

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef INTERFACE_NAMESERVICE_H_
#   include <Interface/NameService.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_H_
#   include <Implementation/NameService.h>
#endif

class LNSNameServiceImple : public NameServiceImple
{
public:
    Boolean attributeType(const char *objName, const char *attrName,
			  ObjectName::AttributeType &attrType);
    Boolean firstAttributeName(const char *objName, char *&attrName);
    Boolean nextAttributeName(const char *objName, const char *attrName, char *&nextAttrName);

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

    Boolean uniqueObjectName(ObjectName &value);

    static  const ClassName &name();
    virtual const ClassName &className() const;

    static  Resource *create();

    virtual Resource *control(const ClassName &ctrlInterName);
    
    virtual NameServiceImple* clone ();
    
    static  LNSNameServiceImple *castup(Resource *resource);
    virtual void                *castup(const ClassName &className) const;

protected:
    Boolean openDBForReading(DBM *&dbm);
    Boolean openDBForWriting(DBM *&dbm);
    Boolean closeDB(DBM *dbm);

    LNSNameServiceImple();
    virtual ~LNSNameServiceImple();
};

#endif
