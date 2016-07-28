/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ReplicationR.cc,v 1.17 1996/10/09 13:14:38 nmcl Exp $
 */

/*
 *
 * Replication Record Class
 *
 */

#ifndef SYSENT_H_
#  include <System/unistd.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef NETDB_H_
#   include <System/netdb.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#if defined(DEBUG ) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef REPOBJSTOREIF_H_
#  include <Replication/RepObjStoreIF.h>
#endif

#ifndef REPOBJSTORE_H_
#  include <Replication/RepObjStore.h>
#endif

#ifndef REPLICATIONR_H_
#  include "ReplicationR.h"
#endif


static const char RCSid[] = "$Id: ReplicationR.cc,v 1.17 1996/10/09 13:14:38 nmcl Exp $";

/*
 * ReplicationRecords manage replication of objects. They are created when
 * an object first accesses its state within an atomic action.
 * They guarantee that when the top-level action ends (commits or aborts) all
 * information pertaining to the relevant replica group is re-initialised.
 * This means that if the object is used again within another top-level
 * it will have to re-obtain this information, which is what is required.
 */


Boolean ReplnSetUp::setUp = FALSE;


ReplnSetUp::ReplnSetUp ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ReplnSetUp::ReplnSetUp ()" << endl;
#endif
    
    if (!setUp)
    {
        ReplicationRecord tr;
	AbstractRecord::addToList(ReplnSetUp::create, ReplnSetUp::remove, tr.type());
	setUp = TRUE;
    }
}

ReplnSetUp::~ReplnSetUp () {}

AbstractRecord* ReplnSetUp::create ()
{
    return new ReplicationRecord;
}

void ReplnSetUp::remove (AbstractRecord*& toDelete)
{
    delete toDelete;
    toDelete = (AbstractRecord*) 0;
}


/*
 * Public constructors and destructors
 */

/*
 * This constructor is used to create a new instance of an
 * ReplicationRecord.
 */

ReplicationRecord::ReplicationRecord ( RepObjStoreInterface* rpOS, const Uid& gUid,
				       const TypeName tn, Boolean usingStore )
                                     : AbstractRecord(gUid, tn, ANDPERSISTENT),
				       objectAddr(rpOS),
				       haveDeactivated(usingStore)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ReplicationRecord::ReplicationRecord(" << tn
                 << ", " << gUid << ")" << endl;
#endif

    ObjectStore::reference(objectAddr);
    groupUid = new Uid(gUid);
    objName = ::new char[::strlen(tn)+1];
    ::memset(objName, '\0', ::strlen(tn)+1);
    ::memcpy(objName, tn, ::strlen(tn));
}

ReplicationRecord::ReplicationRecord ()
                                      : AbstractRecord(),
                                        groupUid(0),
                                        objectAddr(0),
                                        objName(0),
                                        haveDeactivated(FALSE)

{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "ReplicationRecord::ReplicationRecord ()" << endl;
#endif
}

ReplicationRecord::~ReplicationRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ReplicationRecord::~ReplicationRecord() for " << order() << endl;
#endif

    if (groupUid)
	delete groupUid;
    if (objName)
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] objName;
#else
        ::delete objName;
#endif

    if (objectAddr)
    {
	ObjectStore* toDelete = (ObjectStore*) objectAddr;
	ObjectStore::destroy(toDelete);
	objectAddr = NULL;
    }
}

/*
 * Public virtual functions and operators
 */

RecordType ReplicationRecord::typeIs () const
{
    return REPLICATION;
}

AnyType ReplicationRecord::value () const
{
    return (AnyType) objectAddr;
}

void ReplicationRecord::setValue (AnyType)
{
    error_stream << WARNING << "ReplicationRecord::set_value() : called." << endl;
}

Boolean ReplicationRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ReplicationRecord::nestedAbort() for " << order() << endl;
#endif

    return TRUE;
}

Boolean ReplicationRecord::nestedCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ReplicationRecord::nestedCommit() for " << order() << endl;
#endif

    return TRUE;
}

PrepareOutcome ReplicationRecord::nestedPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ReplicationRecord::nestedPrepare() for " << order() << endl;
#endif

    return PREP_OK;
}

