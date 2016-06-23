/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadTest.cc,v 1.2 1998/11/20 08:28:58 nmcl Exp $
 */

#include <iostream.h>
#include <Thread/Thread.h>

#include <Common/Uid.h>

void* printA (void*)
{
    for (int i = 0; i < 1000; i++)
    {
	cout << 'A';
	Thread::yield();
    }
    
    return 0;
}

void* printB (void*)
{
    for (int i = 0; i < 1000; i++)
    {
	cout << 'B';
	Thread::yield();
    }

    return 0;
}

int main (int, char**)
{
    Uid u1;

    cout << u1 << endl;

    return 0;
    
    Thread* t1 = new Thread(printA, NULL);
    Thread* t2 = new Thread(printB, NULL);

    t1->resume();
    t2->resume();
    
    cout << "main waiting" << endl;

    if (!t1->join())
	cout << "\njoin1 failed" << endl;
    else
	cout << "\njoined with thread1" << endl;
    
    if (!t2->join())
	cout << "\njoin2 failed" << endl;
    else
	cout << "\njoined with thread2" << endl;

    delete t1;
    delete t2;

    return 0;
}
