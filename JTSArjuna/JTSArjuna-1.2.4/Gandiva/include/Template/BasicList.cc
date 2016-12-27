/*
 * Copyright (C) 1994, 1995, 1996, 1997
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicList.cc,v 1.7 1998/01/19 12:37:30 nmcl Exp $
 */

#ifndef TEMPLATE_BASICLIST_CC_
#define TEMPLATE_BASICLIST_CC_

#ifndef TEMPLATE_BASICLISTITERATOR_H_
#  include <Template/BasicListIterator.h>
#endif

template <class T>
ListCons<T>::ListCons (T* p, ListCons<T>* n)
		      : content(p),
			next(n)
{
}

template <class T>
ListCons<T>::~ListCons ()
{
}

template <class T>
T* ListCons<T>::car ()
{
    return content;
}

template <class T>
ListCons<T>* ListCons<T>::cdr ()
{
    return next;
}

template <class T>
void ListCons<T>::SetfCdr (ListCons<T>* n)
{
    next = n;
}


template <class T, class U>
BasicList<T, U>::BasicList ()
			   : Head(0)
{
}

template <class T, class U>
BasicList<T, U>::~BasicList ()
{
    if (Head)
	delete Head;
}

template <class T, class U>
void BasicList<T, U>::insert (T* p)
{
    Head = new ListCons<T>(p, Head);
}

template <class T, class U>
Boolean BasicList<T, U>::remove (T* element)
{
    // Take care of boundary condition - empty list
    
    if ((!Head) || (!element))
	return FALSE;

    for (ListCons<T>* prev = 0, *ptr = Head; ptr; prev = ptr, ptr = ptr->cdr())
    {
	if (ptr->car() == element)
	{
	    ListCons<T>* oldcons = ptr;
	    
	    // unlink the cons cell for the element we're removing
	    if (prev)
		prev->SetfCdr(ptr->cdr());
	    else
		Head = ptr->cdr();
	    
	    // flush the dead cons cell
	    delete oldcons;
	    
            return TRUE;
	}
    }

    return FALSE;
}

template <class T, class U>
T* BasicList<T, U>::remove (const U& id)
{
    // Take care of boundary condition - empty list
    if (!Head)
	return (T*) 0;

    for (ListCons<T>* prev = 0, *ptr = Head; ptr; prev = ptr, ptr = ptr->cdr())
    {
	if (ptr->car()->uid() == id)
	{
	    ListCons<T>* oldcons = ptr;
	    T* result = ptr->car();
	    
	    // unlink the cons cell for the element we're removing
	    if (prev)
		prev->SetfCdr(ptr->cdr());
	    else
		Head = ptr->cdr();
	    
	    // flush the dead cons cell
	    delete oldcons;
	    
            return result;
	}
    }

    return (T*) 0;
}

template <class T, class U>
T* BasicList<T, U>::find (const U& id) const
{
    // Take care of boundary condition - empty list
    if (!Head)
	return (T*) 0;

    for (ListCons<T>* prev = 0, *ptr = Head; ptr; prev = ptr, ptr = ptr->cdr())
    {
	if (ptr->car()->uid() == id)
	    return ptr->car();
    }

    return (T*) 0;
}

template <class T, class U>
ostream& BasicList<T, U>::print (ostream& strm) const
{
    BasicListIterator<T, U> iter(*this);
    int count = 1;

    for (T* p = iter(); p; p = iter(), count++)
	strm << "Thread number\t" << count << "\twith uid\t"
	     << p->uid() << "\n";
    
    return strm;
}

template <class T, class U>
ListCons<T>* BasicList<T, U>::operator[] (int index)
{
    ListCons<T>* ptr = Head;
    for (int i = 0; i < index; i++)
        ptr = ptr->cdr();

    return ptr;
}

#endif