Boolean ReplicationRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ReplicationRecord::topLevelAbort() for " << order() << endl;
#endif

    /*
     * Try to cleanup. Since we do not know where in the action we aborted,
     * this may not do anything, i.e., there may not be any state to remove.
     *
     * If we have aborted due to a replica failure, this call will attempt to
     * recreate the replica group, which may result in a performance penalty.
     */

    if (objectAddr)
	(void) objectAddr->remove_uncommitted(*groupUid, objName);

    /* Try to deactivate replication protocol. */
    
    (void) topLevelPrepare();
    return topLevelCommit();
}

Boolean ReplicationRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ReplicationRecord::topLevelCommit() for " 
		 << order() << endl;
#endif

    /*
     * Must clean up object store as group view may have changed at db
     * the next time we use this object, i.e., the object store state
     * must be re-initialized when the next top-level action starts.
     */

    if (objectAddr)
	objectAddr->cleanUp();

    if (!haveDeactivated)
	return FALSE;                /* Get crash recovery to fix this for us. */

    return TRUE;
}

PrepareOutcome ReplicationRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "ReplicationRecord::topLevelPrepare() for " 
		 << order() << endl;
#endif

    /*
     * Deactivate this object store. This will contact that database
     * and decrement the use count for this user. The actual object
     * store instance will not be removed until the interface is deleted,
     * or it binds to another instance.
     */
    
    if ((objectAddr) && (!haveDeactivated))
	haveDeactivated = objectAddr->deactivate();

    if (!haveDeactivated)
	error_stream << "ReplicationRecord::topLevelPrepare - could not deactivate object store." << endl;

    return PREP_OK;
}

Boolean ReplicationRecord::propagateOnAbort () const
{
    return TRUE;
}

Boolean ReplicationRecord::restore_state ( ObjectState& os, ObjectType )
{
    Boolean result = FALSE;
    char* hostName = 0;
    pid_t pId = -1;
    
    if (!groupUid)
	groupUid = new Uid(NIL_UID);
    if (objName)
    {
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] objName;
#else
        ::delete objName;
#endif    
	objName = 0;
    }
	
    result = os.unpack(hostName) && os.unpack(pId) && groupUid->unpack(os) && os.unpack(objName) && os.unpack(haveDeactivated);
    
    if (result)
    {
	if (!objectAddr)
	    objectAddr = new RepObjStoreInterface();

	if (!objectAddr->instanceRPOS)
	    objectAddr->instanceRPOS = new ReplicatedPersistentObjectStore;
	else
	    objectAddr->instanceRPOS->cleanUp();
	
	if ((!haveDeactivated) && (objectAddr->instanceRPOS->activateDatabase()))
	{
	    objectAddr->instanceRPOS->doRelease = TRUE;
	    objectAddr->instanceRPOS->myData.group_uid = new Uid(*groupUid);
	    objectAddr->instanceRPOS->myData.pid = pId;
	}
	else
	    result = FALSE;
    }
    
    return result;
}

Boolean ReplicationRecord::doSave () const
{
    return TRUE;
}

Boolean ReplicationRecord::save_state ( ObjectState& os, ObjectType )
{
    Boolean result = FALSE;
    char hostName[MAXHOSTNAMELEN+1];
    pid_t pId = getpid();
    
    ::memset(hostName, '\0', MAXHOSTNAMELEN+1);
    ::gethostname(hostName, MAXHOSTNAMELEN);
    
    result = os.packMappedString(hostName) && os.pack(pId) && groupUid->pack(os) && os.packMappedString(objName) && os.pack(haveDeactivated);

    return result;
}

ostream& ReplicationRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);
    strm << "ReplicationRecord for group " << *groupUid << " and type "
         << objName << endl;
    return strm;
}

const TypeName ReplicationRecord::type () const
{
    return ("/StateManager/AbstractRecord/ReplicationRecord");
}

void ReplicationRecord::merge ( const AbstractRecord * )
{
}

void ReplicationRecord::alter (AbstractRecord *)
{
}

/*
 * should_merge and should_replace are invoked by the record list manager
 * to determine if two records should be merged together or if the
 * 'newer' should replace the older.
 * shouldAdd determines if the new record should be added in addition
 * to the existing record and is currently only invoked if both of
 * should_merge and should_replace return FALSE
 * Default implementations here always return FALSE - ie new records
 * do not override old
 */

Boolean ReplicationRecord::shouldAdd ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean ReplicationRecord::shouldAlter (const AbstractRecord*) const
{
    return FALSE;
}

Boolean ReplicationRecord::shouldMerge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean ReplicationRecord::shouldReplace ( const AbstractRecord * ) const
{
    return FALSE;
}
