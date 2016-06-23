/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HashList.cc,v 1.1 1997/09/25 15:27:55 nmcl Exp $
 */

#ifndef HASHLIST_CC_
#define HASHLIST_CC_

/*
 * Ctor/dtor
 */

template <class T>
HashList<T>::HashList ( size_t hashBucket )
		      : maxBucket(hashBucket)
{
    buckets = new BasicList<T> *[hashBucket];
    
    for (int i = 0; i < hashBucket; i++)
	buckets[i] = 0;
}

template <class T>
HashList<T>::~HashList ()
{
    empty();
    
    delete [] buckets;
}

/*
 * Public
 */

template <class T>
Boolean HashList<T>::add ( T *th )
{
    if (th)
    {
	unsigned long entry = th->get_uid().hash() % maxBucket;

	if (!buckets[entry])
	    buckets[entry] = new BasicList<T>;

	return buckets[entry]->add(th);
    }
    
    return FALSE;
}

/*
 * Insert is like add execpt that it only adds once. i.e. if the entry
 * already exists it returns FALSE;
 */

template <class T>
Boolean HashList<T>::insert ( T *th )
{
    if (th)
    {
	unsigned long entry = th->get_uid().hash() % maxBucket;

	if (buckets[entry])
	    return buckets[entry]->insert(th);
	else
	{
	    buckets[entry] = new BasicList<T>;
	    return buckets[entry]->add(th);
	}
    }
    
    return FALSE;
}

template <class T>
Boolean HashList<T>::remove ( T *findMe )
{
    if (findMe)
    {
	unsigned long entry = findMe->get_uid().hash() % maxBucket;

	if (buckets[entry])
	    return buckets[entry]->remove(findMe);
    }
    
    return FALSE;
}

template <class T>
T *HashList<T>::remove ( const Uid& findMe )
{
    unsigned long entry = findMe.hash() % maxBucket;
    
    if (buckets[entry])
	return buckets[entry]->remove(findMe);

    return 0;
}

template <class T>
T *HashList<T>::lookFor ( const Uid& findMe )
{
    unsigned long entry = findMe.hash() % maxBucket;
	
    if (buckets[entry])
	return buckets[entry]->lookFor(findMe);

    return 0;
}

template <class T>
T *HashList<T>::orderedPop ()
{
    T *th = 0;
    
    for (int i = 0; (i < maxBucket) && (!th); i++)
    {
	if (buckets[i] && buckets[i]->size() > 0)
	{
	    th = buckets[i]->orderedPop();
	}
    }
    
    return th;
}

template <class T>
HashListI<T>::HashListI ( const HashList<T> &list )
			: theList(list),
			  iter(0),
			  lastBucket(-1)
{
}

template <class T>
HashListI<T>::~HashListI ()
{
    if (iter)
	delete iter;
}

template <class T>
T *HashListI<T>::operator() ()
{
    T* th = 0;
    
    /*
     * Assume that receiving a null from the current iterator means
     * we have come to the end of that list, so move on to next.
     */

    if (iter)
	th = (*iter)();

    if (th)
	return th;
    else
	iter = 0;

    lastBucket++;  // move on to next bucket!
    
    while ((lastBucket < theList.maxBucket) &&
	   ((theList.buckets[lastBucket] == 0) ||
	    (theList.buckets[lastBucket]->size() == 0)))
	lastBucket++;

    if (lastBucket < theList.maxBucket)
    {
	if (iter != 0)
	    delete iter;
	
	iter = new BasicListI<T>(*theList.buckets[lastBucket]);
    }

    if (iter != 0)
	th = (*iter)();
    
    return th;
}

#endif
