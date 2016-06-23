/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SimpleList.cc,v 1.1 1997/09/25 15:28:01 nmcl Exp $
 */

#ifndef SIMPLELIST_CC_
#define SIMPLELIST_CC_

template <class T>
SimpleListEntry<T>::SimpleListEntry ( SimpleListEntry<T> *old, T *th )
				    : next(old),
				      theData(th)
{
}

template <class T>
SimpleListEntry<T>::~SimpleListEntry ()
{
}

template <class T>
SimpleList<T>::SimpleList ( )
			  : head_(0),
			    entryCount(0)
{
}

template <class T>
SimpleList<T>::~SimpleList ()
{
    empty();
}

template <class T>
Boolean SimpleList<T>::lookFor ( T *th )
{
    if (th)
    {
	SimpleListEntry<T> *te = head_;
        
        while (te)
        {
            /* exists already ? */
            
            if (te->theData == th)
            {               
                return TRUE;
            }
                
            te = te->next;
        }
    }
    
    return FALSE;
}

template <class T>
Boolean SimpleList<T>::push ( T *th )
{
    if (th)
    {
	SimpleListEntry<T> *te = new SimpleListEntry<T>(head_, th);
	
	head_ = te;
	entryCount++;
	    
	return TRUE;
    }
    
    return FALSE;
}

template <class T>
Boolean SimpleList<T>::add ( T *th )
{
    return push(th);
}

/*
 * Insert is like add except that it only adds once. i.e., if the entry
 * already exists it returns FALSE;
 */

template <class T>
Boolean SimpleList<T>::insert ( T *th )
{
    if (th)
    {
	SimpleListEntry<T> *te = head_;
	
	while (te)
	{
	    /* exists already ? */
	    
	    if (te->theData == th)
	    {		    
		return FALSE;
	    }
		
	    te = te->next;
	}
	
	/* insert if not found */
	
	te = new SimpleListEntry<T>(head_, th);
	
	head_ = te;
	entryCount++;
	    
	return TRUE;
    }
    
    return FALSE;
}

template <class T>
Boolean SimpleList<T>::remove ( T *findMe )
{
    SimpleListEntry<T> *te = head_;
    SimpleListEntry<T> *old = 0;
	
    while (te)
    {
	if (te->theData == findMe)
	{
	    if (old == 0)
		head_ = te->next;
	    else
		old->next = te->next;
	    
	    delete te;
	    entryCount--;
	    
	    return TRUE;
	}
	
	old = te;
	te = te->next;
    }
    
    return FALSE;
}

template <class T>
T *SimpleList<T>::orderedPop ()
{
    T *th = 0;
	    
    if (head_)
    {
	SimpleListEntry<T> *te = head_;
	    
	head_ = te->next;
	th = te->theData;
	entryCount--;
	    
	delete te;
    }
    
    return th;
}

template <class T>
SimpleListI<T>::SimpleListI ( const SimpleList<T> &list )
			    : nextEnt(list.head_)
{
}

template <class T>
SimpleListI<T>::~SimpleListI ()
{
}

template <class T>
T *SimpleListI<T>::operator() ()
{
    T *th = 0;

    if (nextEnt != 0)
    {
	th = nextEnt->theData;
	nextEnt = nextEnt->next;
    }

    return th;
}

#endif
