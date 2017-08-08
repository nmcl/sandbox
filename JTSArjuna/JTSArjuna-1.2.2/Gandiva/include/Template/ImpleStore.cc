/*
 * Copyright (C) 1994, 1995,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ImpleStore.cc,v 1.6 1997/07/01 14:55:43 nsmw Exp $
 */

#ifndef TEMPLATE_IMPLESTORE_CC_
#define TEMPLATE_IMPLESTORE_CC_

#ifndef OS_IOSTREAM_H_
#  include <os/iostream.h>
#endif

#ifndef CORE_RESOURCE_H_
#  include <Core/Resource.h>
#endif


template<class T>
ImplementationElement<T>::ImplementationElement (const ObjectName& name, T* mech)
						: _objName(name),
						  _imple(0),
						  _next(0)
{
    _imple = mech;
}

template<class T>
ImplementationElement<T>::~ImplementationElement ()
{
}

template<class T>
ostream& ImplementationElement<T>::print (ostream& strm) const
{
    strm << "****" << endl;
    strm << "Implementation name: " << _objName << endl;
    strm << "Implementation pointer: " << (void*) _imple << endl;
    strm << "****" << endl;

    if (_next)
	return _next->print(strm);
    else
	return strm;
}


template<class T>
ImplementationStore<T>::ImplementationStore ()
					    : headOfList(0)
{
}

template<class T>
ImplementationStore<T>::~ImplementationStore ()
{
    ImplementationElement<T>* ptr = headOfList;
    
    while (ptr)
    {
	headOfList = ptr->_next;
	delete ptr;
	ptr = headOfList;
    }
}

template<class T>
T* ImplementationStore<T>::getImplementation (const ObjectName& name)
{
    if (!headOfList)
	return (T*) 0;
    
    ImplementationElement<T>* ptr = headOfList;

    while (ptr)
    {
	if (ptr->_objName == name)
	    return ptr->_imple;
	else
	    ptr = ptr->_next;
    }
    
    return (T*) 0;
}
	
template<class T>
void ImplementationStore<T>::addImplementation (const ObjectName& name,
						T* mech)
{
    ImplementationElement<T>* ptr = new ImplementationElement<T>(name, mech);
    ptr->_next = headOfList;
    headOfList = ptr;
}

template<class T>
void ImplementationStore<T>::removeImplementation (T* mech)
{
    if (!headOfList)
	return;
    
    ImplementationElement<T>* ptr = headOfList;
    ImplementationElement<T>* trail = ptr;

    while (ptr)
    {
	if (ptr->_imple == mech)
	{
	    if (ptr == headOfList)
		headOfList = headOfList->_next;
	    else
		trail->_next = ptr->_next;

	    delete ptr;
	    return;
	}
	else
	{
	    trail = ptr;
	    ptr = ptr->_next;
	}
    }
}

template<class T>
void ImplementationStore<T>::removeImplementation (const ObjectName& name)
{
    if (!headOfList)
	return;
    
    ImplementationElement<T>* ptr = headOfList;
    ImplementationElement<T>* trail = ptr;

    while (ptr)
    {
	if (ptr->_objName == name)
	{
	    if (ptr == headOfList)
		headOfList = headOfList->_next;
	    else
		trail->_next = ptr->_next;

	    delete ptr;
	    return;
	}
	else
	{
	    trail = ptr;
	    ptr = ptr->_next;
	}
    }
}

template<class T>
Boolean ImplementationStore<T>::empty () const
{
    return (Boolean) (headOfList == 0);
}

template<class T>
ostream& ImplementationStore<T>::print (ostream& strm) const
{
    strm << "Implementation store for " << T::name() << endl;
    
    if (headOfList)
	headOfList->print(strm);
    else
	strm << "List is empty." << endl;
    
    return strm;
}

#endif
