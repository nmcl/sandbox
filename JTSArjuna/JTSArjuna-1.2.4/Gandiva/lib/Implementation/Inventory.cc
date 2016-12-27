/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Inventory.cc,v 1.3 1996/01/02 13:15:44 nsmw Exp $
 */

#include <os/stddef.h>

#ifndef GANDIVA_H_
#   include <Gandiva.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef IMPLEMENTATION_INVENTORY_H_
#   include <Implementation/Inventory.h>
#endif

InventoryImple *InventoryImple::_inventImple = NULL;

const ClassName &InventoryImple::name()
{
    return Gandiva::Implementation::Inventory::name();
}

const ClassName &InventoryImple::className() const
{
    return Gandiva::Implementation::Inventory::name();
}

InventoryImple *InventoryImple::castup(Resource *resource)
{
    if (resource != NULL)
        return (InventoryImple *) resource->castup(InventoryImple::name());
    else
        return NULL;
}

void *InventoryImple::castup(const ClassName &className) const
{
    if (className == InventoryImple::name())
        return (void *) this;
    else
        return NULL;
}

InventoryImple::InventoryImple()
{
}

InventoryImple::~InventoryImple()
{
}

