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
#include "Link.h"
#endif

#ifndef HEAD_H_
#include "Head.h"
#endif


Link::Link ()
            : prev(0),
	      next(0),
	      inList(false)
{
}

Link::~Link () { RemoveElement(); }

void Link::RemoveElement ()
{
    if (prev)
	prev->next = next;

    if (next)
	next->prev = prev;

    inList = false;
}

Link* Link::Out ()
{
    RemoveElement();
    return this;
}

void Link::InTo (Head* list)
{
    if (list)
    {
	list->AddLast(this);
	return;
    }
    
    (void) Out();
}

void Link::Precede (Link* element)
{
    if (!element)
	(void) Out();
    else
	if (!element->inList)
	    (void) Out();
	else
	{
	    if (inList)
		(void) Out();
	    
	    element->addBefore(this);
	}
}

void Link::Follow (Link* element)
{
    if (!element)
	(void) Out();
    else
	if (!element->inList)
	    (void) Out();
	else
	{
	    if (inList)
		(void) Out();
	    
	    element->addAfter(this);
	}
}

void Link::addAfter (Link* toAdd)
{
    toAdd->inList = true;
    toAdd->prev = this;

    if (!next)
	next = toAdd;
    else
    {
	next->prev = toAdd;
	toAdd->next = next;
	next = toAdd;
    }
}

void Link::addBefore (Link* toAdd)
{
    toAdd->inList = true;
    toAdd->next = this;

    if (!prev)
	prev = toAdd;
    else
    {
	prev->next = toAdd;
	toAdd->prev = prev;
	prev = toAdd;
    }
}

void Link::Follow (Head* list)
{
    if (list)
	list->AddFirst(this);
}


#ifdef NO_INLINES
#  define LINK_CC_
#  include "Link.n"
#  undef LINK_CC_
#endif
