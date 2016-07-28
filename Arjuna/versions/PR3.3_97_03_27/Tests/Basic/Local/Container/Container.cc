/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Container.cc,v 1.1 1994/01/20 12:00:55 ngdp Exp $
 */

/*
 *
 * Lock list maintainance class implementation
 *
 */ 

#ifndef CONTAINER_H_
#  include "Container.h"
#endif

#include <Common/Boolean.h>
#include <Common/Uid.h>
#include <Arjuna/AtomicA.h>
#include <Common/Debug.h>

#include "Object.h"

static const char RCSid[] = "$Id: Container.cc,v 1.1 1994/01/20 12:00:55 ngdp Exp $";

/*
 * Public constructors and destructor
 */

Container::Container ()
                   : LockManager(RECOVERABLE,FALSE),
		     head(0),
		     count(0)
{
}

Container::Container ( const Uid& u )
                   : LockManager(u),
		     head(0),
		     count(0)
{
}

Container::~Container ()
{
    Object *temp = head;
    Object *last = head;
    

    while (last != 0)
    {
	last = temp->getLink();
	delete temp;
    }
}

/*
 * Non virtual public functions 
 */

Boolean Container::insert ( Object *newObj )
{
    Boolean ok = TRUE;
    Object *current = head;
    Object *last = 0;
    
    if ((AtomicAction::Current() == 0) ||
	(setlock(new Lock(WRITE)) == GRANTED))
    {
	if (head == 0)
	{
	    push(newObj);
	}
	else
	{
	    while (current != 0)
	    {
		last = current;
		current = current->getLink();
	    }
	    last->setLink(newObj);
	    newObj->setLink(0);
	    count++;
	}
    }
    else
    {
	ok = FALSE;
    }
    
    return ok;
}

/*
 * Pop the first element off the list and return it.
 */

Object *Container::pop ()
{
    Object *current;

    if (count == 0)
	return 0;

    
   if ((AtomicAction::Current() == 0) ||
       (setlock(new Lock(WRITE)) == GRANTED))
   {
	current = (Object *)head;
	count--;
	head = head->getLink();
	current->setLink(0);
    }
    else
    {
	current = 0;
    }
    
    return current;
}

Boolean Container::push ( Object *newObj )
{
    Boolean ok = TRUE;
    
    if ((AtomicAction::Current() == 0) ||
	(setlock(new Lock(WRITE)) == GRANTED))
    {
	newObj->setLink(head);
	head = newObj;
	count++;

    }
    else
    {
	ok = FALSE;
    }    
    
    return ok;
    
}

Boolean Container::save_state ( ObjectState& os, ObjectType ot )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PUBLIC;
    debug_stream << "Container::save_state ()" << endl;
#endif
    Object *current = head;
    
    os.pack(count);
    
    while (current)
    {
	current->get_uid().pack(os);
	current = current->getLink();
    }
    return TRUE;
    
}

Boolean Container::restore_state ( ObjectState& os, ObjectType ot )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_STATE_MAN << VIS_PUBLIC;
    debug_stream << "Container::restore_state ()" << endl;
#endif
    Object *temp;
    Uid u(NIL_UID);
    int dummy;

    while ((temp = pop()) != 0)
    {
	delete temp;
    }
   
    os.unpack(count);
    for (int i = 0; i < count; i++)
    {
	u.unpack(os);
	temp = new Object(u,dummy);
	temp->restore_state(os,ot);
	insert(temp);
    }
    return TRUE;
}


const TypeName Container::type() const
{
    return "/StateManager/LockManager/Container";
}
