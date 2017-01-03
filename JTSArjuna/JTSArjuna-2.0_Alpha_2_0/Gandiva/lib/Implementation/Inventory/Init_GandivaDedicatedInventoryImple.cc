/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Init_GandivaDedicatedInventoryImple.cc,v 1.4 1996/01/02 13:16:02 nsmw Exp $
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

#ifndef IMPLEMENTATION_INVENTORY_GANDIVADEDICATED_H_
#   include <Implementation/Inventory/GandivaDedicated.h>
#endif


ImplePresent(GandivaDedicatedInventory)

class GandivaDedicatedInsertVoid
{
public:
    GandivaDedicatedInsertVoid ()
    {
	if (added == FALSE)
	{
	    Inventory* _invent = Inventory::inventory();

	    if (_invent)
	    {
		Resource::ref(_invent);
		
		if (_invent->addCreateVoid(GandivaDedicatedInventoryImple::name(),
					   GandivaDedicatedInventoryImple::create))
		{
#ifdef DEBUG
		    cout << "GandivaDedicatedInsertVoid - Added to inventory void: "
			 << GandivaDedicatedInventoryImple::name() << endl;
#endif
		    added = TRUE;
		}
		else
		    cerr << "GandivaDedicatedInsertVoid - failed to add "
			 << GandivaDedicatedInventoryImple::name() << endl;

		Resource::unref(_invent);
	    }
	    else
		cerr << "GandivaDedicatedInsertVoid - error, no inventory present!" << endl;
	}
    }

    ~GandivaDedicatedInsertVoid () {};

private:
    static Boolean added;
};

Boolean GandivaDedicatedInsertVoid::added = FALSE;

static GandivaDedicatedInsertVoid initGandivaDedicatedInventoryImple;
