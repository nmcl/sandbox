/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NTMutex.h,v 1.1 1998/09/16 11:03:28 nmcl Exp $
 */

#ifndef NTMUTEX_H_
#define NTMUTEX_H_

#ifndef WINDOWS_H_
#  include <System/windows.h>
#endif

#ifndef THREADMUTEX_H_
#  include <Thread/ThreadMutex.h>
#endif

class NTMutex : public ThreadMutex
{
    friend ThreadMutex;

public:
    virtual ~NTMutex ();
    
    virtual Boolean lock ();
    virtual Boolean unlock ();
    virtual Boolean tryLock ();

private:
    NTMutex (Boolean);
    
    HANDLE mutex;
    DWORD owner;
    int count;
    Boolean recursive;
    Boolean valid;
};


#endif
