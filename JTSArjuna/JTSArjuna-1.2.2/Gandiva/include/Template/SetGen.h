/*
 * Copyright (C) 1994, 1995, 1996,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SetGen.h,v 1.3 1998/01/12 12:58:16 nmcl Exp $
 */

#ifndef TEMPLATE_SETGEN_H_
#define TEMPLATE_SETGEN_H_

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

struct SetElemGen;

class SetGen
{
public:
    friend class SetIteratorGen;

public:
    SetGen();
    virtual ~SetGen();

    Boolean present(void *data);

    void add(void *data);

    Boolean remove(void *data);
private:
    SetElemGen *_first;
    SetElemGen *_last;
};

class SetIteratorGen
{
public:
    SetIteratorGen(SetGen *set);
    ~SetIteratorGen();

    void next();

    void start();

    void *content();
    void content(void *data);

private:
    SetGen     *_setGen;
    SetElemGen *_presentElem;
};

#endif
