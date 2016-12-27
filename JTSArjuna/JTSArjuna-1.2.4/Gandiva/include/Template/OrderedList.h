/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrderedList.h,v 1.3 1998/01/12 12:58:13 nmcl Exp $
 */

#ifndef TEMPLATE_ORDEREDLIST_H_
#define TEMPLATE_ORDEREDLIST_H_

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

template <class DataType> struct OrderedListIterator;

template <class DataType> struct OrderedListElem;

template <class DataType> class OrderedList
{
    friend OrderedListIterator<DataType>;

public:
    OrderedList();
    ~OrderedList();

    Boolean add(DataType *data);
    Boolean remove(DataType *data);

    Boolean present(DataType *data);

private:
    Boolean _add(OrderedListElem<DataType> *&head, OrderedListElem<DataType> *olElem);
    Boolean _remove(OrderedListElem<DataType> *&head, DataType *data);

    Boolean _present(OrderedListElem<DataType> *head, DataType *data);

    OrderedListElem<DataType> *_head;
};

template <class DataType> class OrderedListIterator
{
public:
    OrderedListIterator(OrderedList<DataType> *orderedList);
    ~OrderedListIterator();

    void next();
    void head();

    DataType *content();

private:
    OrderedList<DataType>     *_orderedList;
    OrderedListElem<DataType> *_presentOrderedListElem;
};

#ifndef HAVE_TEMPLATE_REPOSITORY
#   ifndef TEMPLATE_ORDEREDLIST_CC_
#       include <Template/OrderedList.cc>
#   endif
#endif

#endif
