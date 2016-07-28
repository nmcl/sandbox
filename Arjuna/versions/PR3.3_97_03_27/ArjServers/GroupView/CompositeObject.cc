/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: CompositeObject.cc,v 1.7 1995/02/22 11:26:44 nmcl Exp $
 */


#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef COMPOBJECT_H_
#  include "CompositeObject.h"
#endif

#ifndef GROUPDATA_H_
#  include <ArjServers/GroupData.h>
#endif

#ifndef GROUPVIEW_H_
#  include "GroupView.h"
#endif

#ifndef REPLICALIST_H_
#  include "ReplicaList.h"
#endif

#ifndef DEBUG_H_
#  include <Common/Debug.h>
#endif


CompositeObject::CompositeObject ()
				 : subObject(new Uid(NIL_UID)),
				   next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "CompositeObject::CompositeObject ()" << endl;
#endif
}

CompositeObject::CompositeObject (const Uid& u)
				 : subObject(new Uid(u)),
				   next(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "CompositeObject::CompositeObject ( " << u << " )" << endl;
#endif
}

CompositeObject::~CompositeObject ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "CompositeObject::~CompositeObject ()" << endl;
#endif

    if (subObject)
        delete subObject;
}

Boolean CompositeObject::isSubObject (const Uid& u) const
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean CompositeObject::isSubObject ( " << u << " ) const" << endl;
#endif

    Boolean found = FALSE;
    
    if (u == *subObject)
        found = TRUE;
    else
        found = ((next == (CompositeObject*) 0) ? FALSE : next->isSubObject(u));

    return found;
}

ReplicaDescriptor* CompositeObject::getList (const char* hostName, pid_t pID) const
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "ReplicaDescriptor* CompositeObject::getList ( " << hostName << ", " << pID << " ) const" << endl;
#endif

    ReplicaDescriptor* rd    = (ReplicaDescriptor*) 0;
    unsigned long      dummy = 0;
    ObjectGroups*      ptr   = GroupView::searchList(*subObject, GroupView::head);

    if (ptr == (ObjectGroups*) 0)  // unknown Uid
	return (ReplicaDescriptor*) 0;

    if (ptr->me == NULL)
    {
	ptr->me = GroupView::createReplicaList(*subObject);

	if (!ptr->me)
	    return (ReplicaDescriptor*) 0;
    }

    rd = ptr->me->getList(hostName, pID, dummy);

    if (next)
    {
	ReplicaDescriptor* tempRD = next->getList(hostName, pID);

	if (tempRD)
	{
	    *rd += *tempRD;
	    delete tempRD;
	}
    }

    return rd;
}

CompositeObject::OutCome CompositeObject::releaseGroup (const char* hostName, pid_t pID) const
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome CompositeObject::releaseGroup ( " << hostName << ", " << pID << " ) const" << endl;
#endif

    CompositeObject::OutCome result = NOTDONE;
    unsigned long            dummy  = 0;
    ObjectGroups*            ptr    = GroupView::searchList(*subObject, GroupView::head);

    if (ptr == (ObjectGroups*) 0)
	return CompositeObject::NOTDONE;

    if (ptr->me == NULL)
    {
	ptr->me = GroupView::createReplicaList(*subObject);
	
	if (!ptr->me)
	    return CompositeObject::NOTDONE;
    }

    result = ((ptr->me->releaseGroup(hostName, pID, dummy) == DONE) ? CompositeObject::DONE : CompositeObject::NOTDONE);

    if (result == CompositeObject::DONE)
	return ((next != (CompositeObject*) 0) ? next->releaseGroup(hostName, pID) : result);
    else
	return result;
}

Boolean CompositeObject::scanCompositeObject (CompositeObject* toScan, const Uid& myUid) const
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean CompositeObject::scanCompositeObject ( " << myUid << " ) const" << endl;
#endif

    Boolean found = FALSE;

    while ((toScan) && (!found))
    {
	if (*toScan->subObject != myUid)
	{
	    ObjectGroups* ptr = GroupView::searchList(*toScan->subObject, GroupView::head);

	    if (ptr == (ObjectGroups*) 0)
		return FALSE;

	    if (ptr->me == NULL)
	    {
		ptr->me = GroupView::createReplicaList(*toScan->subObject);

		if (!ptr->me)
		    return FALSE;
	    }
	    
	    if (ptr->me->envelope)
		found = scanCompositeObject(ptr->me->envelope, myUid);

	    toScan = toScan->next;
	}
	else
	    found = TRUE;
    }
    
    return found;
}

