/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: GandivaDedicated.h,v 1.3 1996/01/02 13:15:58 nsmw Exp $
 */

#ifndef IMPLEMENTATION_INVENTORY_GANDIVADEDICATED_H_
#define IMPLEMENTATION_INVENTORY_GANDIVADEDICATED_H_

#ifndef OS_STDDEF_H_
#   include <os/stddef.h>
#endif

#ifndef OS_IOSTREAM_H_
#   include <os/iostream.h>
#endif

class GandivaDedicatedInventoryImple;

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef IMPLEMENTATION_INVENTORY_H_
#   include <Implementation/Inventory.h>
#endif

class GandivaDedicatedInventoryImple : public InventoryImple
{
public:
    virtual Resource *createVoid(const ClassName &className);
    virtual Resource *createClassName(const ClassName &className, const ClassName &paramClassName);
    virtual Resource *createObjectName(const ClassName &className, const ObjectName &paramObjectName);
    virtual Resource *createResources(const ClassName &className, Resource *paramResources[]);
    virtual Resource *createClassNameResources(const ClassName &className, const ClassName &paramClassName, Resource *paramResources[]);
    virtual Resource *createObjectNameResources(const ClassName &className, const ObjectName &paramObjectName, Resource *paramResources[]);

    virtual Boolean  addCreateVoid(const ClassName &className, Resource *(*create)(void));
    virtual Boolean  addCreateClassName(const ClassName &className, Resource *(*create)(const ClassName &className));
    virtual Boolean  addCreateObjectName(const ClassName &className, Resource *(*create)(const ObjectName &objectName));
    virtual Boolean  addCreateResources(const ClassName &className, Resource *(*create)(Resource *resources[]));
    virtual Boolean  addCreateClassNameResources(const ClassName &className, Resource *(*create)(const ClassName &className, Resource *resources[]));
    virtual Boolean  addCreateObjectNameResources(const ClassName &className, Resource *(*create)(const ObjectName &objectName, Resource *resources[]));
 
    virtual Boolean addInventory(Inventory *inventory);

    static  const ClassName &name();
    virtual const ClassName &className() const;

    virtual void print(ostream &ostr);

    static  Resource *create();

    virtual Resource *control(const ClassName &ctrlInterName);

    static  GandivaDedicatedInventoryImple *castup(Resource *resource);
    virtual void                           *castup(const ClassName &className) const;

protected:
    GandivaDedicatedInventoryImple();
    virtual ~GandivaDedicatedInventoryImple();
};

#endif
