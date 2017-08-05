/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClassNameTest2.cc,v 1.3 1997/01/27 17:22:35 nsmw Exp $
 */

#include <os/stddef.h>
#include <os/iostream.h>

#ifndef COMMON_CLASSNAME_H_
#  include <Common/ClassName.h>
#endif


int main (int, char**)
{
    ClassName c1("Class1");
    ClassName c2("Class2");
    ClassName c3(NULL);

    cout << "Class c1 is: " << c1 << endl;

    cout << "\nClass c2 is : " << c2 << endl;

    cout << "\nClass c3 is: " << c3 << endl;

    if (c1 != c2)
	cout << "c1 != c2\n" << endl;
    else
	cout << "c1 == c2\n" << endl;
    
    return 0;
}
