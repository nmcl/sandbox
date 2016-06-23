/*
 * Copyright (C) 1995
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SimpleList.h,v 1.1 1997/09/25 15:28:02 nmcl Exp $
 */

#ifndef SIMPLELIST_H_
#define SIMPLELIST_H_

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

template <class T> class SimpleList;
template <class T> class SimpleListI;

template <class T>
class SimpleListEntry
{
    friend SimpleList<T>;
    friend SimpleListI<T>;
    
public:
    SimpleListEntry (SimpleListEntry<T> *, T *);
    ~SimpleListEntry ();

private:
    SimpleListEntry<T> *next;
    T *theData;
};

template <class T>
class SimpleList
{
    friend SimpleListI<T>;
    
public:
    SimpleList ();
    virtual ~SimpleList ();
    
    Boolean push (T *);
    Boolean add (T *);
    Boolean insert (T *);
    Boolean remove (T *);
    Boolean lookFor (T*);

    T *head ();
    T *orderedPop ();
    size_t size () const;
    void empty ();

private:
    SimpleListEntry<T> *head_;
    size_t entryCount;
};

/*
 * Put possible inline candidates first
 */

template <class T>
inline void SimpleList<T>::empty ()
{
    T *te;

    while ((te = orderedPop()))
    {
    }
}

template <class T>
inline T *SimpleList<T>::head ()
{
    if (head_)
	return head_->theData;
    else
	return 0;
}

template <class T>
inline size_t SimpleList<T>::size () const
{
    return entryCount;
}


template <class T>
class SimpleListI
{
public:
    SimpleListI (const SimpleList<T>&);
    ~SimpleListI ();
    
    T *operator ()();
    
private:
    SimpleListEntry<T> *nextEnt;
};

#ifdef __GNUG__

#ifndef SIMPLELIST_CC_
#  include <Template/SimpleList.cc>
#endif

#endif

#endif
