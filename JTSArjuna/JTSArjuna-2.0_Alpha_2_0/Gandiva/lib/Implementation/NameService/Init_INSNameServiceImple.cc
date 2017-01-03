/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Init_INSNameServiceImple.cc,v 1.4 1996/01/02 13:16:15 nsmw Exp $
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

#ifndef IMPLEMENTATION_NAMESERVICE_INS_H_
#   include <Implementation/NameService/INS.h>
#endif

ImplePresent(INSNameService)

class INSInsertVoid
{
public:
    INSInsertVoid ()
    {
	if (added == FALSE)
	{
	    Inventory* _invent = Inventory::inventory();

	    if (_invent)
	    {
		Resource::ref(_invent);
		
		if (_invent->addCreateVoid(INSNameServiceImple::name(),
					   INSNameServiceImple::create))
		{
#ifdef DEBUG
		    cout << "INSInsertVoid - Added to inventory void: "
			 << INSNameServiceImple::name() << endl;
#endif
		    added = TRUE;
		}
		else
		    cerr << "INSInsertVoid - failed to add " << INSNameServiceImple::name() << endl;

		Resource::unref(_invent);
	    }
	    else
		cerr << "INSInsertVoid - error, no inventory present!" << endl;
	}
    }

    ~INSInsertVoid () {};

private:
    static Boolean added;
};

Boolean INSInsertVoid::added = FALSE;


static INSInsertVoid initINSNameServiceImple;
