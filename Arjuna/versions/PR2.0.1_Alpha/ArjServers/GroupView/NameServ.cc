/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameServ.cc,v
 */

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define NAMESERVER_SERVER

#include "NameServ_stub.h"

#ifndef GROUPVIEW_H_
#  include "GroupView.h"
#endif

#ifndef SERVERVIEW_H_
#  include "ServerView.h"
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef MALLOC_H_
#  include <System/malloc.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


NameServer::NameServer (unsigned short& res) : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "NameServer::NameServer (unsigned short& res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    GV = 0;
    SV = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

NameServer::NameServer (const Uid& u, unsigned short& res) : LockManager(u)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "NameServer::NameServer (const Uid& u, unsigned short& res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    GV = 0;
    SV = 0;

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

NameServer::~NameServer ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "NameServer::~NameServer ()\n" << flush;
#endif

    terminate();

    if (GV)
	delete GV;

    if (SV)
	delete SV;
}

Boolean NameServer::imposeNewGroupViewState (ObjectState& os, int& ret)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::imposeNewGroupViewState (ObjectState& os, int& ret)" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    ret = 0;
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->imposeState(os);
    }

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();
    
    return result;
}

Boolean NameServer::getGroupViewState (ObjectState& os, int& ret, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "NameServer::getGroupViewState (ObjectState& os, unsigned long& version)" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    
    A.Begin();
    
    ret = 0;

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->getState(os, version);
    }

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();
    
    return result;
}

OutCome NameServer::addReplicas (const ReplicaDescriptor& objects, const Uid& group, int& ret, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::addReplicas ( const ReplicaDescriptor&, " << group << " )\n" << flush;
#endif

    AtomicAction A;
    OutCome result = NotDone;

    A.Begin();

    ret = 0;

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->addReplicas(objects, group, v_number);
	else
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		int res = -1;

		GV = new GroupView(res);

		if (res != 0)
		{
		    if (GV)
		    {
			delete GV;
			GV = 0;
		    }
		}
		else
		    result = GV->addReplicas(objects, group, v_number);
	    }
	    else
		result = Locked;
	}
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();

    return result;
}

OutCome NameServer::deleteReplicas (const Uid& group, const ReplicaDescriptor& objects, int& res,
				     unsigned long& v_number, const Boolean& deleteAll)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::deleteReplicas ( " << group << ", const ReplicaDescriptor&, "
		 << ((deleteAll == TRUE) ? "TRUE" : "FALSE") << " )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    int left = 0;

    A.Begin();

    res = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (GV)
	    result = GV->deleteReplicas(objects, group, deleteAll, left, v_number);
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();

    return result;
}

Boolean NameServer::getUseCount (const Uid& group, long& UseCount, int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::getUseCount ( " << group << " )" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    
    res = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->getUseCount(group, UseCount, v_number);
    }

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();
    
    return result;
}

OutCome NameServer::releaseGroup (const Uid& group, const char* hostname, pid_t userPid, int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::releaseGroup ( " << group << ", " << hostname << ", " << userPid << " )\n" << flush;
#endif

    AtomicAction A;
    OutCome result = NotDone;

    res = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->releaseGroup(group, hostname, userPid, v_number);
    }
    else
	result = Locked;

    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

Boolean NameServer::traverseDatabase (ReplicaDescriptor& objects, Uid& group, int& position,
				      const char* hostName, pid_t userPid, int& ret, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::traverseDatabase ( ReplicaDescriptor&, " << group << ", " << position
		 << ", " << hostName << ", " << userPid << " )" << endl;
#endif
    
    AtomicAction A;
    Boolean result = FALSE;

    ret = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->traverseDatabase(objects, group, position, hostName, userPid, v_number);
    }

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();

    return result;
}

Boolean NameServer::getView (ReplicaDescriptor& objects, const Uid& group, const char* hostname, pid_t userPid,
			     int& ret, unsigned long& v_number, const LockMode mode)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::getView ( ReplicaDescriptor&, " << group << ", " << hostname << ", "
		 << userPid << ", " << ((mode == READ) ? "READ" : "WRITE") << " )" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;

    ret = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->getView(objects, group, hostname, userPid, v_number, mode);
    }

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();
    
    return result;
}

Boolean NameServer::getExclusiveAccess (int& ret)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::getExclusiveAccess ()" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    
    ret = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->getExclusiveAccess();
    }
    
    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();
    
    return result;
}

