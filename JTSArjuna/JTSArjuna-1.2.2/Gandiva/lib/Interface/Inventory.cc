/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Inventory.cc,v 1.4 1996/01/02 13:34:27 nsmw Exp $
 */

#include <os/stddef.h>

#ifndef GANDIVA_H_
#   include <Gandiva.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef IMPLEMENTATION_INVENTORY_H_
#   include <Implementation/Inventory.h>
#endif

#ifndef INTERFACE_INVENTORY_H_
#   include <Interface/Inventory.h>
#endif

Inventory *Inventory::_invent = NULL;

Resource *Inventory::createVoid(const ClassName &className) const
{
    Resource *res = NULL;

    if (_imple != NULL)
        res = _imple->createVoid(className);

    if (res == NULL)
        cerr << "WARNING: Inventory::createVoid(" << className
	     << ") failed" << endl;

    return res;
}

Resource *Inventory::createClassName(const ClassName &className,
				     const ClassName &paramClassName) const
{
    Resource *res = NULL;

    if (_imple != NULL)
        res = _imple->createClassName(className, paramClassName);

    if (res == NULL)
        cerr << "WARNING: Inventory::createClassName(" << className
	     << ", " << paramClassName << ") failed" << endl;

    return res;
}

Resource *Inventory::createObjectName(const ClassName &className,
				      const ObjectName &paramObjectName) const
{
    Resource *res = NULL;

    if (_imple != NULL)
        res = _imple->createObjectName(className, paramObjectName);

    if (res == NULL)
        cerr << "WARNING: Inventory::createObjectName(" << className
	     << ", " << paramObjectName << ") failed" << endl;

    return res;
}

Resource *Inventory::createResources(const ClassName &className,
				     Resource *paramResources[]) const
{
    Resource *res = NULL;

    if (_imple != NULL)
        res = _imple->createResources(className, paramResources);

    if (res == NULL)
        cerr << "WARNING: Inventory::createResources(" << className
	     << ", " << (void*) paramResources << ") failed" << endl;

    return res;
}

Resource *Inventory::createClassNameResources(const ClassName &className,
					      const ClassName &paramClassName,
					      Resource *paramResources[]) const
{
    Resource *res = NULL;

    if (_imple != NULL)
        res = _imple->createClassNameResources(className, paramClassName,
					       paramResources);

    if (res == NULL)
        cerr << "WARNING: Inventory::createClassNameResources(" << className
	     << ", " << paramClassName << ", " << (void*) paramResources
	     << ") failed" << endl;

    return res;
}

Resource *Inventory::createObjectNameResources(const ClassName &className,
					       const ObjectName &paramObjectName,
					       Resource *paramResources[]) const
{
    Resource *res = NULL;

    if (_imple != NULL)
        res = _imple->createObjectNameResources(className, paramObjectName,
						paramResources);

    if (res == NULL)
        cerr << "WARNING: Inventory::createObjectNameResources(" << className
	     << ", " << paramObjectName << ", " << (void*) paramResources
	     << ") failed" << endl;

    return res;
}

Boolean Inventory::addCreateVoid(const ClassName &className,
				 Resource *(*create)(void))
{
    Boolean res = FALSE;

    if (_imple != NULL)
        res = _imple->addCreateVoid(className, create);

    if (! res)
        cerr << "WARNING: ObjectName::addCreateVoid(" << className << ", "
	     << (void*) create << " << ) failed" << endl;

    return res;
}

Boolean Inventory::addCreateClassName(const ClassName &className,
				      Resource *(*create)(const ClassName &className))
{
    Boolean res = FALSE;

    if (_imple != NULL)
        res = _imple->addCreateClassName(className, create);

    if (! res)
        cerr << "WARNING: ObjectName::addCreateClassName(" << className
	     << ", " << (void*) create << " << ) failed" << endl;

    return res;
}

Boolean Inventory::addCreateObjectName(const ClassName &className,
				       Resource *(*create)(const ObjectName &objectName))
{
    Boolean res = FALSE;

    if (_imple != NULL)
        res = _imple->addCreateObjectName(className, create);

    if (! res)
        cerr << "WARNING: ObjectName::addCreateObjectName(" << className
	     << ", " << (void*) create << " << ) failed" << endl;

    return res;
}

