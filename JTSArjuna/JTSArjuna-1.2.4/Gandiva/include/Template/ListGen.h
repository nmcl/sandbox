/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ListGen.h,v 1.4 1998/01/12 12:58:13 nmcl Exp $
 */

#ifndef TEMPLATE_LISTGEN_H_
#define TEMPLATE_LISTGEN_H_

#ifndef COMMON_BOOLEAN_H_
#   include <Common/Boolean.h>
#endif

struct ListElemGen;

class ListGen
{
public:
    friend class ListIteratorGen;

public:
    ListGen();
    ~ListGen();

    Boolean present(void *data);

    void append(void *data);
    void prepend(void *data);

    Boolean remove(void *data);
private:
    ListElemGen *_head;
    ListElemGen *_tail;
};

class ListIteratorGen
{
public:
    ListIteratorGen(ListGen *list);
    ~ListIteratorGen();

    void previous();
    void next();

    void head();
    void tail();

    Boolean swapForward();
    Boolean swapBackward();

    void *content();
    void content(void *data);

private:
    ListGen     *_listGen;
    ListElemGen *_presentElem;
};

#endif
