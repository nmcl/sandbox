/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Init_CreateFunctionListInventoryImple.cc,v 1.3 1996/01/02 13:16:00 nsmw Exp $
 */

#include <os/stddef.h>

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

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


ImplePresent(CreateFunctionListInventory)

class CreateFunctionListInsertVoid
{
public:
    CreateFunctionListInsertVoid ()
    {
	if (added == FALSE)
	{
	    Inventory* _invent = Inventory::inventory();

	    if (_invent)
	    {
		Resource::ref(_invent);
		
		if (_invent->addCreateVoid(CreateFunctionListInventoryImple::name(),
					   CreateFunctionListInventoryImple::create))
		{
#ifdef DEBUG
		    cout << "CreateFunctionListInsertVoid - Added to inventory void: "
			 << CreateFunctionListInventoryImple::name() << endl;
#endif
		    added = TRUE;
		}
		else
		    cerr << "CreateFunctionListInsertVoid - failed to add " << CreateFunctionListInventoryImple::name() << endl;

		Resource::unref(_invent);
	    }
	    else
		cerr << "CreateFunctionListInsertVoid - error, no inventory present!" << endl;
	}
    }

    ~CreateFunctionListInsertVoid () {};

private:
    static Boolean added;
};

Boolean CreateFunctionListInsertVoid::added = FALSE;

static CreateFunctionListInsertVoid initCreateFunctionListInventoryImple;
