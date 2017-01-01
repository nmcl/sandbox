/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: List.h,v 1.4 1997/03/20 09:45:01 nmcl Exp $
 */

#ifndef TEMPLATE_LIST_H_
#define TEMPLATE_LIST_H_

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef TEMPLATE_LISTGEN_H_
#   include <Template/ListGen.h>
#endif

template <class DataType> class List : public ListGen
{
public:
    List() { };
    ~List() { };

    Boolean present(DataType *data) { return ListGen::present(data); };

    void append(DataType *data) { ListGen::append(data); };
    void prepend(DataType *data) { ListGen::prepend(data); };

    Boolean remove(DataType *data) { return ListGen::remove(data); };
};

template <class DataType> class ListIterator : private ListIteratorGen
{
public:
    ListIterator(List<DataType> *list) : ListIteratorGen(list) { };
    ~ListIterator() { };

    void previous() { ListIteratorGen::previous(); };
    void next() { ListIteratorGen::next(); };

    void head() { ListIteratorGen::head(); };
    void tail() { ListIteratorGen::tail(); };

    Boolean swapForward() { return ListIteratorGen::swapForward(); };
    Boolean swapBackward() { return ListIteratorGen::swapBackward(); };

    DataType *content() { return (DataType*) ListIteratorGen::content(); };
    void content(DataType *data) { ListIteratorGen::content(data); };
};

#endif
