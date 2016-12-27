/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArrayTest1.cc,v 1.3 1997/01/27 17:22:30 nsmw Exp $
 */

#include <os/iostream.h>

#ifndef TEMPLATE_ARRAY_H_
#  include <Template/Array.h>
#endif

const char* name1 = " Hello";
const char* name2 = " World.";
const char* name3 = " To be,";
const char* name4 = " or not to be.";
const char* name5 = " That is the question.";


int main (int, char**)
{
    Array<const char*> array1(2);
    Array<const char*> array2(3);
    int i = 0;

    array1[0] = name1;
    array1[1] = name2;

    for (i = 0; i < 2; i++)
	cout << array1[i];
    cout << endl;

    array2[0] = name3;
    array2[1] = name4;
    array2[2] = name5;

    for (i = 0; i < 3; i++)
	cout << array2[i];
    cout << endl;

    array1.resize(5);
    array1[2] = name3;
    array1[3] = name4;
    array1[4] = name5;

    for (i = 0; i < 5; i++)
	cout << array1[i];
    cout << endl;

    array1 = array2;

    array1.print(cout);
    
    return 0;
}
