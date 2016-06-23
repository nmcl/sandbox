/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NTSpecific.cc,v 1.1 1998/09/16 11:03:31 nmcl Exp $
 */

#ifndef NTSPECIFIC_H_
#  include "NT_Specific.h"
#endif

ThreadSpecific* ThreadSpecific::create ()
{
    NTSpecific* temp = new NTSpecific();

    if (temp)
    {
	if (!temp->valid())
	{
	    delete temp;
	    temp = 0;
	}
    }
	
    return temp;
}

NTSpecific::NTSpecific ()
		       : _key(0)
{
    _key = TlsAlloc();

    if (_key != errorValue)
	valid(TRUE);
}

NTSpecific::~NTSpecific ()
{
    (void) TlsFree(_key);
}

Boolean NTSpecific::setSpecific (void* threadData)
{
    if (valid())
    {
	if (TlsSetValue(_key, (LPVOID) threadData))
	    return TRUE;
	else
	    return addToOverflow(threadData);
    }

    return FALSE;
}

Boolean NTSpecific::getSpecific (void*& threadData)
{
    if (valid())
    {
	LPVOID ptr = TlsGetValue(_key);

	if ((ptr) || ((!ptr) && (GetLastError() == NO_ERROR)))
	{
	    threadData = ptr;
	    return TRUE;
	}
	else
	    return getFromOverflow(threadData);
    }
    
    return FALSE;
}

Boolean NTSpecific::destroySpecific ()
{
    if (valid())
    {
	if (TlsSetValue(_key, (LPVOID) 0))
	    return TRUE;
	else
	    return removeFromOverflow();
    }

    return FALSE;
}
