/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ThreadSpecific.cc,v 1.1 1998/01/12 12:56:14 nmcl Exp $
 */

#ifndef COMMON_THREAD_THREADSPECIFIC_CC_
#define COMMON_THREAD_THREADSPECIFIC_CC_

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

template<class DataType>
ThreadSpecific<DataType>::ThreadSpecific (void (*destructor)(DataType*))
					 : _list(ThreadDataImple::create()),
					   _destructor(destructor)
{
}

template<class DataType>
ThreadSpecific<DataType>::~ThreadSpecific ()
{
    if (_list)
	delete _list;
}

/*
 * It is the programmer's responsibility to destroy any previously
 * held data, otherwise memory leaks will result.
 */

template<class DataType>
Boolean ThreadSpecific<DataType>::setSpecific (DataType* threadData)
{
    return ((_list) ? _list->setSpecific((void*) threadData) : FALSE);
}

template<class DataType>
Boolean ThreadSpecific<DataType>::getSpecific (DataType*& threadData)
{
    if (_list)
    {
	void* ptr = 0;
	if (_list->getSpecific(ptr))
	{
	    if (ptr)
		threadData = (DataType*) ptr;
	    else
		threadData = (DataType*) 0;
	    
	    return TRUE;
	}
    }
    
    return FALSE;
}

/*
 * It is the programmer's responsibility to indicate whether or not
 * the data should be deleted.
 */

template<class DataType>
Boolean ThreadSpecific<DataType>::removeSpecific ()
{
    if (!_list)
	return FALSE;

    DataType* ptr = (DataType*) 0;

    if (getSpecific(ptr))
	return _list->removeSpecific();
    else
	return FALSE;
}

template<class DataType>
Boolean ThreadSpecific<DataType>::deleteSpecific ()
{
    if (!_list)
	return FALSE;

    DataType* ptr = (DataType*) 0;

    if (getSpecific(ptr))
    {
	if (_destructor)
	    (*_destructor)(ptr);
	else
	    delete ptr;  // fallback on default delete
    }
    else
	return FALSE;
    
    return _list->removeSpecific();
}

template<class DataType>
ostream& ThreadSpecific<DataType>::print (ostream& strm) const
{
    strm << "<ThreadSpecific:" << (void*) _list << '>' << endl;
    return strm;
}

template<class DataType>
Boolean ThreadSpecific<DataType>::valid () const
{
    return ((_list) ? _list->valid() : FALSE);
}

#endif
