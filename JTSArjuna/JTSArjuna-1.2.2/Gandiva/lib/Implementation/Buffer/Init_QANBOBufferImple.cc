/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Init_QANBOBufferImple.cc,v 1.4 1996/01/02 13:15:49 nsmw Exp $
 */

#ifndef OS_STDDEF_H_
#  include <os/stddef.h>
#endif

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef INTERFACE_INVENTORY_H_
#   include <Interface/Inventory.h>
#endif

#ifndef IMPLEMENTATION_BUFFER_QANBO_H_
#   include <Implementation/Buffer/QANBO.h>
#endif

ImplePresent(QANBOBuffer)


class QANBOInsertVoid
{
public:
    QANBOInsertVoid ()
    {
	if (added == FALSE)
	{
	    Inventory* _invent = Inventory::inventory();

	    if (_invent)
	    {
		Resource::ref(_invent);
		
		if (_invent->addCreateVoid(QANBOBufferImple::name(), QANBOBufferImple::create))
		{
#ifdef DEBUG
		    cout << "QANBOInsertVoid - Added to inventory void: "
			 << QANBOBufferImple::name() << endl;
#endif
		    added = TRUE;
		}
		else
		    cerr << "QANBOInsertVoid - failed to add " << QANBOBufferImple::name() << endl;

		Resource::unref(_invent);
	    }
	    else
		cerr << "QANBOInsertVoid - error, no inventory present!" << endl;
	}
    }

    ~QANBOInsertVoid () {};

private:
    static Boolean added;
};

Boolean QANBOInsertVoid::added = 0;

static QANBOInsertVoid initQANBOBufferImple;
