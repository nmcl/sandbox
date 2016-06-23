/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TSHashList.cc,v 1.2 1998/02/06 12:08:17 nmcl Exp $
 */

#ifndef TSHASHLIST_CC_
#define TSHASHLIST_CC_

template <class T>
ThreadSafeHashList<T>::ThreadSafeHashList ( size_t elems )
					  : HashList<T>(elems),
					    lock(ThreadRWLock::create())
{
}

template <class T>
ThreadSafeHashList<T>::~ThreadSafeHashList ()
{
    if (lock)
	lock->writeLock();

    /*
     * Cannot call local empty as we will block on our own
     * write lock!
     */
    
    HashList<T>::empty();    
	
    if (lock)
    {
	lock->unlock();
	delete lock;
    }
}

template <class T>
Boolean ThreadSafeHashList<T>::add ( T *th )
{
    if (th && lock)
    {
	if (lock->writeLock())
	{
	    Boolean rVal = HashList<T>::add(th);
	    
	    lock->unlock();
	    return rVal;
	}
    }
    
    return FALSE;
}

/*
 * Insert is like add except that it only adds once. i.e., if the thread
 * already exists it returns FALSE;
 */

template <class T>
Boolean ThreadSafeHashList<T>::insert ( T *th )
{
    if (th && lock)
    {
	if (lock->writeLock())
	{
	    Boolean rVal = HashList<T>::insert(th);
	    
	    lock->unlock();
	    return rVal;
	}
    }
    
    return FALSE;
}

template <class T>
Boolean ThreadSafeHashList<T>::remove ( T *findMe )
{
    if (findMe && lock)
    {
	if (lock->writeLock())
	{
	    Boolean rVal = HashList<T>::remove(findMe);
	    
	    lock->unlock();
	    return rVal;
	}
    }
    
    return FALSE;
}

template <class T>
T *ThreadSafeHashList<T>::remove ( const Uid& findMe )
{
    if (lock)
    {
	if (lock->writeLock())
	{
	    T *rVal = HashList<T>::remove(findMe);
	    
	    lock->unlock();
	    return rVal;
	}
    }
    
    return 0;
}

template <class T>
T *ThreadSafeHashList<T>::lookFor ( const Uid& findMe )
{
    T *rVal = 0;
    
    if (lock && (lock->readLock()))
    {
	rVal = HashList<T>::lookFor(findMe);
	
	lock->unlock();
    }
    
    return rVal;
}

template <class T>
T *ThreadSafeHashList<T>::orderedPop ()
{
    T *th = 0;
	    
    if (lock)
    {
	if (lock->writeLock())
	{
	    th = HashList<T>::orderedPop();
	    
	    lock->unlock();
	}
    }
    
    return th;
}

template <class T>
size_t ThreadSafeHashList<T>::size () const
{
    size_t rv = 0;
    
    if (lock && lock->readLock())
    {
	rv = HashList<T>::size();
	lock->unlock();
    }

    return rv;
}

template <class T>
ThreadSafeHashListI<T>::ThreadSafeHashListI ( const ThreadSafeHashList<T> &list )
					    : theIter(list),
					      theList(list),
					      valid(FALSE)
{
    if (theList.lock && theList.lock->readLock())
	valid = TRUE;  
}

template <class T>
ThreadSafeHashListI<T>::~ThreadSafeHashListI ()
{
    if (valid)
	theList.lock->unlock();
}

template <class T>
T *ThreadSafeHashListI<T>::operator() ()
{
    T *th = 0;

    if (valid)
    {
	th = theIter();
    }

    return th;
}

#endif
