/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ErrorList.cc,v 1.7 1995/10/02 15:14:57 ngdp Exp $
 */

/*
 *
 * Error list maintainance class implementation
 *
 */ 

#ifndef ERRORLIST_H_
#  include <Common/ErrorList.h>
#endif

#ifndef ERRORLISTI_H_
#  include <Common/ErrorListI.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

static const char RCSid[] = "$Id: ErrorList.cc,v 1.7 1995/10/02 15:14:57 ngdp Exp $";

/*
 * Public constructors and destructor
 */

ErrorList::ErrorList ()
		     : count(0),
		       head(0)
{
}

ErrorList::~ErrorList ()
{
    clearList();
}

/*
 * Non virtual public functions 
 */

void ErrorList::clearList ()
{
    
    Error *temp;

    while ((temp = pop()) != 0)
    {
	delete temp;
    }
}

const Error *const ErrorList::findError ( FacilityCode fac ) const
{

    ErrorListI next(*this);
    const Error *current;

    while ((current = next()) != 0)
    {
	if (current->errorFac() == fac)
	    break;
    }

    return current;
}

/*
 * Insert a new Error. This returns TRUE if the insertion occurred, false
 * otherwise. Insertion fails if another error already in the list
 * has a matching facility.
 */

Boolean ErrorList::insert ( Error *newError )
{
    ErrorListI next(*this);
    const Error *current;

    if (newError == 0)
	return FALSE;
    
    while ((current = next()) != 0)
    {
	if (current->errorFac() == newError->errorFac())
	{
	    delete newError;
	    return FALSE;
	}
    }
    push(newError);
    return TRUE;
}

/*
 * Pop the first element off the list and return it.
 */

Error *ErrorList::pop ()
{
    Error *current;

    if (count == 0)
	return 0;

    current = (Error *)head;
    count--;
    head = head->getLink();
    current->setLink(0);
    return current;
}

/*
 *  Push a new element at the head of the list. First set the link
 *  field to be the old head, and then set head to be the new element.
 */

void ErrorList::push ( Error *newError )
{
    if (newError)
    {
	newError->setLink(head);
	head = newError;
	count++;
    }
}

/*
 * Discard the element following the one pointed at. If it is the
 * first element (current = 0) then simply change the head pointer.
 * Beware if current points at the last element or the list is empty!
 * This probably indicates a bug in the caller.
 */

void ErrorList::forgetNext ( Error *current )
{
    if (count > 0)			/* something there to forget */
    {
	if (current == 0)
	    head = head->getLink();
	else
	{
	    Error *nextOne = current->getLink();
	    
	    /* See if at list end */

	    if (nextOne != 0)
		current->setLink(nextOne->getLink());
	    else
	    {
		/*
		 * Probably an error - being asked to forget element
		 * after end of list
		 */
		count++;
		current->setLink(0);	/* force end of list */
	    }

	}
	count--;
    }
}

#ifdef NO_INLINES
#  define ERRORLIST_CC_
#  include <Common/ErrorList.n>
#  undef ERRORLIST_CC_
#endif
