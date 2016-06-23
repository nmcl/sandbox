/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NTMutex.cc,v 1.1 1998/09/16 11:03:30 nmcl Exp $
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef NTMUTEX_H_
#  include "NTMutex.h"
#endif

ThreadMutex* ThreadMutex::create (Boolean recurs)
{
    return new NTMutex(recurs);
}

NTMutex::NTMutex (Boolean recurs)
		 : mutex(0),
		   owner(0),
		   count(0),
		   recursive(recurs),
		   valid(FALSE)
{
    mutex = CreateMutex(NULL, 0, NULL);

    if (mutex)
	valid = TRUE;
}

NTMutex::~NTMutex ()
{
    if (valid)
    {
	CloseHandle(mutex);
	Sleep(0);
    }
}

Boolean NTMutex::lock ()
{
    Boolean result = FALSE;

    if (valid)
    {
	DWORD tid = GetCurrentThreadId();

	if (recursive && (tid == owner))
	{
	    count++;
	    result = TRUE;
	}
	else
	{
	    WaitForSingleObject(mutex, INFINITE);
	    count = 1;
	    owner = tid;
	    result = TRUE;
	}
    }

    return result;
}

Boolean NTMutex::unlock ()
{
    Boolean result = FALSE;

    if (valid)
    {
	DWORD tid = GetCurrentThreadId();

	if (recursive)
	{
	    if (tid == owner)
	    {
		result = TRUE;

		if (--count == 0)
		{
		    owner = 0;
		    count = 0;

		    if (ReleaseMutex(mutex) != 0)
			result = FALSE;
		}
	    }
	    else
	    {
		/*
		 * Not the owner, so shouldn't be calling unlock!
		 */
	    }
	}
	else
	{
	    if (ReleaseMutex(mutex) != 0)
		result = FALSE;
	    else
		result = TRUE;
	}
    }

    return result;
}

Boolean NTMutex::tryLock ()
{
    Boolean result = FALSE;

    if (valid)
    {
	DWORD tid = GetCurrentThreadId();

	if (recursive && (tid == owner))
	{
	    count++;
	    result = TRUE;
	}
	else
	{
	    if (WaitForSingleObject(mutex, 0) != WAIT_TIMEOUT)
	    {
		count = 1;
		owner = tid;
		result = TRUE;
	    }
	}
    }

    return result;
}
