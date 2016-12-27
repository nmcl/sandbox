/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Array.cc,v 1.8 1997/07/01 14:55:38 nsmw Exp $
 */

#ifndef TEMPLATE_ARRAY_CC_
#define TEMPLATE_ARRAY_CC_

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

template<class T>
Array<T>::Array (array_t size)
		: _size(size)
{
    _arrayBase = new T[_size];
}

template<class T>
Array<T>::Array (const Array<T>& toCopy)
{
    *this = toCopy;
}

template<class T>
Array<T>::~Array ()
{
    delete [] _arrayBase;
}

template<class T>
T& Array<T>::operator[] (array_t i)
{
    return _arrayBase[i];
}

template<class T>
T& Array<T>::operator[] (array_t i) const
{
    return _arrayBase[i];
}

template<class T>
array_t Array<T>::getSize () const
{
    return _size;
}

template<class T>
Boolean Array<T>::setSize (array_t n)
{
    if (n == _size)
	return TRUE;
    
    if (n > 0)
    {
        delete _arrayBase;
	_size = n;
	_arrayBase = new T[_size];

	return TRUE;
    }
    else
	return FALSE;
}

/*
 * If resizing downwards the user is responsible
 * for deleting the contents of the elements.
 */

template<class T>
Boolean Array<T>::resize (array_t n)
{
    if (n == _size)
	return TRUE;
    
    T* _tempBase = new T[n];
    array_t s = ((n > _size) ? _size : n);
    
    for (array_t i = 0; i < s; i++)
	_tempBase[i] = _arrayBase[i];

    T* toDelete = _arrayBase;
	
    _arrayBase = _tempBase;
    _size = n;
    delete [] toDelete;

    return TRUE;
}

template<class T>
Array<T>& Array<T>::operator= (const Array<T>& toCopy)
{
    if (this == &toCopy)
        return *this;

    setSize(toCopy._size);

    T* p = _arrayBase + _size;
    T* q = toCopy._arrayBase + _size;

    while (p > _arrayBase) *--p = *--q;

    return *this;
}

template<class T>
Array<T>& Array<T>::operator= (T toCopy)
{
    T* p = _arrayBase + _size;

    while (p > _arrayBase) *--p = toCopy;

    return *this;
}

template<class T>
ostream& Array<T>::print (ostream& strm) const
{
    for (array_t i = 0; i < _size; i++)
	strm << "[ " << i << ", " << _arrayBase[i] << " ]\n";

    return strm;
}

#endif
