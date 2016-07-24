/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RegisterGroup.cc,v 1.13 1995/09/30 10:22:32 nmcl Exp $
 */

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef ATOMICA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef REGISTERGROUPS_H_
#  include <Replication/RegisterGroup.h>
#endif

#ifndef REPLICACREATE_H_
#  include <Replication/ReplicaCreate.h>
#endif

#ifndef REPDESC_H_
#  include <ArjServers/RepDesc_stub.h>
#endif

#ifndef NAMINGR_H_
#  include "NamingR.h"
#endif


RegisterGroup::RegisterGroup (const Uid& groupUid)
			     : group_uid(&groupUid),
			       rd1(0),
			       rd2(0),
			       NS(0),
			       doRelease(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RegisterGroup::RegisterGroup ( " << groupUid << " )" << endl;
#endif
}

RegisterGroup::RegisterGroup (const Uid& groupUid, ReplicaDescriptor* toUse)
			     : group_uid(&groupUid),
			       rd1(toUse),
			       rd2(0),
			       NS(0),
			       doRelease(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RegisterGroup::RegisterGroup ( " << groupUid << " )" << endl;
#endif
}

RegisterGroup::RegisterGroup (const Uid& groupUid, ReplicaCreate* toUse)
			     : group_uid(&groupUid),
			       rd1(toUse->rd),
			       rd2(0),
			       NS(0),
			       doRelease(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RegisterGroup::RegisterGroup ( " << groupUid << " )" << endl;
#endif
}

RegisterGroup::~RegisterGroup ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "RegisterGroup::~RegisterGroup ()\n" << flush;
#endif

    /* Make sure we release the group if we have locked it. */

    if (doRelease)
        (void) releaseGroup();
    if (rd2)
	delete rd2;
    if (NS)
	delete NS;
}

Boolean RegisterGroup::releaseGroup ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PROTECTED;
    debug_stream << "Boolean RegisterGroup::releaseGroup ()" << endl;
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

Boolean RegisterGroup::lockGroup (LockMode mode)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean RegisterGroup::lockGroup ( " << ((mode == READ) ? "READ" : "WRITE") << " )" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    ReplicaDescriptor dummy;

    if (!NS)
        return FALSE;

    A.Begin();

    if (activateDatabase())
        result = (NS->getView(dummy, *group_uid, NSInterface::nodeName, NSInterface::processID) == DONE);

    if (result)
    {
	doRelease = TRUE;
	NamingRecord* nr = new NamingRecord(*group_uid, (const TypeName) dummy.getObjectName(), NSInterface::processID, (char*) NSInterface::nodeName);
	if (A.add(nr) != AtomicAction::ADDED)
	    delete nr;
        if (A.End() != COMMITTED)
	    result = FALSE;
    }
    else
        A.Abort();

    return result;
}

Boolean RegisterGroup::modifyGroup (ReplicaCreate* rc)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean RegisterGroup::modifyGroup (ReplicaCreate* rc)" << endl;
#endif

    // check we have something to modify.

    if (rc->rd == (ReplicaDescriptor*) 0)
	return FALSE;

    return modifyGroup(*rc->rd);
}

Boolean RegisterGroup::modifyGroup (const ReplicaDescriptor& toUse)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean RegisterGroup::modifyGroup ()" << endl;
#endif

    Boolean result = FALSE;
    AtomicAction A;

    A.Begin();

    if (activateDatabase())
    {
	ReplicaDescriptor dummyRD;

        /* Remove old group and replace it with new group. */

        result = (NS->deleteReplicas(dummyRD, *group_uid, TRUE) == DONE);
	if (result)
	    result = (NS->addReplicas(toUse, *group_uid) == DONE);
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

Boolean RegisterGroup::activateDatabase ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PRIVATE;
    debug_stream << "Boolean RegisterGroup::activateDatabase ()" << endl;
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

Boolean RegisterGroup::removeGroup ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean RegisterGroup::removeGroup ()" << endl;
#endif

    AtomicAction A;
    ReplicaDescriptor dummy;
    Boolean result = FALSE;

    A.Begin();

    if (activateDatabase())
    {
        if (NS->deleteReplicas(dummy, *group_uid, TRUE) == DONE)
	    result = TRUE;
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

/*
 * This method will only record those replicas which are marked as being
 * available. In addition, the db will only add those replicas which are
 * not already present.
 */

Boolean RegisterGroup::recordGroup ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_REPLICATION << VIS_PUBLIC;
    debug_stream << "Boolean RegisterGroup::recordGroup ()" << endl;
#endif

    Boolean result = FALSE;
    ReplicaDescriptor *tmp1 = rd1, *tmp2 = 0;
    AtomicAction A;

    // check we have something to record.

    if (rd1 == (ReplicaDescriptor*) 0)
	return FALSE;
    
    A.Begin();

    if (activateDatabase())
    {
        while (tmp1)
	{
	    if (tmp1->getActive())
	    {
	        tmp2 = new ReplicaDescriptor;
		tmp2->setHost(tmp1->getHost());
		tmp2->setObjectName(tmp1->getObjectName());
		tmp2->setStoreRoot(tmp1->getStoreRoot());
		tmp2->setUid(tmp1->getUid());
		tmp2->setActive(TRUE);
		tmp2->setGroupUid(*group_uid);
		tmp2->next = rd2;
		rd2 = tmp2;
	    }

	    tmp1 = tmp1->next;
	}

	result = (NS->addReplicas(*rd2, *group_uid) == DONE);
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


#ifdef NO_INLINES
#  define REGGROUP_CC_
#  include <Replication/RegisterGroup.n>
#  undef REGGROUP_CC_
#endif
