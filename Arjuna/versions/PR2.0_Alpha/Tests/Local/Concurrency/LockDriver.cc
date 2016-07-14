/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockDriver.cc,v 1.6 1993/03/18 15:07:25 n048z Exp $
 */

#include <System/iostream.h>

#include "TestLock.h"

extern "C"
{
    int fork();
    void perror(const char*);
}

void main()
{
    LockCode res;
    TestLock lock;
    int child;
    int ok;

    if ((child = fork()) == -1)
	perror("Error: fork ");
    else
    {
	if (!child)
	{
	    ok = 0;
	    for (int i = 0; i < 10; i++)
	    {
		res = lock.acquire(i, TRUE);
		if (res == Set)
		    cout << "Parent Acquired lock " << i << "\n" << flush;
		else
		{
		    ok++;
		    cout << "Parent Cannot acquire lock " << i << "\n" << flush;
		}
	    }

	    if (ok < 10)
		for (int j = 0; j < 10; j++)
		{
		    cout << "Parent Releasing lock " << j << "\n" << flush;
		    if (lock.release(j, TRUE))
		    {
			cout << "Parent has released all locks held.\n" << flush;
			break;
		    }
		    else
			cout << "Could not release lock.\n" << flush;
		}
	}
	else
	{
	    ok = 0;
	    for (int i = 0; i < 10; i++)
	    {
		res = lock.acquire(i, FALSE);
		if (res == Set)
		    cout << "Child Acquired lock " << i << "\n" << flush;
		else
		{
		    ok++;
		    cout << "Child Cannot acquire lock " << i << "\n" << flush;
		}
	    }

	    if (ok < 10)
		for (int j = 0; j < 10; j++)
		{
		    cout << "Child Releasing lock " << j << "\n" << flush;
		    if (lock.release(j, FALSE))
		    {
			cout << "Child has released all locks held.\n" << flush;
			break;
		    }
		    else
			cout << "Could not release lock.\n" << flush;
		}
	}
    }
}
