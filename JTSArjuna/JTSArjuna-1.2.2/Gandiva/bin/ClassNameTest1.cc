/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClassNameTest1.cc,v 1.3 1997/01/27 17:22:33 nsmw Exp $
 */

#include <os/iostream.h>

#ifndef COMMON_CLASSNAME_H_
#  include <Common/ClassName.h>
#endif

#ifndef INTERFACE_BUFFER_H_
#  include <Interface/Buffer.h>
#endif

#ifndef GANDIVA_H_
#  include <Gandiva.h>
#endif


int main (int, char**)
{
    ClassName c1("Class1");
    ClassName c2("Class2");
    ClassName c3(NULL);
    Buffer buff(Gandiva::Implementation::Buffer::QANBO::name());

    cout << "Class c1 is: " << c1 << endl;

    cout << "\nClass c2 is : " << c2 << endl;

    cout << "\nClass c3 is: " << c3 << endl;

    if (c1 != c2)
	cout << "c1 != c2\n" << endl;
    else
	cout << "c1 == c2\n" << endl;

    if (c2.pack(buff))
    {
	cout << "Packed c2 correctly." << endl;
	if (c2.unpack(buff))
	    cout << "Unpacked " << c2 << endl;
    }

    char* ptr = c2.stringForm();

    cout << "c2 string form is " << ptr << endl;

    ::delete [] ptr;

    ClassName c4(c2);

    cout << "\nCopy constructor c4(c2) returned: " << c4 << endl;
    
    return 0;
}
