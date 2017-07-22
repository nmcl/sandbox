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

#ifndef TRACKER_H_
#include "Tracker.h"
#endif

NodeData::NodeData ()
                    : nodeName(0),
		      next(0)
{
    TTF.tv_sec = TTF.tv_usec = 0;
    TTR.tv_sec = TTR.tv_usec = 0;
}

NodeData::~NodeData ()
{
    if (nodeName)
	delete nodeName;
}

Boolean NodeData::pack (Buffer& buff) const
{
    Boolean result = FALSE;

    if (nodeName)
    {
	result = buff.pack(nodeName);
	result = result && buff.pack(TTF.tv_sec);
	result = result && buff.pack(TTF.tv_usec);
	result = result && buff.pack(TTR.tv_sec);
	result = result && buff.pack(TTR.tv_usec);
    }

    return result;
}

Boolean NodeData::unpack (Buffer& buff)
{
    Boolean result = FALSE;

    result = buff.unpack(nodeName);
    result = result && buff.unpack(TTF.tv_sec);
    result = result && buff.unpack(TTF.tv_usec);
    result = result && buff.unpack(TTR.tv_sec);
    result = result && buff.unpack(TTR.tv_usec);

    return result;
}

ostream& NodeData::print (ostream& strm) const
{
    strm << "****" << endl;
    strm << "Node name : " << nodeName << endl;
    strm << "TTF : <" << TTF.tv_sec << ", " << TTF.tv_usec << ">" << endl;
    strm << "TTR : <" << TTR.tv_sec << ", " << TTR.tv_usec << ">" << endl;
    
    if (next)
	strm << *next;
    else
	strm << "End of list" << endl;

    return strm;
}

ostream& operator<< (ostream& strm, const NodeData& nd)
{
    return nd.print(strm);
}


Tracker::Tracker (int& res) : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    cout << "Tracker::Tracker (int& res)" << endl;
#endif
    
    int value = -1;

    head = 0;
    numberOfNodes = 0;
    depList = new Dependency(value);
    if (value == 0)
	res = 0;
}

Tracker::Tracker (const Uid& u, int& res) : LockManager(u)
{
#ifdef DEBUG
    cout << "Tracker::Tracker (const Uid& u, int& res)" << endl;
#endif

    AtomicAction A;
    
    head = 0;
    numberOfNodes = 0;
    depList = 0;

    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

Tracker::~Tracker ()
{
#ifdef DEBUG
    cout << "Tracker::~Tracker ()" << endl;
#endif
    
    terminate();

    if (head)
	deleteList();

    if (depList)
	delete depList;
}

void Tracker::deleteList ()
{
#ifdef DEBUG
    cout << "void Tracker::deleteList ()" << endl;
#endif
    
    NodeData* ptr2 = 0;

    if (head)
    {
	ptr2 = head;
	while (ptr2)
	{
	    ptr2 = ptr2->next;
	    delete head;
	    head = ptr2;
	}

	head = 0;
    }
}


/* TTF/TTR data currently given - should fetch eventually (more dynamic) */

Boolean Tracker::registerNode (char* newNode, timeval TTF, timeval TTR)
{
#ifdef DEBUG
    cout << "Boolean Tracker::registerNode" << endl;
#endif
    
    AtomicAction A;
    Boolean result = FALSE;
    NodeData* ptr = 0;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (!head)
	{
	    head = new NodeData;
	    ptr = head;
	}
	else
	{
	    ptr = head;
	    while (ptr->next)
		ptr = ptr->next;

	    ptr->next = new NodeData;
	    ptr = ptr->next;
	}

	ptr->nodeName = new char[strlen(newNode)+1];
	memset(ptr->nodeName, '\0', strlen(newNode)+1);
	memcpy(ptr->nodeName, newNode, strlen(newNode));
	ptr->TTF.tv_sec = TTF.tv_sec;
	ptr->TTF.tv_usec = TTF.tv_usec;
	ptr->TTR.tv_sec = TTR.tv_sec;
	ptr->TTR.tv_usec = TTR.tv_usec;
	numberOfNodes++;

	if (A.End() == COMMITTED)
	    result = TRUE;
    }
    else
	A.Abort();

    return result;
}

Boolean Tracker::unregisterNode (char* toDelete)
{
#ifdef DEBUG
    cout << "Boolean Tracker::unregisterNode" << endl;
#endif
    
    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (!head)
	{
	    A.Abort();
	    return FALSE;
	}

	Boolean found = FALSE;

	NodeData *ptr1 = head, *ptr2 = 0;
	while ((!found) && (ptr1))
	{
	    if (strcmp(ptr1->nodeName, toDelete) == 0)
		found = TRUE;
	    else
	    {
		ptr2 = ptr1;
		ptr1 = ptr1->next;
	    }
	}

	if (found)
	{
	    if (!ptr2)
		head = ptr1->next;
	    else
		ptr2->next = ptr1->next;

	    delete ptr1;
	    ptr1 = 0;
	    numberOfNodes--;

	    if (A.End() == COMMITTED)
		result = TRUE;
	}
	else
	    A.Abort();
    }
    else
	A.Abort();

    return result;
}

