#include <os/string.h>

#ifndef IMPLEMENTATION_PIDFACTORYIMPLELIST_H_
#  include <Implementation/PIDFactoryImpleList.h>
#endif

#ifndef IMPLEMENTATION_PIDFACTORY_INIT_H_
#  include <Implementation/PIDFactory_Init.h>
#endif

FactoryListElement* PIDImpleList::_headOfList = (FactoryListElement*) 0;


class FactoryListElement
{
public:
    FactoryListElement (PIDFactory_Init*);
    ~FactoryListElement ();
    
    PIDFactory_Init*    _imple;
    FactoryListElement* _next;
};

FactoryListElement::FactoryListElement (PIDFactory_Init* p)
			 : _imple(p),
			   _next(0)
{
}

FactoryListElement::~FactoryListElement ()
{
}


PIDImpleList::PIDImpleList ()
{
}

PIDImpleList::~PIDImpleList ()
{
}

/*
 * Could check we are not already added, but better (i.e., quicker) to
 * leave that to the adding caller.
 */

void PIDImpleList::add (PIDFactory_Init* toAdd)
{
    FactoryListElement* ptr = new FactoryListElement(toAdd);

    ptr->_next = _headOfList;
    _headOfList = ptr;
}

void PIDImpleList::remove (PIDFactory_Init* toRemove)
{
}

PIDFactory_Init* PIDImpleList::find (const char* name)
{
    FactoryListElement* ptr = _headOfList;

    while (ptr)
    {
	if (::strcmp(ptr->_imple->pidName(), name) == 0)
	    return ptr->_imple;
	else
	    ptr = ptr->_next;
    }

    return (PIDFactory_Init*) 0;
}
