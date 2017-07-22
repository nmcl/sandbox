/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * This class defines the elements of the linked lists within SIMSET.
 */


#ifndef LINK_H_
#define LINK_H_

#ifndef COMMON_H_
#include "common.h"
#endif


class Head;


class Link
{
public:
    virtual ~Link ();
    
    Link* Suc () const;
    Link* Pred () const;

    Link* Out ();
    void InTo (Head*);

    void Precede (Link*);
    void Precede (Head*);
    void Follow (Link*);
    void Follow (Head*);

    boolean inList;
    
protected:
    Link ();           // can only derive from this class

private:
    void RemoveElement ();

    void addAfter (Link*);
    void addBefore (Link*);
    
    Link *prev, *next;
};


#include "Link.n"

#endif
