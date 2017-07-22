/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * This class essentially defines the linked list manager used by the SIMSET
 * class in SIMULA.
 */


#ifndef HEAD_H_
#define HEAD_H_

#ifndef COMMON_H_
#include "common.h"
#endif


class Link;


class Head
{
public:
    Head ();
    virtual ~Head ();
    
    Link* First () const;
    Link* Last () const;
    
    void AddFirst (Link*);
    void AddLast (Link*);

    long Cardinal () const;
    boolean Empty () const;

    void Clear ();
    
private:
    Link *first, *last;
};


#include "Head.n"

#endif