Boolean Tracker::modifyNode (char* name, timeval TTF, timeval TTR)
{
#ifdef DEBUG
    cout << "Boolean Tracker::modifyNode" << endl;
#endif
    
    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (unregisterNode(name))
	{
	    if (registerNode(name, TTF, TTR))
	    {
		if (A.End() == COMMITTED)
		    result = TRUE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();

    return result;
}

Boolean Tracker::updateNode (char* name, timeval TTF, timeval TTR)
{
#ifdef DEBUG
    cout << "Boolean Tracker::updateNode" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    
    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (!head)
	{
	    A.Abort();
	    return FALSE;
	}
	
	Boolean found = FALSE;
	
	NodeData *ptr = head;
	while ((!found) && (ptr))
	{
	    if (strcmp(ptr->nodeName, name) == 0)
		found = TRUE;
	    else
		ptr = ptr->next;
	}
	
	if (found)
	{
	    ptr->TTF.tv_sec = TTF.tv_sec;
	    ptr->TTF.tv_usec = TTF.tv_usec;
	    ptr->TTR.tv_sec = TTR.tv_sec;
	    ptr->TTR.tv_usec = TTR.tv_usec;
	    
	    if (A.End() == COMMITTED)
		result = TRUE;
	}
	else
	    A.Abort();
    }
    else
	A.Abort();
    
    return result;
}
    
Boolean Tracker::listNodes (NodeData*& ptr)
{
#ifdef DEBUG
    cout << "Boolean Tracker::listNodes" << endl;
#endif
    
    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	{
	    result = TRUE;
	    ptr = head;
	}
    }
    else
	A.Abort();

    return result;
}

int Tracker::getNumberOfNodes ()
{
#ifdef DEBUG
    cout << "int Tracker::getNumberOfNodes ()" << endl;
#endif

    AtomicAction A;
    int number = -1;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    number = numberOfNodes;
    }
    else
	A.Abort();
    
    return number;
}

Boolean Tracker::hasDependency (NodeData* toCheck, NodeData* against)
{
#ifdef DEBUG
    cout << "Boolean Tracker::hasDependency" << endl;
#endif
    
    if ((!toCheck) || (!against))
	return FALSE;
    
    unsigned long againstTTF, againstTTR;
    unsigned long toCheckTTF, toCheckTTR;
    Boolean result = FALSE;
    
    againstTTF = (against->TTF.tv_sec*1000000)+against->TTF.tv_usec;
    againstTTR = (against->TTR.tv_sec*1000000)+against->TTR.tv_usec;
    toCheckTTF = (toCheck->TTF.tv_sec*1000000)+toCheck->TTF.tv_usec;
    toCheckTTR = (toCheck->TTR.tv_sec*1000000)+toCheck->TTR.tv_usec;

    if ((toCheckTTR < againstTTF) || (againstTTR < toCheckTTF))
	result = FALSE;
    else
	if ((toCheckTTF >= againstTTF) && (toCheckTTR >= againstTTR))
	    result = TRUE;

    return result;
}

Boolean Tracker::doTrack ()
{
#ifdef DEBUG
    cout << "Boolean Tracker::doTrack ()" << endl;
#endif
    
    Boolean result = FALSE;
    AtomicAction A;
    NodeData *indx = 0, *ptr = 0;
    
    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (!head)
	{
	    A.Abort();
	    return result;
	}

	indx = head;
	
	while (indx)
	{
	    ptr = head;

	    while (ptr)
	    {
		if (strcmp(indx->nodeName, ptr->nodeName) != 0)
		{
		    if (hasDependency(indx, ptr))
			depList->assignDependency(indx->nodeName, ptr->nodeName, SLIGHT);
		}
		
		ptr = ptr->next;
	    }

	    indx = indx->next;
	}
	
	if (A.End() == COMMITTED)
	    result = TRUE;
    }
    else
	A.Abort();

    return result;
}

ostream& Tracker::my_print (ostream& strm) const
{
    if (!head)
	strm << "No data" << endl;
    else
    {
	strm << *head;
	
	if (!depList)
	    strm << "No dependency list" << endl;
	else
	{
	    strm << "Dependency list :" << endl;
	    strm << *depList;
	}
    }
    
    return strm;
}

ostream& operator<< (ostream& strm, const Tracker& t)
{
    return t.my_print(strm);
}

Boolean Tracker::save_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    cout << "Boolean Tracker::save_state" << endl;
#endif
    
    Boolean result;
    NodeData* ptr = 0;

    result = (depList->get_uid()).pack(os);

    result = result && os.pack(numberOfNodes);

    if ((!result) || (numberOfNodes == 0))
	return result;

    ptr = head;
    for (int i = 0; ((result) && (ptr)); i++)
    {
	result = ptr->pack(os);
	ptr = ptr->next;
    }

    return result;
}

Boolean Tracker::restore_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    cout << "Boolean Tracker::restore_state" << endl;
#endif
    
    Boolean result = TRUE;
    NodeData* ptr = 0;

    Uid objUid(NIL_UID);
    result = objUid.unpack(os);

    if (result)
    {
	int value = -1;

	if (depList)
	    delete depList;

	depList = new Dependency(objUid, value);

	result = (Boolean) (value == 0);
    }

    result = result && os.unpack(numberOfNodes);

    if ((!result) || (numberOfNodes == 0))
	return result;

    if (head)
	deleteList();

    head = new NodeData;
    for (int i = 0; ((i < numberOfNodes) && (result)); i++)
    {
	if (!ptr)
	    ptr = head;
	else
	{
	    ptr->next = new NodeData;
	    ptr = ptr->next;
	}

	result = ptr->unpack(os);
    }

    return result;
}

const TypeName Tracker::type () const
{
    return "/StateManager/LockManager/Tracker";
}
