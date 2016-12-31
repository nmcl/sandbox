/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArrayTest2.cc,v 1.3 1997/01/27 17:22:32 nsmw Exp $
 */

#include <os/iostream.h>

#ifndef TEMPLATE_ARRAY_H_
#  include <Template/Array.h>
#endif


int main (int, char**)
{
    Array<int> array1(0);
    Array<int> array2(2);

    array2[0] = 0;
    array2[1] = 1;
    
    cout << "size of array1 " << array1.getSize() << endl;
    cout << "size of array2 " << array2.getSize() << endl;

    array1.setSize(array2.getSize());

    array1 = array2;

    for (int i = 0; i < 2; i++)
    {
	cout << "array1[" << i << "] : " << array1[i] << endl;
	cout << "array2[" << i << "] : " << array2[i] << endl;
    }

    return 0;
}