OutCome NameServer::exclude (const ReplicaDescriptor& objects, const Uid& group,
			     int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::exclude ( const ReplicaDescriptor&, " << group << " )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    
    res = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->exclude(objects, group, v_number);
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

OutCome NameServer::include (const ReplicaDescriptor& objects, const Uid& group,
			     int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::include ( const ReplicaDescriptor&, " << group << " )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    
    res = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->include(objects, group, v_number);
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

OutCome NameServer::recover (const char* nodename, int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::recover ( " << nodename << " )" << endl;
#endif

    OutCome result = NotDone;
    AtomicAction A;
    
    res = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->recover(nodename, v_number);
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

OutCome NameServer::needsUpdating (const Uid& recovered, Boolean& isModified, ReplicaDescriptor& info, const char* nodename,
				   pid_t userPid, int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::needsUpdating ( " << recovered << ", ReplicaDescriptor&, "
		 << nodename << ", " << userPid << " )" << endl;
#endif
    
    AtomicAction A;
    OutCome result = NotDone;

    res = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->needsUpdating(recovered, isModified, info, nodename, userPid, v_number);
    }
    else
	result = Locked;

    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

Boolean NameServer::groupAssociation (const Uid& replicaUid, Uid& group, int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::groupAssociation ( " << replicaUid << ", Uid& group )" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    
    res = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->groupAssociation(replicaUid, group, v_number);
    }

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();
    
    return result;
}

OutCome NameServer::getNodeUpdateList (ReplicaDescriptor& toupdate, ReplicaDescriptor& unknownList,
				       const char* hostname, int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::getNodeUpdateList ( ReplicaDescriptor&, ReplicaDescriptor&, "
		 << hostname << " )" << endl;
#endif
    
    AtomicAction A;
    OutCome result = NotDone;

    res = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->getNodeUpdateList(toupdate, unknownList, hostname, v_number);
    }
    else
	result = Locked;

    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

Boolean NameServer::testAndSet (ReplicaDescriptor& objects, const Uid& group, const char* nodename, pid_t userPid, int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::testAndSet ( ReplicaDescriptor& objects, " << group << ", " << nodename << ", " << userPid << " )" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;

    res = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->testAndSet(objects, group, nodename, userPid, v_number);
    }

    if (result)
    {
	if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
	A.Abort();
    
    return result;
}

OutCome NameServer::alterLocation (const Uid& group, const Uid& replica,
				   const char* location, int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::alterLocation ( " << group << ", " << replica << ", " << location << " )" << endl;
#endif
    
    AtomicAction A;
    OutCome result = NotDone;

    res = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->alterLocation(group, replica, location, v_number);
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

OutCome NameServer::addTemplate (const Uid& group, Buffer& toUse,
				 int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::addTemplate ( " << group << ", Buffer& toUse )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;

    res = 0;
    
    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        if (GV)
	    result = GV->addTemplate(group, toUse, v_number);
	else
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		int res = -1;
	    
		GV = new GroupView(res);
		
		if (res != 0)
		{
		    if (GV)
		    {
			delete GV;
			GV = 0;
		    }
		}
		else
		    result = GV->addTemplate(group, toUse, v_number);
	    }
	    else
		result = Locked;
	}
    }
    else
        result = Locked;

    if (result == Done)
    {
        if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
        A.Abort();

    return result;
}

Boolean NameServer::getTemplate (const Uid& group, Buffer& toUse,
				 int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::getTemplate ( " << group << " )" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;

    res = 0;
    
    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
       if (GV)
	   result = GV->getTemplate(group, toUse, v_number);
    }

    if (result)
    {
        if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
        A.Abort();

    return result;
}

OutCome NameServer::deleteTemplate (const Uid& group, int& res,
				    unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::deleteTemplate ( " << group << " )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;

    res = 0;
    
    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        if (GV)
	    result = GV->deleteTemplate(group, v_number);
    }
    else
        result = Locked;

    if (result == Done)
    {
        if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
        A.Abort();

    return result;
}

OutCome NameServer::replaceTemplate (const Uid& group, Buffer& toUse,
				     int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::replaceTemplate ( " << group << ", Buffer& toUse )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;

    res = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        if (GV)
	    result = GV->replaceTemplate(group, toUse, v_number);
    }
    else
        result = NotDone;
  
    if (result == Done)
    {
        if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
        A.Abort();

    return result;
}

OutCome NameServer::assignCompositeObject (const Uid& groupAssignTo, const ReplicaDescriptor& objects,
					   int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::assignCompositeObject ( " << groupAssignTo << ", const ReplicaDescriptor& objects )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;

    res = 0;
    
    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        if (GV)
	    result = GV->assignCompositeObject(groupAssignTo, objects, v_number);
    }
    else
        result = Locked;

    if (result == Done)
    {
        if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
        A.Abort();

    return result;
}

OutCome NameServer::assignCompositeObject (const Uid& groupAssignTo, const Uid& groupToAssign,
					   int& res, unsigned long& v_number)

{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::assignCompositeObject ( " << groupAssignTo << ", " << groupToAssign << " )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    
    res = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->assignCompositeObject(groupAssignTo, groupToAssign, v_number);
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

OutCome NameServer::removeCompositeObject (const Uid& groupAssignedTo, const ReplicaDescriptor& objects,
					   int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::removeCompositeObject ( " << groupAssignedTo << ", const ReplicaDescriptor& objects )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;

    res = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
        if (GV)
	    result = GV->removeCompositeObject(groupAssignedTo, objects, v_number);
    }
    else
        result = Locked;

    if (result == Done)
    {
        if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
        A.Abort();

    return result;
}
    
OutCome NameServer::removeCompositeObject (const Uid& groupAssignedTo, const Uid& groupAssigned,
					   int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "OutCome NameServer::removeCompositeObject ( " << groupAssignedTo << ", " << groupAssigned << " )" << endl;
#endif

    AtomicAction A;
    OutCome result = NotDone;
    
    res = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->removeCompositeObject(groupAssignedTo, groupAssigned, v_number);
    }
    else
	result = Locked;
    
    if (result == Done)
    {
	if (A.End() != COMMITTED)
	    result = NotDone;
    }
    else
	A.Abort();
    
    return result;
}

void NameServer::storeName (Boolean& done, Buffer key_buff, Buffer data_buff, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NameServer::storeName (...)" << endl;
#endif
    
    AtomicAction A;

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (SV)
	    SV->storeName(done, key_buff, data_buff, v_number);
	else
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		int res = -1;

		SV = new ServerView(res);

		if (res != 0)
		{
		    if (SV)
		    {
			delete SV;
			SV = 0;
		    }
		    done = FALSE;
		}
		else
		    SV->storeName(done, key_buff, data_buff, v_number);
	    }
	    else
		done = FALSE;
	}
    }

    if (done)
    {
	if (A.End() != COMMITTED)
	    done = FALSE;
    }
    else
	A.Abort();
}

