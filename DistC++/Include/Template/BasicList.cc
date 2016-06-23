/*
 * Copyright (C) 1995, 1996, 1997
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicList.cc,v 1.1 1997/09/25 15:27:53 nmcl Exp $
 */

#ifndef BASICLIST_CC_
#define BASICLIST_CC_

template <class T>
BasicList<T>::BasicList ()
{
}

template <class T>
BasicList<T>::~BasicList ()
{
}

template <class T>
T* BasicList<T>::remove ( const Uid& findMe )
{
    SimpleListI<T> iter(*this);
    T *th = 0;
    
    while (th = iter())
    {
	if (th && th->get_uid() == findMe)
	{
	    SimpleList<T>::remove(th);
	    
	    return th;
	}
    }
    
    return 0;
}

template <class T>
T* BasicList<T>::lookFor ( const Uid& findMe )
{
    SimpleListI<T> iter(*this);
    T *th = 0;
    
    while (th = iter())
    {
	if (th && th->get_uid() == findMe)
	{
	    return th;
	}
    }
    
    return 0;
}

template <class T>
Boolean BasicList<T>::lookFor ( T *findMe )
{
    return SimpleList<T>::lookFor(findMe);
}

template <class T>
Boolean BasicList<T>::remove ( T *findMe )
{
    return SimpleList<T>::remove(findMe);
}

template <class T>
BasicListI<T>::BasicListI ( const BasicList<T> &list )
			  : SimpleListI<T>(list)
{
}

#endif