Boolean Inventory::addCreateResources(const ClassName &className,
				      Resource *(*create)(Resource *resources[]))
{
    Boolean res = FALSE;

    if (_imple != NULL)
        res = _imple->addCreateResources(className, create);

    if (! res)
        cerr << "WARNING: ObjectName::addCreateResources(" << className
	     << ", " << (void*) create << " << ) failed" << endl;

    return res;
}

Boolean Inventory::addCreateClassNameResources(const ClassName &className,
					       Resource *(*create)(const ClassName &className, Resource *resources[]))
{
    Boolean res = FALSE;

    if (_imple != NULL)
        res = _imple->addCreateClassNameResources(className, create);

    if (! res)
        cerr << "WARNING: ObjectName::addCreateClassNameResources("
	     << className << ", " << (void*) create << " << ) failed" << endl;

    return res;
}

Boolean Inventory::addCreateObjectNameResources(const ClassName &className,
						Resource *(*create)(const ObjectName &objectName, Resource *resources[]))
{
    Boolean res = FALSE;

    if (_imple != NULL)
        res = _imple->addCreateObjectNameResources(className, create);

    if (! res)
        cerr << "WARNING: ObjectName::addCreateObjectNameResources("
	     << className << ", " << (void*) create << " << ) failed" << endl;

    return res;
}

Boolean Inventory::addInventory(Inventory *inventory)
{
    Boolean res = FALSE;

    if (_imple != NULL)
        res = _imple->addInventory(inventory);

    return res;
}

const ClassName &Inventory::name()
{
    return Gandiva::Interface::Inventory::name();
}

const ClassName &Inventory::className() const
{
    return Gandiva::Interface::Inventory::name();
}

const ClassName &Inventory::impleClassName() const
{
    if (_imple != NULL)
        return _imple->className();
    else
        return ClassName::invalid();
}

Inventory *Inventory::create(const ClassName &inventImpleName)
{
    Inventory *res    = NULL;
    Inventory *invent = Inventory::inventory();

    if (invent != NULL)
    {
        Resource::ref(invent);

        InventoryImple *inventImple = InventoryImple::castup(invent->createVoid(inventImpleName));

        if (inventImple != NULL)
            res = new Inventory(inventImple);

        Resource::unref(invent);
    }

    return res;
}

Inventory *Inventory::inventory()
{
    if (_invent == NULL)
    {
        InventoryImple *inventImple = InventoryImple::inventoryImple();

        if (inventImple != NULL)
        {
            _invent = new Inventory(inventImple);
            Resource::ref(_invent);
        }
    }

    return _invent;
}

Resource *Inventory::control(const ClassName &ctrlInterName)
{
    if (_imple != NULL)
        return _imple->control(ctrlInterName);
    else
        return NULL;
}

Inventory *Inventory::castup(Resource *resource)
{
    if (resource != NULL)
        return (Inventory *) resource->castup(Inventory::name());
    else
        return NULL;
}

void *Inventory::castup(const ClassName &className) const
{
    if (className == Inventory::name())
        return (void *) this;
    else
        return NULL;
}

Inventory::Inventory(const ClassName &inventImpleName)
{
    Inventory *invent = Inventory::inventory();

    _imple = NULL;

    if (invent != NULL)
    {
        Resource::ref(invent);

        _imple = InventoryImple::castup(invent->createVoid(inventImpleName));
        Resource::ref(_imple);

        Resource::unref(invent);
    }
}

Inventory::Inventory(const Inventory &invent)
{
    _imple = invent._imple;
    Resource::ref(_imple);
}

Inventory::Inventory(InventoryImple *imple)
{
    _imple = imple;
    Resource::ref(_imple);
}

Inventory::~Inventory()
{
    Resource::unref(_imple);
}

void Inventory::print(ostream &ostr)
{
    if (_imple != NULL)
        _imple->print(ostr);
    else
        ostr << "<Inventory::_imple=NULL>";
}

ostream &operator<< (ostream &ostr, Inventory &invent)
{
    invent.print(ostr);
    return ostr;
} 
