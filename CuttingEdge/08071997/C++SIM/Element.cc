/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef ELEMENT_H_
#include "Element.h"
#endif

#ifndef SET_H_
#include "Set.h"
#endif


Element::Element ()
                  : value(0)
{
}

Element::Element (long x)
                        : value(x)
{
}

Element::~Element ()
{
}

boolean Element::Belongs (Set* toCheck) const
{
    boolean found = false;
    Element* temp = (Element*) toCheck->First();
    
    while ((temp) && (!found))
    {
	if (temp->GetValue() == value)
	    found = true;
	else
	    temp = (Element*) temp->Suc();
    }
    
    return found;
}


#ifdef NO_INLINES
#  define ELEMENT_CC_
#  include "Element.n"
#  undef ELEMENT_CC_
#endif
