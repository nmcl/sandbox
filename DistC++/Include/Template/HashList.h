/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HashList.h,v 1.1 1997/09/25 15:27:56 nmcl Exp $
 */

#ifndef HASHLIST_H_
#define HASHLIST_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef BASICLIST_H_
#  include <Template/BasicList.h>
#endif

template <class T> class HashListI;

template <class T>
class HashList
{
    friend class HashListI<T>;
    
public:
    HashList (size_t);
    virtual ~HashList ();
    
    Boolean add (T *);
    Boolean insert (T *);
    Boolean remove (T *);
    T *remove (const Uid&);
    T *lookFor (const Uid&);
    T *orderedPop ();
    size_t size () const;

    void empty ();
    
private:
    BasicList<T> **buckets;

    size_t maxBucket;
};

/*
 * Put possible inline candidates first
 */

template <class T>
inline void HashList<T>::empty ()
{
    for (int i = 0; i < maxBucket; i++)
	if (buckets[i] != 0)
	{
	    delete buckets[i];
	    buckets[i] = 0;
	}
}

template <class T>
inline size_t HashList<T>::size () const
{
    size_t rVal = 0;
    
    for (int i = 0; i < maxBucket; i++)
    {
	if (buckets[i])
	{
	    rVal += buckets[i]->size();
	}
    }
    
    return rVal;
}


template <class T>
class HashListI
{
public:
    HashListI (const HashList<T>&);
    ~HashListI ();
    
    T *operator ()();
    
private:
    const HashList<T> &theList;
    BasicListI<T> *iter;
    
    int lastBucket;
};

#ifdef __GNUG__

#ifndef HASHLIST_CC_
#  include <Template/HashList.cc>
#endif

#endif

#endif
