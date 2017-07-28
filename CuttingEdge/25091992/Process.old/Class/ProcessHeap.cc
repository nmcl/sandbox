#include "ProcessHeap.h"

ProcessHeap::ProcessHeap() { Head = 0; }

ProcessHeap::~ProcessHeap() {}

Element* ProcessHeap::ScanOnTime(Process& Find, Element* From)
{
    Element* wanted;

    if (From == 0)
	return 0;

    if (Find.evtime() <= From->Proc->evtime())
	wanted = ScanOnTime(Find, From->lson);
    else
	wanted = ScanOnTime(Find, From->rson);

    if (wanted == 0)
	return From;
    else
	return wanted;
}

Element* ProcessHeap::ScanOnProcess(Process& Find, Element* From)
{
    Element* wanted;

    if (From == 0)
	return 0;

    if (From->Proc == &Find)
	return From;

    if (From->lson != 0)
	wanted = ScanOnProcess(Find, From->lson);
    else
	wanted = ScanOnProcess(Find, From->rson);

    if (wanted != 0)
	return From;
    else
	return 0;
}

void ProcessHeap::Insert(Process &p, boolean prior)
{
    if (!Head)
    {
	Head = new Element(&p);
	return;
    }

    Element* wanted, *temp;
    wanted = ScanOnTime(p, Head);

    if (prior)
    {
	if (wanted->Proc->evtime() == p.evtime())
	{
	    temp = wanted->lson;
	    wanted->lson = new Element(wanted->Proc);
	    wanted->lson->lson = temp;
	    wanted->Proc = &p;
	    return;
	}
    }

    if (wanted->Proc->evtime() < p.evtime())
	wanted->lson = new Element(&p);
    else
	wanted->rson = new Element(&p);
}

boolean ProcessHeap::InsertBefore(Process& ToInsert, Process& Before)
{
    Element* temp, *wanted;

    if (Head->Proc == &Before)
    {
	temp = Head;
	Head = new Element(&ToInsert);
	if (Head->Proc->evtime() <= temp->Proc->evtime())
	    Head->lson = temp;
	else
	    Head->rson = temp;

	return true;
    }

    wanted = ScanOnProcess(Before, Head);
    if (wanted == 0) return false;
	
    if (wanted->lson->Proc == &Before)
    {
	temp = wanted->lson;
	wanted->lson = new Element(&Before);
	if (wanted->lson->Proc->evtime() <= temp->Proc->evtime())
	    wanted->lson->lson = temp;
	else
	    wanted->lson->rson = temp;

	return true;
    }

    if (wanted->rson->Proc == &Before)
    {
	temp = wanted->rson;
	wanted->rson = new Element(&Before);
	if (wanted->rson->Proc->evtime() <= temp->Proc->evtime())
	    wanted->rson->lson = temp;
	else
	    wanted->rson->rson = temp;

	return true;
    }

    return false;
}

boolean ProcessHeap::InsertAfter(Process& ToInsert, Process& After)
{
    Element* temp, *wanted;

    if (Head->Proc == &After)
    {
	if (ToInsert.evtime() <= Head->Proc->evtime())
	{
	    temp = Head->lson;
	    Head->lson = new Element(&ToInsert);
	    if (temp->Proc->evtime() <= Head->lson->Proc->evtime())
		Head->lson->lson = temp;
	    else
		Head->lson->rson = temp;

	    return true;
	}
	else
	{
	    temp = Head->rson;
	    Head->rson = new Element(&ToInsert);
	    if (temp->Proc->evtime() <= Head->rson->Proc->evtime())
		Head->rson->lson = temp;
	    else
		Head->rson->rson = temp;

	    return true;
	}
    }

    wanted = ScanOnProcess(After, Head);
    if (wanted == 0) return false;

    if (wanted->lson->Proc == &After)
	wanted = wanted->lson;
    else
	wanted = wanted->rson;

    if (ToInsert.evtime() <= wanted->Proc->evtime())
    {
	temp = wanted->lson;
	wanted->lson = new Element(&ToInsert);
	if (temp->Proc->evtime() <= wanted->lson->Proc->evtime())
	    wanted->lson->lson = temp;
	else
	    wanted->lson->rson = temp;

	return true;
    }
    else
    {
	temp = wanted->rson;
	wanted->rson = new Element(&ToInsert);
	if (temp->Proc->evtime() <= wanted->rson->Proc->evtime())
	    wanted->rson->lson = temp;
	else
	    wanted->rson->rson = temp;

	return true;
    }
}

Process *ProcessHeap::Remove(Process* element)
{
    Element* ptr = Head, *temp, *wanted;
    Process* p = 0;
    boolean found = false;

    if (!Head) return 0;

    if (element == 0)
    {
	if (Head->lson == 0)
	{
	    p = Head->Proc;
	    Head = Head->rson;
	    return p;
	}

	while (!found)
	{
	    if (Process::Never <= ptr->Proc->evtime())
	    {
		if (ptr->lson != 0)
		{
		    temp = ptr;
		    ptr = ptr->lson;
		}
		else
		    found = true;
	    }
	}

	if (ptr->rson != 0)
	    temp->lson = ptr->rson;

	return ptr->Proc;
    }

    if (Head->Proc == element)
    {
	ptr = Head->rson;
	p = Head->Proc;

	Head = Head->lson;
	temp = Head;
	while (!found)
	{
	    if (temp->rson == 0)
		found = true;
	    else
		temp = temp->rson;
	}

	temp->rson = ptr;
	return p;
    }

    wanted = ScanOnProcess(*element, Head);
    if (wanted == 0) return 0;

    if (wanted->lson->Proc == element)
    {
	temp = wanted->lson;
	p = temp->Proc;
	if (wanted->lson->rson != 0)
	    wanted->lson = wanted->lson->rson;
	else
	{
	    wanted->lson = wanted->lson->lson;
	    return p;
	}

	if (wanted->lson->lson == 0)
	{
	    wanted->lson->lson = temp->lson->lson;
	    return p;
	}

	ptr = wanted->lson->lson;
	while (!found)
	{
	    if (ptr->lson == 0)
		found = true;
	    else
		ptr = ptr->lson;
	}

	ptr->lson = temp->lson->lson;
	return p;
    }
    else
    {
	temp = wanted->rson;
	p = temp->Proc;
	if (wanted->rson->rson != 0)
	    wanted->rson = wanted->rson->rson;
	else
	{
	    wanted->rson = wanted->rson->lson;
	    return p;
	}

	if (wanted->rson->lson == 0)
	{
	    wanted->rson->lson = temp->lson->lson;
	    return p;
	}

	ptr = wanted->rson->lson;
	while (!found)
	{
	    if (ptr->lson == 0)
		found = true;
	    else
		ptr = ptr->lson;
	}

	ptr->lson = temp->lson->lson;
	return p;
    }
}
