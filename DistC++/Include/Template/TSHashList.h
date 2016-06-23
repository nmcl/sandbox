/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TSHashList.h,v 1.1 1997/09/25 15:28:07 nmcl Exp $
 */

#ifndef TSHASHLIST_H_
#define TSHASHLIST_H_

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

#ifndef HASHLIST_H_
#  include <Template/HashList.h>
#endif

template <class T> class ThreadSafeHashListI;

template <class T>
class ThreadSafeHashList : public HashList <T>
{
    friend class ThreadSafeHashListI<T>;
    
public:
    ThreadSafeHashList (size_t);
    virtual ~ThreadSafeHashList ();
    
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
inline void ThreadSafeHashList<T>::empty ()
{
    if (lock)
	lock->writeLock();
    
    HashList<T>::empty();
	
    if (lock)
    {
	lock->unlock();
    }
}


template <class T>
class ThreadSafeHashListI
{
public:
    ThreadSafeHashListI (const ThreadSafeHashList<T>&);
    ~ThreadSafeHashListI();

    T *operator()();

private:
    HashListI<T> theIter;
    const ThreadSafeHashList<T> &theList;
    Boolean valid;
};

#ifdef __GNUG__

#ifndef TSHASHLIST_CC_
#  include <Template/TSHashList.cc>
#endif

#endif

#endif
