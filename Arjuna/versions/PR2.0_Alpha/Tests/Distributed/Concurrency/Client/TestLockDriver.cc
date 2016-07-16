/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TestLockDriver.cc,v 1.11 1993/03/18 15:06:10 n048z Exp $
 */

#include <Configure.h>

#include <System/iostream.h>
#include <System/stdlib.h>

#include <Common/Boolean.h>

#include "TestLock_stub.h"

#if RPC_SYSTEM_TYPE == RPC_ANSAAction_ || RPC_SYSTEM_TYPE == RPC_ANSA_
int mainbody(int, char**)
#else
int main(int, char**)
#endif
{
    int ok, val = -1;
    Boolean res;
    TestLock lock(val);

    if (val != 0)
    {
	cout << "Constructor error\n" << flush;
	exit(0);
    }

    ok = 0;
    for (int i = 0; i < 10; i++)
    {
	res = FALSE;
	res = lock.acquire(i, TRUE);
	if (res)
	    cout << "Acquired lock " << i << "\n" << flush;
	else
	{
	    ok++;
	    cerr << "Cannot acquire lock " << i << "\n" << flush;
	}
    }
    
    if (ok < 10)
	for (int j = 0; j < 10; j++)
	{
	    cout << "Releasing lock " << j << "\n" << flush;
	    if (lock.release(j, TRUE))
	    {
		cout << "Released all held locks.\n" << flush;
		return 1;
	    }
	}

    return 0;
}
