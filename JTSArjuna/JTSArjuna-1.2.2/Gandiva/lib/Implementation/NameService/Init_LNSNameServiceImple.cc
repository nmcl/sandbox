/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Init_LNSNameServiceImple.cc,v 1.4 1996/01/02 13:38:43 nsmw Exp $
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

#ifndef IMPLEMENTATION_NAMESERVICE_H_
#   include <Implementation/NameService.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_LNS_H_
#   include <Implementation/NameService/LNS.h>
#endif


ImplePresent(LNSNameService)

class LNSInsertVoid
{
public:
    LNSInsertVoid ()
    {
	if (added == FALSE)
	{
	    Inventory* _invent = Inventory::inventory();

	    if (_invent)
	    {
		Resource::ref(_invent);
		
		if (_invent->addCreateVoid(LNSNameServiceImple::name(),
					   LNSNameServiceImple::create))
		{
#ifdef DEBUG
		    cout << "LNSInsertVoid - Added to inventory void: "
			 << LNSNameServiceImple::name() << endl;
#endif
		    added = TRUE;
		}
		else
		    cerr << "LNSInsertVoid - failed to add " << LNSNameServiceImple::name() << endl;

		Resource::unref(_invent);
	    }
	    else
		cerr << "LNSInsertVoid - error, no inventory present!" << endl;
	}
    }

    ~LNSInsertVoid () {};

private:
    static Boolean added;
};

Boolean LNSInsertVoid::added = FALSE;

static LNSInsertVoid initLNSNameServiceImple;
