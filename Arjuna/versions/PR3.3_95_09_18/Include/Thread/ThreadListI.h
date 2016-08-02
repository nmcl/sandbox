/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadListI.h,v 1.1 1995/06/26 10:39:00 ngdp Exp $
 */

#ifndef THREADLISTI_H_
#define THREADLISTI_H_

#ifdef STUB
#pragma @NoRename
#endif

class Thread;
class ThreadListEntry;

// @NoRemote, @NoMarshall
class ThreadListI
{
public:
    ThreadListI (ThreadList *);
    ~ThreadListI ();
    
    Thread *operator ()();
    
private:
    ThreadListEntry *nextEnt;
    ThreadList *theList;
    int lastBucket;
};

#endif