void NameServer::fetchName (Boolean& done, Buffer key_buff, Buffer& data_buff, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NameServer::fetchName (...)" << endl;
#endif
    
    AtomicAction A;

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (SV)
	    SV->fetchName(done, key_buff, data_buff, v_number);
    }

    if (done)
    {
	if (A.End() != COMMITTED)
	    done = FALSE;
    }
    else
	A.Abort();
}

void NameServer::removeName (Boolean& done, Buffer key_buff, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NameServer::removeName (...)" << endl;
#endif
    
    AtomicAction A;
    int left = 0;

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (SV)
	    SV->removeName(done, key_buff, left, v_number);
    }

    if (done)
    {
	if (A.End() != COMMITTED)
	    done = FALSE;
    }
    else
	A.Abort();
}

void NameServer::replaceName (Boolean& done, Buffer key_buff, Buffer data_buff, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NameServer::replaceName (Boolean& done, Buffer key_buff, Buffer data_buff, unsigned long& v_number)" << endl;
#endif
    
    AtomicAction A;

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (SV)
	    SV->replaceName(done, key_buff, data_buff, v_number);
    }

    if (done)
    {
	if (A.End() != COMMITTED)
	    done = FALSE;
    }
    else
	A.Abort();
}

void NameServer::getNextName (Boolean& done, Buffer& key_buff, int& position, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NameServer::getNextName (Boolean& done, Buffer& key_buff, int& position, unsigned long& v_number)" << endl;
#endif
    
    AtomicAction A;

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (SV)
	    SV->getNextName(done, key_buff, position, v_number);
    }

    if (done)
    {
	if (A.End() != COMMITTED)
	    done = FALSE;
    }
    else
	A.Abort();
}

void NameServer::reset (int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "void NameServer::reset (int& res)" << endl;
#endif
    
    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

Boolean NameServer::save_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::save_state (ObjectState& os, ObjectType)" << endl;
#endif
    
    Boolean res;
    Uid u;       // Have to do this because of inability of 2.1 compilers to handle temporaries correctly!
    
    if (GV)
    {
	u = GV->get_uid();
	res = os.pack(0) && u.pack(os);
    }
    else
	res = os.pack(-1);

    if (res)
    {
	if (SV)
	{
	    u = SV->get_uid();
	    res = os.pack(0) && u.pack(os);
	}
	else
	    res = os.pack(-1);
    }

    return res;
}

Boolean NameServer::restore_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_NAMING_AND_BINDING << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::restore_state (ObjectState& os, ObjectType)" << endl;
#endif
    
    Boolean res;
    int err, ret;
    
    if (GV)
    {
	delete GV;
	GV = 0;
    }
    
    if (SV)
    {
	delete SV;
	SV = 0;
    }

    ret = 0;
    res = os.unpack(ret);
    
    if ((res) && (ret == 0))
    {
	Uid objUid(NIL_UID);
	res = objUid.unpack(os);

	if (res)
	{
	    err = -1;
	    GV = new GroupView(objUid, err);
	    res = (Boolean) (err == 0);
	}
    }

    if (res)
    {
	ret = 0;
	res = os.unpack(ret);
	
	if ((res) && (ret == 0))
	{
	    Uid objUid(NIL_UID);
	    res = objUid.unpack(os);

	    if (res)
	    {
		err = -1;
		SV = new ServerView(objUid, err);
		res = (Boolean) (err == 0);
	    }
	}
    }

    return res;
}

const TypeName NameServer::type () const
{
    return "/StateManager/LockManager/NameServ";
}
