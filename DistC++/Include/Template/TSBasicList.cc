/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TSBasicList.cc,v 1.1 1997/09/25 15:28:03 nmcl Exp $
 */

#ifndef TSBASICLIST_CC_
#define TSBASICLIST_CC_

template <class T>
ThreadSafeBasicList<T>::ThreadSafeBasicList ( )
					    : BasicList<T>(),
					      lock(ThreadRWLock::create())
{
}

template <class T>
ThreadSafeBasicList<T>::~ThreadSafeBasicList ()
{
    if (lock)
	lock->writeLock();

    /*
     * Cannot call local empty as we will block on our own
     * write lock!
     */
    
    BasicList<T>::empty();    
	
    if (lock)
    {
	lock->unlock();
	delete lock;
    }
}

template <class T>
Boolean ThreadSafeBasicList<T>::add ( T *th )
{
    if (th && lock)
    {
	if (lock->writeLock())
	{
	    Boolean rVal = BasicList<T>::add(th);
	    
	    lock->unlock();
	    return rVal;
	}
    }
    
    return FALSE;
}

/*
 * Insert is like add execpt that it only adds once. i.e. if the thread
 * already exists it returns FALSE;
 */

template <class T>
Boolean ThreadSafeBasicList<T>::insert ( T *th )
{
    if (th && lock)
    {
	if (lock->writeLock())
	{
	    Boolean rVal = BasicList<T>::insert(th);
	    
	    lock->unlock();
	    return rVal;
	}
    }
    
    return FALSE;
}

template <class T>
Boolean ThreadSafeBasicList<T>::remove ( T *findMe )
{
    if (findMe && lock)
    {
	if (lock->writeLock())
	{
	    Boolean rVal = BasicList<T>::remove(findMe);
	    
	    lock->unlock();
	    return rVal;
	}
    }
    
    return FALSE;
}

template <class T>
T *ThreadSafeBasicList<T>::remove ( const Uid& findMe )
{
    if (lock)
    {
	if (lock->writeLock())
	{
	    T *rVal = BasicList<T>::remove(findMe);
	    
	    lock->unlock();
	    return rVal;
	}
    }
    
    return 0;
}

template <class T>
T *ThreadSafeBasicList<T>::lookFor ( const Uid& findMe )
{
    T *rVal = 0;
    
    if (lock && (lock->readLock()))
    {
	rVal = BasicList<T>::lookFor(findMe);
	
	lock->unlock();
    }
    
    return rVal;
}

template <class T>
T *ThreadSafeBasicList<T>::orderedPop ()
{
    T *th = 0;
	    
    if (lock)
    {
	if (lock->writeLock())
	{
	    th = BasicList<T>::orderedPop();
	    
	    lock->unlock();
	}
    }
    
    return th;
}

template <class T>
size_t ThreadSafeBasicList<T>::size () const
{
    size_t rv = 0;
    
    if (lock && lock->readLock())
    {
	rv = BasicList<T>::size();
	lock->unlock();
    }

    return rv;
}

template <class T>
ThreadSafeBasicListI<T>::ThreadSafeBasicListI ( const ThreadSafeBasicList<T> &list )
					      : theIter(list),
						theList(list),
						valid(FALSE)
{
    if (theList.lock && theList.lock->readLock())
	valid = TRUE;
}

template <class T>
ThreadSafeBasicListI<T>::~ThreadSafeBasicListI ()
{
    if (valid)
	theList.lock->unlock();
}

template <class T>
T *ThreadSafeBasicListI<T>::operator() ()
{
    T *th = 0;

    if (valid)
    {
	th = theIter();
    }
    
    return th;
}

#endif
