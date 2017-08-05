/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: UidTest1.cc,v 1.3 1997/01/27 17:22:40 nsmw Exp $
 */

#include <os/stdlib.h>
#include <os/iostream.h>

#ifndef COMMON_UID_H_
#  include <Common/Uid.h>
#endif

#ifndef INTERFACE_BUFFER_H_
#  include <Interface/Buffer.h>
#endif

#ifndef GANDIVA_H_
#  include <Gandiva.h>
#endif


int main (int, char**)
{
    cout << "Testing uniqueness." << endl;
    
    for (int i = 0; i < 100; i++)
    {
	Uid u;
	cout << u << endl;
    }

    cout << "\nTesting stringForm." << endl;
    
    Uid dummy;
    char* ptr = dummy.stringForm();
    
    cout << "String form of " << dummy << " is " << ptr << endl;

    if (ptr)
	::delete [] ptr;

    cout << "\nTesting pack/unpack." << endl;
    
    Buffer buff(Gandiva::Implementation::Buffer::QANBO::name());

    dummy.pack(buff);

    Uid blank(Uid::invalid());
    blank.unpack(buff);

    if (blank == dummy)
	cout << "Pack/unpack ok" << endl;
    else
	cout << "Pack/unpack error" << endl;
    
    return 0;
}
