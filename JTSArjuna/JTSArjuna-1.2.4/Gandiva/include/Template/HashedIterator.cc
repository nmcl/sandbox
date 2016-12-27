/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HashedIterator.cc,v 1.4 1997/07/01 14:55:41 nsmw Exp $
 */

#ifndef TEMPLATE_HASHEDITERATOR_CC_
#define TEMPLATE_HASHEDITERATOR_CC_

#ifndef TEMPLATE_HASHEDLIST_H_
#    include <Template/HashedList.h>
#endif

template <class T, class U>
HashedIterator<T, U>::HashedIterator (const HashedList<T, U>& H)
				     : ptr(0),
				       ref(H),
				       _index(0)
{
    if (H.hashedList)
        ptr = new BasicListIterator<T, U>(*ref.hashedList[0]);
}

template <class T, class U>
HashedIterator<T, U>::~HashedIterator ()
{
    if (ptr)
        delete ptr;
}

template <class T, class U>
T* HashedIterator<T, U>::operator ()()
{
    if (ptr)
    {
        T* t = (*ptr)();

	if (!t)
	{
	    _index++;

	    if (_index < ref.listSize)
	    {
		delete ptr;
		const BasicList<T,U>& tmp = *(ref.hashedList[_index]);
		ptr = new BasicListIterator<T,U>(tmp);

		return (*this)();
	    }
	}
	else
	    return t;
    }

    return (T*) 0;
}

#endif
