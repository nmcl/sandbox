#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef SYS_PARAM_H_
#include <System/sys/param.h>
#endif

#ifndef MEMORY_H_
#include <System/memory.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef PROTOS_H_
#include <System/protos.h>
#endif

#ifndef UNISTD_H_
#include <System/unistd.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#ifndef DEPENDENCY_H_
#include "Dependency.h"
#endif

listElement::listElement ()
                          : element(0),
			    next(0)
{
#ifdef DEBUG
    cout << "listElement::listElement ()" << endl;
#endif    
}

listElement::~listElement ()
{
#ifdef DEBUG
    cout << "listElement::~listElement ()" << endl;
#endif

    if (element)
	delete element;
}


toDelete::toDelete ()
                    : head(0)
{
#ifdef DEBUG
    cout << "toDelete::toDelete ()" << endl;
#endif    
}

toDelete::~toDelete ()
{
#ifdef DEBUG
    cout << "toDelete::~toDelete ()" << endl;
#endif
    
    if (head)
	purgeList();
}

void toDelete::addElement (listElement* toadd)
{
#ifdef DEBUG
    cout << "void toDelete::addElement" << endl;
#endif
    
    listElement *ptr = 0, *indx = 0;
    
    if (toadd)
    {
	toadd->next = 0;

	if (!head)
	{
#ifdef DEBUG
	    cout << "void toDelete::addElement - New list" << endl;
#endif
	    head = toadd;
	}
	else
	{
#ifdef DEBUG
	    cout << "void toDelete::addElement - adding to exisiting list" << endl;
#endif	    
	    indx = head;
	    while (indx)
	    {
		ptr = indx;
		indx = indx->next;
	    }
	    
	    ptr->next = toadd;
	}
    }
}

void toDelete::purgeList ()
{
#ifdef DEBUG
    cout << "void toDelete::purgeList ()" << endl;
#endif
    
    listElement *ptr;

    if (head)
    {
#ifdef DEBUG
	cout << "void toDelete::purgeList - have list to delete" << endl;
#endif	

	while (head)
	{
	    ptr = head;
	    head = ptr->next;
	    delete ptr;
	}

	head = 0;
    }
#ifdef DEBUG
    else
	cout << "void toDelete::purgeList - empty list" << endl;
#endif    
}
	    
    
Dependency::Dependency (int& res) : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    cout << "Dependency::Dependency (int& res)" << endl;
#endif
    
    AtomicAction A;
    
    head = 0;
    numberElements = 0;

    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

Dependency::Dependency (Uid& u, int& res) : LockManager(u)
{
#ifdef DEBUG
    cout << "Dependency::Dependency (Uid& u, int& res)" << endl;
#endif
 
    AtomicAction A;
    
    head = 0;
    numberElements = 0;

    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

Dependency::~Dependency ()
{
#ifdef DEBUG
    cout << "Dependency::~Dependency ()" << endl;
#endif
    
    terminate();

    if (head)
	deleteList();

    deletedObjects.purgeList();
}

void Dependency::deleteList ()
{
#ifdef DEBUG
    cout << "void Dependency::deleteList ()" << endl;
#endif

    listElement *ptr = head;
    
    while (head)
    {
	head = head->next;
	deletedObjects.addElement(ptr);
	ptr = head;
    }
    
    head = 0;
}

Boolean Dependency::scanForName (char* toFind, listElement*& marker)
{
#ifdef DEBUG
    cout << "Boolean Dependency::scanForName" << endl;
#endif
    
    AtomicAction A;
    Boolean found = FALSE;

    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	while ((marker != 0) && (!found))
	{
	    if (marker->element->isSame(toFind))
		found = TRUE;
	    else
		marker = marker->next;
	}

	if (A.End() != COMMITTED)
	    found = FALSE;
    }
    else
	A.Abort();
	
    return found;
}

DependencyStore* Dependency::createNewName (char* toCreate)
{
#ifdef DEBUG
    cout << "DependencyStore* Dependency::createNewName" << endl;
#endif
    
    listElement *ptr = 0, *mkr = 0;
    AtomicAction A;
    
    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (head == 0)
	{
	    head = new listElement;
	    head->element = new DependencyStore(toCreate);
	    mkr = head;
	}
	else
	{
	    ptr = head;
	    
	    while (ptr != 0)
	    {
		mkr = ptr;
		ptr = ptr->next;
	    }

	    mkr->next = new listElement;
	    mkr->next->element = new DependencyStore(toCreate);
	    mkr = mkr->next;
	}

	numberElements++;
	
	if (A.End() != COMMITTED)
	    mkr = 0;
    }
    else
	A.Abort();

    if (!mkr)
	return 0;
    else
	return mkr->element;
}

