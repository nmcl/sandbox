/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadList.h,v 1.1 1997/06/09 19:52:04 nmcl Exp $
 */

#ifndef THREADLIST_H_
#define THREADLIST_H_

#ifdef STUB
#pragma @NoRename
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

class ThreadID;
class Thread;
class AtomicAction;
class ThreadMutex;
class ThreadListEntry;

static const int LISTMAX = 37;

// @NoRemote, @NoMarshall
class ThreadListEntry
{
    friend class ThreadList;
    friend class ThreadListI;
    
public:
    ThreadListEntry (ThreadListEntry *, Thread *);
    ~ThreadListEntry ();

private:
    ThreadListEntry *next;
    Thread *theThread;
};

// @NoRemote, @NoMarshall
class ThreadList
{
    friend class ThreadListI;
    
public:
    ThreadList ();
    ~ThreadList ();
    
    Boolean add (Thread *);
    Boolean insert (Thread*);
    Boolean remove (Thread *);
    Thread *lookFor (const ThreadID&);
    Thread *orderedPop ();
    size_t size () const;
    
private:
    ThreadListEntry *head[LISTMAX];
    ThreadMutex *lock;
    size_t entryCount;
};

inline size_t ThreadList::size () const { return entryCount; }

#endif
