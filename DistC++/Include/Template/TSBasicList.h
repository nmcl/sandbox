/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TSBasicList.h,v 1.2 1998/02/06 12:08:16 nmcl Exp $
 */

#ifndef TSBASICLIST_H_
#define TSBASICLIST_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef THREADRWLOCK_H_
#  include <Thread/ThreadRWLock.h>
#endif

#ifndef BASICLIST_H_
#  include <Template/BasicList.h>
#endif

template <class T> class ThreadSafeBasicListI;

template <class T>
class ThreadSafeBasicList<T> : public BasicList<T>
{
    friend class ThreadSafeBasicListI<T>;
    
public:
    ThreadSafeBasicList ();
    ~ThreadSafeBasicList ();
    
    Boolean add (T *);
    Boolean insert (T*);
    Boolean remove (T *);
    T *remove (const Uid&);
    T *lookFor (const Uid&);
    T *orderedPop ();
    size_t size () const;

    void empty ();
    
private:
    ThreadRWLock *lock;
};

/*
 * Put possible inline candidates first
 */

template <class T>
inline void ThreadSafeBasicList<T>::empty ()
{
    if (lock)
	lock->writeLock();
    
    BasicList<T>::empty();
	
    if (lock)
    {
	lock->unlock();
    }
}

template <class T>
class ThreadSafeBasicListI
{
public:
    ThreadSafeBasicListI (const ThreadSafeBasicList<T>&);
    ~ThreadSafeBasicListI();

    T *operator()();

private:
    BasicListI<T> theIter;
    const ThreadSafeBasicList<T> &theList;
    Boolean valid;
};

#ifdef __GNUG__

#ifndef TSBASICLIST_CC_
#  include <Template/TSBasicList.cc>
#endif

#endif

#endif