Boolean Dependency::assignDependency (char* me, char* dependentUpon, DependencyLevel level)
{
#ifdef DEBUG
    cout << "Boolean Dependency::assignDependency" << endl;
#endif
    
    AtomicAction B;
    Boolean result = FALSE, ok = FALSE;
    DependencyStore *newStore = 0;

    B.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	listElement *marker = head;
	
	if (!scanForName(me, marker))
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {

		newStore = createNewName(me);
		if (newStore)
		    ok = newStore->addDependency(dependentUpon, level);
		else
		{
		    B.Abort();
		    return FALSE;
		}
	    }
	    else
	    {
		B.Abort();
		return FALSE;
	    }
	}
	else
	    ok = marker->element->addDependency(dependentUpon, level);

	if (ok)
	{
	    if (B.End() == COMMITTED)
		result = TRUE;
	}
	else
	    B.Abort();
    }
    else
	B.Abort();
    
    return result;
}

void Dependency::deleteElement (DependencyStore* marker)
{
#ifdef DEBUG
    cout << "void Dependency::deleteElement" << endl;
#endif
    
    AtomicAction A;

    A.Begin();
    
    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	listElement *ptr = head, *indx = 0;

	if (head->element == marker)
	{
	    ptr = head;
	    head = head->next;
	}
	else
	{
	    while (ptr->next->element != marker)
		ptr = ptr->next;

	    indx = ptr;
	    ptr = ptr->next;
	    indx->next = indx->next->next;
	}

	deletedObjects.addElement(ptr);

	A.End(); // ??!!
    }
    else
	A.Abort();    
}

Boolean Dependency::getDependency (char* name, isDependent** list)
{
#ifdef DEBUG
    cout << "Boolean Dependency::getDependency" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    isDependent* ptr;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	listElement* marker = head;

	if (scanForName(name, marker))
	{
	    ptr = marker->element->DepList();

	    if (A.End() == COMMITTED)
	    {
		*list = ptr;
		result = TRUE;
	    }
	}
	else
	    A.Abort();
    }
    else
	A.Abort();

    return result;
}

Boolean Dependency::deleteDependency (char* me, char* dependentUpon)
{
#ifdef DEBUG
    cout << "Boolean Dependency::deleteDependency" << endl;
#endif
    
    AtomicAction B;
    Boolean result = FALSE, ok = TRUE;

    B.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	listElement* marker = head;

	if (scanForName(me, marker))
	{
	    int number = 0;
	    marker->element->deleteDependency(dependentUpon, number);

	    if (number == 0)
	    {
		if (setlock(new Lock(WRITE), 0) == GRANTED)
		{
		    deleteElement(marker->element);
		    numberElements--;
		}
		else
		{
		    B.Abort();
		    ok = FALSE;
		}
	    }

	    if (ok)
		if (B.End() == COMMITTED)
		    result = TRUE;
	}
	else
	    B.Abort();
    }
    else
	B.Abort();

    return result;
}

ostream& Dependency::my_print (ostream& strm) const
{
    listElement* ptr = head;
    
    if (!head)
	strm << "End of list" << endl;
    else
    {
	while (ptr)
	{
	    strm << *ptr->element;
	    ptr = ptr->next;
	}
    }
    
    return strm;
}

ostream& operator<< (ostream& strm, const Dependency& d)
{
    return d.my_print(strm);
}

	

Boolean Dependency::save_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    cout << "Boolean Dependency::save_state" << endl;
#endif
    
    Boolean result;

    result = os.pack(numberElements);

    if ((!result) || (numberElements == 0))
	return result;

    listElement* ptr = head;
    for (int i = 0; ((i < numberElements) && (result)); i++)
    {
	result = (ptr->element->get_uid()).pack(os);
	ptr = ptr->next;
    }

    return result;
}

Boolean Dependency::restore_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    cout << "Boolean Dependency::restore_state" << endl;
#endif
    
    Boolean result;

    result = os.unpack(numberElements);

    if ((!result) || (numberElements == 0))
	return result;

    listElement* ptr = 0;

    if (head)
	deleteList();

    deletedObjects.purgeList();

    for (int i = 0; ((i < numberElements) && (result)); i++)
    {
	Uid objUid(NIL_UID);
	result = objUid.unpack(os);

	if (result)
	{
	    if (!head)
	    {
		head = new listElement;
		head->element = new DependencyStore(objUid);
		ptr = head;
	    }
	    else
	    {
		ptr->next = new listElement;
		ptr->next->element = new DependencyStore(objUid);
		ptr = ptr->next;
	    }
	}
    }
}

const TypeName Dependency::type () const
{
    return "/StateManager/LockManager/Dependency";
}
