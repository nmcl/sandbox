/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NT_SpecificData.cc,v 1.2 1998/02/28 13:12:05 nmcl Exp $
 */

#include <os/windows.h>

#ifndef BASE_THREADDATAIMPLE_H_
#  include <Base/ThreadDataImple.h>
#endif

const DWORD errorValue = 0xFFFFFFFF;

class NT_SpecificDataImple : public ThreadDataImple
{
public:
    NT_SpecificDataImple ();
    ~NT_SpecificDataImple ();

    virtual Boolean setSpecific    (void* threadData);
    virtual Boolean getSpecific    (void*& threadData);
    virtual Boolean removeSpecific ();
    
private:
    DWORD _key;
};


NT_SpecificDataImple::NT_SpecificDataImple ()
					   : _key(0)
{
    _key = TlsAlloc();

    if (_key != errorValue)
	valid(TRUE);
}

NT_SpecificDataImple::~NT_SpecificDataImple ()
{
    if (valid())
	TlsFree(_key);
}

Boolean NT_SpecificDataImple::setSpecific (void* threadData)
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

Boolean NT_SpecificDataImple::getSpecific (void*& threadData)
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

Boolean NT_SpecificDataImple::removeSpecific ()
{
    if (valid())
    {
	if (TlsSetValue(_key, (LPVOID) 0))
	    return TRUE;
	else
	    removeFromOverflow();
    }

    return FALSE;
}

ThreadDataImple* ThreadDataImple::create ()
{
    return new NT_SpecificDataImple;
}
