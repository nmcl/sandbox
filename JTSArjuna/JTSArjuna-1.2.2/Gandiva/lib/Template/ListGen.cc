/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ListGen.cc,v 1.4 1998/01/12 13:10:27 nmcl Exp $
 */

#include <os/stddef.h>

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef TEMPLATE_LISTGEN_H_
#   include <Template/ListGen.h>
#endif

struct ListElemGen
{
    void        *_content;
    ListElemGen *_next;
    ListElemGen *_previous;
};

ListGen::ListGen()
{
    _head = NULL;
    _tail = NULL;
}

ListGen::~ListGen()
{
    if (_head != NULL)
    {
        ListElemGen *tempListElemGen1 = _head;
        ListElemGen *tempListElemGen2;

        while ((tempListElemGen1 != NULL))
        {
            tempListElemGen2 = tempListElemGen1->_next;
            delete tempListElemGen1;
            tempListElemGen1 = tempListElemGen2;
        }
    }
}

Boolean ListGen::present(void *cont)
{
    if (cont != NULL)
    {
        Boolean found = FALSE;
        ListElemGen *tempListElemGen = _head;

        while ((tempListElemGen != NULL) && (! found))
            if (tempListElemGen->_content == cont)
                found = TRUE;
            else
                tempListElemGen = tempListElemGen->_next;

        return found;
    }
    else
        return FALSE;
}

void ListGen::append(void *cont)
{
    if (cont != NULL)
    {
        ListElemGen *listElemGen = new ListElemGen;

        listElemGen->_content  = cont;
        listElemGen->_next     = NULL;
        listElemGen->_previous = _tail;

        if (_tail == NULL)
        {
            _head = listElemGen;
            _tail = listElemGen;
        }
        else
        {
            _tail->_next = listElemGen;
            _tail        = listElemGen;
        }
    }
}

void ListGen::prepend(void *cont)
{
    if (cont != NULL)
    {
        ListElemGen *listElemGen = new ListElemGen;

        listElemGen->_content  = cont;
        listElemGen->_next     = _head;
        listElemGen->_previous = NULL;

        if (_head == NULL)
        {
            _head = listElemGen;
            _tail = listElemGen;
        }
        else
        {
            _head->_previous = listElemGen;
            _head            = listElemGen;
        }
    }
}

Boolean ListGen::remove(void *cont)
{
    if (cont != NULL)
    {
        Boolean found = FALSE;
        ListElemGen *tempListElemGen = _head;

        while ((tempListElemGen != NULL) && (! found))
            if (tempListElemGen->_content == cont)
                found = TRUE;
            else
                tempListElemGen = tempListElemGen->_next;

        if (found)
	{
            if ((tempListElemGen->_next == NULL) &&
                (tempListElemGen->_previous == NULL))
	    {
                _head = NULL;
                _tail = NULL;
            }
            else if (tempListElemGen->_next == NULL)
	    {
                tempListElemGen->_previous->_next = NULL;
                _tail = tempListElemGen->_previous;
	    }
            else if (tempListElemGen->_previous == NULL)
	    {
                tempListElemGen->_next->_previous = NULL;
                _head = tempListElemGen->_next;
	    }
            else
	    {
                tempListElemGen->_next->_previous = tempListElemGen->_previous;
                tempListElemGen->_previous->_next = tempListElemGen->_next;
            }

            delete tempListElemGen;
	}
        
        return found;
    }
    else
        return FALSE;
}

ListIteratorGen::ListIteratorGen(ListGen *listGen)
{
    _listGen = listGen;

    if (_listGen != NULL)
        _presentElem = _listGen->_head;
    else
        _presentElem = NULL;
}

ListIteratorGen::~ListIteratorGen()
{
}

void ListIteratorGen::previous()
{
    if (_presentElem != NULL)
        _presentElem = _presentElem->_previous;
}

void ListIteratorGen::next()
{
    if (_presentElem != NULL)
        _presentElem = _presentElem->_next;
}

void ListIteratorGen::head()
{
    if (_listGen != NULL)
        _presentElem = _listGen->_head;
    else
        _presentElem = NULL;
}

void ListIteratorGen::tail()
{
    if (_listGen != NULL)
        _presentElem = _listGen->_tail;
    else
        _presentElem = NULL;
}


Boolean ListIteratorGen::swapForward()
{
    if (_presentElem != NULL)
    {
        if (_presentElem->_previous != NULL)
	{
            void *content;

            content                           = _presentElem->_content;
            _presentElem->_content            = _presentElem->_previous->_content;
            _presentElem->_previous->_content = content;

            _presentElem = _presentElem->_previous;

            return TRUE;
	}    
        else
            return FALSE;
    }
    else
        return FALSE;
}

Boolean ListIteratorGen::swapBackward()
{
    if (_presentElem != NULL)
    {
        if (_presentElem->_next != NULL)
	{
            void *content;

            content                       = _presentElem->_content;
            _presentElem->_content        = _presentElem->_next->_content;
            _presentElem->_next->_content = content;

            _presentElem = _presentElem->_next;

            return TRUE;
	}    
        else
            return FALSE;
    }
    else
        return FALSE;
}

void *ListIteratorGen::content()
{
    if (_presentElem != NULL)
        return _presentElem->_content;
    else
        return NULL;
}

void ListIteratorGen::content(void *cont)
{
    if (_presentElem != NULL)
        _presentElem->_content = cont;
}
