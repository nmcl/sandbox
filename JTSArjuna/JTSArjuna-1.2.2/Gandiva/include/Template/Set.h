/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Set.h,v 1.2 1996/01/02 13:13:12 nsmw Exp $
 */

#ifndef TEMPLATE_SET_H_
#define TEMPLATE_SET_H_

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

#ifndef TEMPLATE_SETGEN_H_
#   include <Template/SetGen.h>
#endif

template <class DataType> class Set : public SetGen
{
public:
    Set() { };
    ~Set() { };

    Boolean present(DataType *data) { return SetGen::present(data); };

    void    add(DataType *data) { SetGen::add(data); };

    Boolean remove(DataType *data) { return SetGen::remove(data); };

    static  Set<DataType> *unionSet (Set<DataType> *set1, Set<DataType> *set2);
    static  Set<DataType> *intersection (Set<DataType> *set1, Set<DataType> *set2);
};

template <class DataType> class SetIterator : private SetIteratorGen
{
public:
    SetIterator(Set<DataType> *set) : SetIteratorGen(set) { };
    ~SetIterator() { };

    void next() { SetIteratorGen::next(); };

    void start() { SetIteratorGen::start(); };

    DataType *content() { return (DataType*) SetIteratorGen::content(); };
    void content(DataType *data) { SetIteratorGen::content(data); };
};

#endif