/*
 * Need to guarantee that subobject traversal forms
 * an acyclic graph, otherwise infinite recursion.
 */

Boolean CompositeObject::isAcyclic (const Uid& u, const Uid& myUid) const
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PRIVATE;
    debug_stream << "Boolean CompositeObject::isAcyclic ( " << u << ", " << myUid << " ) const" << endl;
#endif    

    Boolean      found = FALSE;
    ObjectGroups *ptr  = GroupView::searchList(u, GroupView::head);

    if (ptr == (ObjectGroups*) 0)
	return FALSE;

    if (ptr->me == NULL)
    {
	ptr->me = GroupView::createReplicaList(u);
	
	if (!ptr->me)
	    return FALSE;
    }

    if (ptr->me->envelope)
	found = scanCompositeObject(ptr->me->envelope, myUid); // if it is present then is cyclic
    
    return !(found);
}

Boolean CompositeObject::addObject (const Uid& u, const Uid& myUid)
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean CompositeObject::addObject ( " << u << ", " << myUid << " )" << endl;
#endif

    Boolean result = TRUE;
    
    if (*subObject != u)
    {
	if (*subObject == NIL_UID)
	{
	    result = isAcyclic(u, myUid);
	    if (result)
		*subObject = u;
	    else
		error_stream << WARNING << "Adding " << u << " will create cycle. Not Done." << endl;
	}
	else
	{
	    if (next == (CompositeObject*) 0)
		next = new CompositeObject();

	    result = next->addObject(u, myUid);
	}
    }
    
    return result;
}

void CompositeObject::removeObject (const Uid& u)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void CompositeObject::removeObject ( " << u << " )" << endl;
#endif

    // deal with head of list separately

    if (next)
    {
        if (next->getSubObject() == u)
	{
	    CompositeObject* coPtr = next;
	    next = coPtr->next;
	    delete coPtr;
	}
	else
	    next->removeObject(u);
    }
}

const Uid& CompositeObject::getSubObject () const
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "const Uid& CompositeObject::getSubObject () const" << endl;
#endif

    return *subObject;
}

Boolean CompositeObject::pack (Buffer& packInto, int number) const
{
    Boolean result = TRUE;
    
    if (number == -1)
    {
	const CompositeObject* coPtr = this;
	number = 0;

	while (coPtr)
	{
	    number++;
	    coPtr = coPtr->next;
	}
	result = packInto.pack(number);
    }
    
    result = result && subObject->pack(packInto);
    if (next)
	result = result && next->pack(packInto, 0);
	
    return result;
}

Boolean CompositeObject::unpack (Buffer& unpackFrom, int number)
{
    Boolean result = TRUE;
    
    if (number == -1)
	result = unpackFrom.unpack(number);

    if (subObject == (Uid*) 0)
	subObject = new Uid;
    result = result && subObject->unpack(unpackFrom);
    number--;

    if (number > 0)
    {
	if (next == (CompositeObject*) 0)
	    next = new CompositeObject;
	
	result = result && next->unpack(unpackFrom, number);
    }
    
    if ((number == 0) && (next))
    {
	CompositeObject *coPtr = next, *indx;
	next = (CompositeObject*) 0;
	while (coPtr)
	{
	    indx = coPtr->next;
	    delete coPtr;
	    coPtr = indx;
	}
    }
    
    return result;
}

ostream& CompositeObject::print (ostream& strm) const
{
    strm << "Sub-object Uid : " << *subObject << endl;
    if (next)
        strm << *next;
    return strm;
}

ostream& operator<< (ostream& strm, const CompositeObject& co)
{
    return co.print(strm);
}

void CompositeObject::deleteAll (CompositeObject*& co)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void CompositeObject::deleteAll (CompositeObject*& co)" << endl;
#endif

    CompositeObject* coPtr = co;
    
    while (coPtr)
    {
        co = coPtr->next;
	delete coPtr;
	coPtr = co;
    }

    co = (CompositeObject*) 0;
}


