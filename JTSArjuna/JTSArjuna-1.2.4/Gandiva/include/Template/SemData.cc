/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SemData.cc,v 1.3 1998/01/12 12:58:15 nmcl Exp $
 */

#ifndef TEMPLATE_SEMDATA_CC_
#define TEMPLATE_SEMDATA_CC_

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef TEMPLATE_SEMDATA_H_
#  include <Template/SemData.h>
#endif

template<class T>
SemData<T>::SemData (array_t size)
		    : _dead(FALSE),
		      _numberAllowed(size),
		      _useCount(size),
		      _currentOwners(size)
{
    for (array_t i = 0; i < _numberAllowed; i++)
    {
	_useCount[i] = 0;
	_currentOwners[i] = 0;
    }
}

template<class T>
SemData<T>::~SemData ()
{
}

/*
 * In case the default isOwner is not sufficient.
 */

template<class T>
T& SemData<T>::operator[] (array_t i)
{
    return _currentOwners[i];
}

template<class T>
T& SemData<T>::operator[] (array_t i) const
{
    return _currentOwners[i];
}

template<class T>
Boolean SemData<T>::isOwner (const T& me, array_t& index) const
{
    for (index = 0; index < _numberAllowed; index++)
    {
        if ((_currentOwners[index]) && (_currentOwners[index] == me))
	    return TRUE;
    }

    return FALSE;
}

template<class T>
Boolean SemData<T>::addOwner (const T& me, array_t& index)
{
    for (index = 0; index < _numberAllowed; index++)
    {
        if (_useCount[index] == 0)
	{
	    _currentOwners[index] = me;
	    return TRUE;
	}
    }

    return FALSE;
}

template<class T>
Boolean SemData<T>::increment (array_t index)
{
    _useCount[index] = _useCount[index] +1;

    return TRUE;
}

template<class T>
Boolean SemData<T>::decrement (array_t index)
{
    _useCount[index] = _useCount[index] -1;

    return TRUE;
}

template<class T>
Boolean SemData<T>::full () const
{
    if (totalCount() == size())
	return TRUE;
    else
	return FALSE;
}

template<class T>
int SemData<T>::count (array_t index) const
{
    return _useCount[index];
}

template<class T>
int SemData<T>::totalCount () const
{
    int c = 0;
    
    for (array_t i = 0; i < _useCount.getSize(); i++)
    {
	if (_useCount[i] > 0)
	    c++;
    }

    return c;
}

template<class T>
void SemData<T>::reset (array_t index)
{
    _useCount[index] = 0;
    _currentOwners[index] = 0;
}

template<class T>
array_t SemData<T>::size () const
{
    return _useCount.getSize();
}

template<class T>
Boolean SemData<T>::dead () const
{
    return _dead;
}

template<class T>
void SemData<T>::dead (Boolean d)
{
    _dead = d;
}

#endif
