/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Init_SNSNameServiceImple.cc,v 1.2 1996/01/02 13:16:16 nsmw Exp $
 */

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef CORE_RESOURCE_H_
#  include <Core/Resource.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMON_CLASSNAME_H_
#  include <Common/ClassName.h>
#endif

#ifndef INTERFACE_INVENTORY_H_
#  include <Interface/Inventory.h>
#endif

#ifndef IMPLEMENTATION_NAMESERVICE_SNS_H_
#  include <Implementation/NameService/SNS.h>
#endif

ImplePresent(SNSNameService)

class SNSInsertVoid
{
public:
    SNSInsertVoid ()
    {
	if (added == FALSE)
	{
	    Inventory* _invent = Inventory::inventory();

	    if (_invent)
	    {
		Resource::ref(_invent);
		
		if (_invent->addCreateVoid(SNSNameServiceImple::name(),
					   SNSNameServiceImple::create))
		{
#ifdef DEBUG
		    cout << "SNSInsertVoid - Added to inventory void: "
			 << SNSNameServiceImple::name() << endl;
#endif
		    added = TRUE;
		}
		else
		    cerr << "SNSInsertVoid - failed to add " << SNSNameServiceImple::name() << endl;

		Resource::unref(_invent);
	    }
	    else
		cerr << "SNSInsertVoid - error, no inventory present!" << endl;
	}
    }

    ~SNSInsertVoid () {};

private:
    static Boolean added;
};

Boolean SNSInsertVoid::added = 0;

static SNSInsertVoid SNSNameServiceVoidSetup;
