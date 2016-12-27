/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicListIterator.cc,v 1.5 1997/07/01 14:55:40 nsmw Exp $
 */

#ifndef TEMPLATE_BASICLISTITERATOR_CC_
#define TEMPLATE_BASICLISTITERATOR_CC_

template <class T, class U>
BasicListIterator<T,U>::BasicListIterator (const BasicList<T, U>& L)
					: ptr(L.Head)
{
}

template <class T, class U>
T* BasicListIterator<T,U>::operator ()()
{
    if (ptr)
    {
	ListCons<T>* p = ptr;
	ptr = ptr->cdr();
	
	return p->car();
    }

    return (T*) 0;
}

#endif
