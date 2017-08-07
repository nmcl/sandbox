/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Inventory.h,v 1.3 1996/01/02 13:12:53 nsmw Exp $
 */

#ifndef IMPLEMENTATION_INVENTORY_H_
#define IMPLEMENTATION_INVENTORY_H_

#ifndef OS_STDDEF_H_
#   include <os/stddef.h>
#endif

class InventoryImple;

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef COMMON_OBJECTNAME_H_
#   include <Common/ObjectName.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#   include <Common/ClassName.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef INTERFACE_INVENTORY_H_
#   include <Interface/Inventory.h>
#endif

class InventoryImple : virtual public Resource
{
public:
    virtual Resource *createVoid(const ClassName &className) = 0;
    virtual Resource *createClassName(const ClassName &className, const ClassName &paramClassName) = 0;
    virtual Resource *createObjectName(const ClassName &className, const ObjectName &paramObjectName) = 0;
    virtual Resource *createResources(const ClassName &className, Resource *paramResources[]) = 0;
    virtual Resource *createClassNameResources(const ClassName &className, const ClassName &paramClassName, Resource *paramResources[]) = 0;
    virtual Resource *createObjectNameResources(const ClassName &className, const ObjectName &paramObjectName, Resource *paramResources[]) = 0;

    virtual Boolean addCreateVoid(const ClassName &className, Resource *(*create)(void)) = 0;
    virtual Boolean addCreateClassName(const ClassName &className, Resource *(*create)(const ClassName &className)) = 0;
    virtual Boolean addCreateObjectName(const ClassName &className, Resource *(*create)(const ObjectName &objectName)) = 0;
    virtual Boolean addCreateResources(const ClassName &className, Resource *(*create)(Resource *resources[])) = 0;
    virtual Boolean addCreateClassNameResources(const ClassName &className, Resource *(*create)(const ClassName &className, Resource *resources[])) = 0;
    virtual Boolean addCreateObjectNameResources(const ClassName &className, Resource *(*create)(const ObjectName &objectName, Resource *resources[])) = 0;

    virtual Boolean addInventory(Inventory *inventory) = 0;
 
    static  const ClassName &name();
    virtual const ClassName &className() const;

    virtual void print(ostream &ostr) = 0;

    static  InventoryImple *inventoryImple();

    virtual Resource *control(const ClassName &ctrlInterName) = 0;

    static  InventoryImple *castup(Resource *resource);
    virtual void           *castup(const ClassName &className) const;

protected:
    InventoryImple();
    virtual ~InventoryImple();

private:
    static InventoryImple  *_inventImple;
};

#endif
