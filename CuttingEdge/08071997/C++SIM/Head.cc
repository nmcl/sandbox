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
#include "Head.h"
#endif

#ifndef LINK_H_
#include "Link.h"
#endif

#include <iostream.h>


Head::Head ()
            : first(0),
	      last(0)
{
}

Head::~Head () { Clear(); }

void Head::AddFirst (Link* element)
{
    if (!element)
	return;
    
    if (!first)
    {
	if (element->inList)
	    element->Out();
	
	first = element;
	last = element;
	element->inList = true;
    }
    else
    {
	first->Precede(element);
	first = first->Pred();
    }
}

void Head::AddLast (Link* element)
{
    if (last)
    {
	element->Follow(last);
	last = element;
    }
    else
    {
	if (element->inList)
	    element->Out();
	
	element->inList = true;
	first = element;
	last = element;
    }
}

long Head::Cardinal () const 
{
    long numberOfElements = 0;
    Link *tempPtr = first;
    
    while (tempPtr)
    {
	numberOfElements++;
	tempPtr = tempPtr->Suc();
    }
    
    return numberOfElements;
}

void Head::Clear ()
{
    Link *tempPtr = first, *marker = 0;
	
    while (!tempPtr)
    {
	marker = tempPtr;
	tempPtr = tempPtr->Suc();
	delete marker;
    }
}


#ifdef NO_INLINES
#  define HEAD_CC_
#  include "Head.n"
#  undef HEAD_CC_
#endif

