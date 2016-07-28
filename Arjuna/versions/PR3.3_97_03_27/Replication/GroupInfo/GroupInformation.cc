/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: GroupInformation.cc,v 1.10 1995/09/30 10:22:05 nmcl Exp $
 */

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

#ifndef REPDESC_H_
#  include <ArjServers/RepDesc_stub.h>
#endif

#ifndef REGISTERGROUPS_H_
#  include <Replication/GroupInformation.h>
#endif

#ifndef REPOBJSTOREIF_H_
#  include <Replication/RepObjStoreIF.h>
#endif

#ifndef NAMINGR_H_
#  include "NamingR.h"
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif



GroupInformation::GroupInformation ()
				   : group_uid(0),
				     NS(0),
				     doRelease(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "GroupInformation::GroupInformation ()" << endl;
#endif
}

GroupInformation::GroupInformation (const Uid& groupUid)
				   : group_uid(new Uid(groupUid)),
				     NS(0),
				     doRelease(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "GroupInformation::GroupInformation ( " << groupUid << " )" << endl;
#endif
}

GroupInformation::GroupInformation (const StateManager& sm)
				   : group_uid(new Uid(sm.get_uid())),
				     NS(0),
				     doRelease(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "GroupInformation::GroupInformation ( " << sm.get_uid() << " )" << endl;
#endif
}

GroupInformation::~GroupInformation ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "GroupInformation::~GroupInformation ()" << endl;
#endif

    /* Make sure we release the group if we have locked it. */

    if (doRelease)
        (void) releaseGroup();
    if (NS)
	delete NS;
    if (group_uid)
	delete group_uid;
}

Boolean GroupInformation::lockGroup (LockMode lMode, const Uid& gUid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean GroupInformation::lockGroup ( "
                 << ((lMode == READ) ? "READ" : "WRITE") << " )" << endl;
#endif
    
    AtomicAction A;
    Boolean result = FALSE;
    Uid* u = (Uid*) 0;

    A.Begin();

    if (activateDatabase())
    {
        if (gUid != NIL_UID)
	    u = new Uid(gUid);
	else
	    if (group_uid != (Uid*) 0)
	        u = new Uid(*group_uid);

	if (u != (Uid*) 0)
	    result = NS->lockGroup(*u, NSInterface::nodeName, NSInterface::processID, lMode);
    }

    if (u)
        delete u;

    if (result)
        result = (Boolean) (A.End() == COMMITTED);
    else
        A.Abort();
	  
    return result;
}

Boolean GroupInformation::getGroup (ReplicaDescriptor*& groupView)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "Boolean GroupInformation::getGroup (ReplicaDescriptor*& groupView)" << endl;
#endif

    Boolean result = FALSE;
    
    if (group_uid == (Uid*) 0)
        return FALSE;

    if (groupView)
    {
        delete groupView;
	groupView = (ReplicaDescriptor*) 0;
    }

    /*
     * If there is an action running then check cached view first.
     */

    if (AtomicAction::Current() != (AtomicAction*) 0)
    {
        RepObjStoreList* dummyPtr = (RepObjStoreList*) 0;

	if (RepObjStoreInterface::headOfList)
	{
	    groupView = RepObjStoreInterface::headOfList->getCachedView(*group_uid, dummyPtr);
	    if (groupView)
		return TRUE;
	}
    }

    if (activateDatabase())
    {
        AtomicAction A;

	A.Begin();

	groupView = new ReplicaDescriptor;
	if (NS->getView(*groupView, *group_uid, NSInterface::nodeName, NSInterface::processID))
	{
	    doRelease = result = TRUE;

	    NamingRecord* nr = new NamingRecord(*group_uid, (const TypeName) groupView->getObjectName(),
						NSInterface::processID, (char*) NSInterface::nodeName);
	    if (A.add(nr) != AtomicAction::ADDED)
	        delete nr;
	}

	if (result)
	    result = (Boolean) (A.End() == COMMITTED);
	else
	    A.Abort();
    }

    return result;
}

Boolean GroupInformation::groupSize (unsigned long& sizeOfGroup)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean GroupInformation::groupSize (unsigned long& sizeOfGroup)" << endl;
#endif

    ReplicaDescriptor* currentView = (ReplicaDescriptor*) 0;
    Boolean            result      = getGroup(currentView);

    if (result)
        sizeOfGroup = ((currentView) ? currentView->getNumber() : 0);

    if (currentView)
        delete currentView;

    return result;
}

/*
 * groupUid is the group we want information about. However, if we were
 * supplied with a group Uid at construction time then we want to keep
 * track of that.
 */

Boolean GroupInformation::groupSize (unsigned long& sizeOfGroup, const Uid& groupUid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean GroupInformation::groupSize (unsigned long& sizeOfGroup, " << groupUid << " )" << endl;
#endif

    Boolean result;
    Uid* u = group_uid;

    group_uid = new Uid(groupUid);
    result = groupSize(sizeOfGroup);

    delete group_uid;
    group_uid = u;

    return result;
}

Boolean GroupInformation::currentGroup (ReplicaDescriptor*& currentView)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean GroupInformation::currentGroup (ReplicaDescriptor*& currentView)" << endl;
#endif

    return getGroup(currentView);
}

Boolean GroupInformation::currentGroup (ReplicaDescriptor*& currentView, const Uid& groupUid)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean GroupInformation::currentGroup (ReplicaDescriptor*& currentView, " << groupUid << " )" << endl;
#endif

    Uid* u = group_uid;
    Boolean result;

    group_uid = new Uid(groupUid);
    result = getGroup(currentView);

    delete group_uid;
    group_uid = u;

    return result;
}

Boolean GroupInformation::releaseGroup ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "Boolean GroupInformation::releaseGroup ()" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;

    if (!NS)
        return FALSE;

    if (!doRelease)
        return TRUE;

    A.Begin();

    result = (NS->releaseGroup(*group_uid, NSInterface::nodeName, NSInterface::processID) == DONE);

    if (result)
    {
        if (A.End() != COMMITTED)
	    result = FALSE;
	else
	    doRelease = FALSE;
    }
    else
        A.Abort();

    return result;
}

Boolean GroupInformation::activateDatabase ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "Boolean GroupInformation::activateDatabase ()" << endl;
#endif

    int res = -1;

    if (NS)
        return TRUE;

    NS = new NSInterface(res);
    if (res == 0)
        return TRUE;
    
    delete NS;
    NS = 0;
    return FALSE;
}

#ifdef NO_INLINES
#  define GROUPINFO_CC_
#  include <Replication/GroupInformation.n>
#  undef GROUPINFO_CC_
#endif
