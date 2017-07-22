/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef PROCESSLIST_H_
#include "ProcessList.h"
#endif


ProcessList::ProcessList () { Head = 0; }

void ProcessList::Insert (Process &p, boolean prior)
{
    // If list is empty, insert at head
    if (!Head)
    {
	Head = new ProcessCons(p, Head);
	return;
    }

    // Try to insert before (if there is anything scheduled later)
    ProcessIterator iter(*this);

    for (Process *prev = 0, *q = iter(); q; prev = q, q = iter())
    {
	if (prior)
	{
	    if (q->evtime() >= p.evtime())
	    {
		(void) InsertBefore(p, *q);
		return;
	    }
	}
	else
	    if (q->evtime() > p.evtime())
	    {
		(void) InsertBefore(p, *q);
		return;
	    }
    }

    // Got to insert at the end (currently pointed at by 'prev'
    (void) InsertAfter(p, *prev);
}

boolean ProcessList::InsertBefore (Process &ToInsert, Process &Before)
{
    for (ProcessCons *prev=0, *p=Head; p; prev=p, p=p->cdr())
	if (p->car() == &Before) {
	    ProcessCons *newcons = new ProcessCons(ToInsert, p);
	    if (prev)
		prev->SetfCdr(newcons);
	    else 
		Head = newcons;

	    return true;
	}
    return false;
}

boolean ProcessList::InsertAfter (Process &ToInsert, Process &After)
{
    for (ProcessCons *p = Head; p; p = p->cdr())
	if (p->car() == &After) {
	    ProcessCons *newcons = new ProcessCons(ToInsert, p->cdr());
	    p->SetfCdr(newcons);
	    return true;
	}
    return false;
}

Process *ProcessList::Remove (const Process *element)
{
    Process *p = 0;

    // Take care of boundary condition - empty list
    if (!Head) return 0;

    // Change unspecified element to "remove head of list" request
    if (element == 0)
	return(Remove(Head->car()));

    for (ProcessCons *prev = 0, *ptr = Head; ptr; prev = ptr, ptr = ptr->cdr())
    {
	if (ptr->car() == element)
	{
	    ProcessCons *oldcons = ptr;
	    // unlink the cons cell for the element we're removing
	    if (prev)
		prev->SetfCdr(ptr->cdr());
	    else
		Head = ptr->cdr();
	    // return the pointer to the process
	    p = ptr->car();
	    // flush the dead cons cell
	    delete oldcons;
	}
    }

    return p;
}
