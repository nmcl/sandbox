/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ResourceList.h,v 1.3 1997/07/01 12:09:15 nsmw Exp $
 */

#ifndef TEMPLATE_RESOURCELIST_H_
#define TEMPLATE_RESOURCELIST_H_

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef CORE_RESOURCE_H_
#   include <Core/Resource.h>
#endif

#ifndef TEMPLATE_LISTGEN_H_
#   include <Template/ListGen.h>
#endif

template <class DataType> class ResourceList : public ListGen
{
public:
    ResourceList() { };
    ~ResourceList() { };

    Boolean present(DataType *data) { return ListGen::present(data); };

    void append(DataType *data) { Resource::ref(data);
                                  ListGen::append(data); };
    void prepend(DataType *data) { Resource::ref(data);
                                   ListGen::prepend(data); };

    Boolean remove(DataType *data) { if (ListGen::remove(data))
                                     {
                                         Resource::unref(data);
                                         return TRUE;
                                     }
                                     else
                                         return FALSE;
                                   };
};

template <class DataType> class ResourceListIterator : private ListIteratorGen
{
public:
    ResourceListIterator(ResourceList<DataType> *resourceList) : ListIteratorGen(resourceList) { };
    ~ResourceListIterator() { };

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
