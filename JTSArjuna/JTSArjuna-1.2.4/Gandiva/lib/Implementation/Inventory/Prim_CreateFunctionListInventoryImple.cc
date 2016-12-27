/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Prim_CreateFunctionListInventoryImple.cc,v 1.2 1996/01/02 13:16:02 nsmw Exp $
 */

#include <os/stddef.h>

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef INTERFACE_INVENTORY_H_
#   include <Interface/Inventory.h>
#endif

#ifndef IMPLEMENTATION_INVENTORY_H_
#   include <Implementation/Inventory.h>
#endif

#ifndef IMPLEMENTATION_INVENTORY_CREATEFUNCTIONLIST_H_
#   include <Implementation/Inventory/CreateFunctionList.h>
#endif

InventoryImple *InventoryImple::inventoryImple()
{
    if (_inventImple == NULL)
    {
        _inventImple = InventoryImple::castup(CreateFunctionListInventoryImple::create());
        Resource::ref(_inventImple);
    }

    return _inventImple;
}

