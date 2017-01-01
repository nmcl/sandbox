/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ListIterator.h,v 1.3 1997/03/11 12:57:35 nsmw Exp $
 */

#ifndef TEMPLATE_LISTITERATOR_H_
#define TEMPLATE_LISTITERATOR_H_

template <class T>
class ListIterator
{
public:
    ListIterator (T* head) { ptr = head; };
    ~ListIterator () {};

    T* current () const { return ptr; };
    T* operator() () { if (ptr) ptr = (T*) (*ptr)(); return ptr; };

private:
    T* ptr;
};

#endif


