/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrderedList.cc,v 1.2 1996/01/02 13:13:10 nsmw Exp $
 */

#ifndef TEMPLATE_ORDEREDLIST_CC_
#define TEMPLATE_ORDEREDLIST_CC_

#ifndef OS_STDDEF_H_
#   include <os/stddef.h>
#endif

#ifndef OS_IOSTREAM_H_
#   include <os/iostream.h>
#endif

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef TEMPLATE_ORDEREDLIST_H_
#   include <Template/OrderedList.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

template <class DataType> struct OrderedListElem
{
    OrderedListElem(DataType *data);
    ~OrderedListElem();

    DataType        *_data;
    OrderedListElem *_next;
};

template <class DataType> OrderedListElem<DataType>::OrderedListElem(DataType *data)
{
    _data = data;
    _next = NULL;

    Resource::ref(_data);
}

template <class DataType> OrderedListElem<DataType>::~OrderedListElem()
{
    Resource::unref(_data);
}

template <class DataType> OrderedList<DataType>::OrderedList()
{
    _head = NULL;
}

template <class DataType> OrderedList<DataType>::~OrderedList()
{
    OrderedListElem<DataType> *temp;

    while (_head != NULL)
    {
        temp = _head->_next;
        delete _head;
        _head = temp;
    }
}

template <class DataType> Boolean OrderedList<DataType>::add(DataType *data)
{
    OrderedListElem<DataType> *olElem = new OrderedListElem<DataType>(data);

    return _add(_head, olElem);
}

template <class DataType> Boolean OrderedList<DataType>::remove(DataType *data)
{
    if (_head == NULL)
        return FALSE;
    else
        return _remove(_head, data);
}

template <class DataType> Boolean OrderedList<DataType>::present(DataType *data)
{
    if (_head == NULL)
        return FALSE;
    else
        return _present(_head, data);
}

template <class DataType> Boolean OrderedList<DataType>::_add(OrderedListElem<DataType> *&head, OrderedListElem<DataType> *olElem)
{
    if (head == NULL)
    {
        head = olElem;
        return TRUE;
    }
    else
    {
        if (*head->_data != *olElem->_data)
	{
            if (*head->_data > *olElem->_data)
            {
                olElem->_next     = head;
                head              = olElem;

                return TRUE;
            }
            else
                return _add(head->_next, olElem);
	}
        else
	{
            delete olElem;
            return FALSE;
	}
    }
}

template <class DataType> Boolean OrderedList<DataType>::_remove(OrderedListElem<DataType> *&head, DataType *data)
{
    if (head == NULL)
        return FALSE;
    else
    {
        if (*head->_data == *data)
	{
            OrderedListElem<DataType> *temp = head;

            head = head->_next;

            delete temp;

            return TRUE;
	}
        else
	{
            if (*head->_data > *data)
                return FALSE;
            else
                return _remove(head->_next, data);
	}
    }
}

template <class DataType> Boolean OrderedList<DataType>::_present(OrderedListElem<DataType> *head, DataType *data)
{
    if (head == NULL)
        return FALSE;
    else
    {
        if (*head->_data == *data)
            return TRUE;
        else
	{
            if (*head->_data > *data)
                return FALSE;
            else
                return _present(head->_next, data);
	}
    }
}

template <class DataType> OrderedListIterator<DataType>::OrderedListIterator(OrderedList<DataType> *orderedList)
{
    _orderedList = orderedList;

    if (_orderedList != NULL)
        _presentOrderedListElem = _orderedList->_head;
    else
        _presentOrderedListElem = NULL;
}

template <class DataType> OrderedListIterator<DataType>::~OrderedListIterator()
{
}

template <class DataType> void OrderedListIterator<DataType>::next()
{
    if (_presentOrderedListElem != NULL)
        _presentOrderedListElem = _presentOrderedListElem->_next;
}

template <class DataType> void OrderedListIterator<DataType>::head()
{
    if (_orderedList != NULL)
        _presentOrderedListElem = _orderedList->_head;
    else
        _presentOrderedListElem = NULL;
}

template <class DataType> DataType *OrderedListIterator<DataType>::content()
{
    if (_presentOrderedListElem != NULL)
        return _presentOrderedListElem->_data;
    else
        return NULL;
}

#endif
