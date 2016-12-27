/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SetGen.cc,v 1.3 1996/02/02 14:35:29 nsmw Exp $
 */

#include <os/stddef.h>
#include <os/stdlib.h>

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef TEMPLATE_SETGEN_H_
#   include <Template/SetGen.h>
#endif

struct SetElemGen
{
    void       *_content;
    SetElemGen *_next;
};

SetGen::SetGen()
{
    _first = NULL;
    _last  = NULL;
}

SetGen::~SetGen()
{
}

Boolean SetGen::present(void *cont)
{
    if (cont != NULL)
    {
        Boolean found = FALSE;

        SetElemGen *tempSetElemGen = _first;

        while ((tempSetElemGen != NULL) && (! found))
            if (tempSetElemGen->_content == cont)
                found = TRUE;
            else
                tempSetElemGen = tempSetElemGen->_next;

        return found;
    }
    else
        return FALSE;
}

void SetGen::add(void *cont)
{
    if (cont != NULL)
    {
        Boolean found = FALSE;

        SetElemGen *tempSetElemGen = _first;

        while ((tempSetElemGen != NULL) && (! found))
            if (tempSetElemGen->_content == cont)
                found = TRUE;
            else
                tempSetElemGen = tempSetElemGen->_next;

        if (! found)
        {
            SetElemGen *setElemGen = new SetElemGen;

            setElemGen->_content  = cont;
            setElemGen->_next     = NULL;

            if (_last == NULL)
            {
                _first = setElemGen;
                _last  = setElemGen;
            }
            else
            {
                _last->_next = setElemGen;
                _last        = setElemGen;
            }
        }
    }
}

Boolean SetGen::remove(void *)
{
    abort();

    return FALSE;
}

SetIteratorGen::SetIteratorGen(SetGen *setGen)
{
    _setGen = setGen;

    if (_setGen != NULL)
        _presentElem = _setGen->_first;
    else
        _presentElem = NULL;
}

SetIteratorGen::~SetIteratorGen()
{
}

void SetIteratorGen::next()
{
    if (_presentElem != NULL)
        _presentElem = _presentElem->_next;
}

void SetIteratorGen::start()
{
    if (_setGen != NULL)
        _presentElem = _setGen->_first;
    else
        _presentElem = NULL;
}

void *SetIteratorGen::content()
{
    if (_presentElem != NULL)
        return _presentElem->_content;
    else
        return NULL;
}

void SetIteratorGen::content(void *cont)
{
    if (_presentElem != NULL)
        _presentElem->_content = cont;
}
