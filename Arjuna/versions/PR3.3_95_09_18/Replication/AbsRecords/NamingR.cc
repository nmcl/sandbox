/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NamingR.cc,v 1.13 1995/09/25 15:07:10 nmcl Exp $
 */

/*
 *
 * Naming Record Class
 *
 */


#ifndef SYSENT_H_
#  include <System/unistd.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
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

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef NSINTERFACE_H_
#  include <ArjServers/NSInterface.h>
#endif

#ifndef REPLICATIONR_H_
#  include "NamingR.h"
#endif

#if defined(DEBUG ) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif


static const char RCSid[] = "$Id: NamingR.cc,v 1.13 1995/09/25 15:07:10 nmcl Exp $";

/*
 * NamingRecords manage releaseing of objects at the database.
 * They are created when an object first accesses its group view
 * from the naming service. Releasing the group view is important
 * for crash recovery purposes, and attempting to release at just
 * any point within an action is not guaranteed to succeed. Therefore,
 * we do this at the last moment. If a release has been performed prior
 * to this, then it does not matter, since it is an idempotent
 * operation. [Performance penalty could occur, and perhaps in later
 * versions we will allow the removal of relevant NamingRecords if a
 * release has already been done].
 */


Boolean NamingSetUp::setUp = FALSE;


NamingSetUp::NamingSetUp ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "NamingSetUp::NamingSetUp ()" << endl;
#endif
    
    if (!setUp)
    {
        NamingRecord tr;
	AbstractRecord::addToList(NamingSetUp::create, NamingSetUp::remove, tr.type());
	setUp = TRUE;
    }
}

NamingSetUp::~NamingSetUp () {}

AbstractRecord* NamingSetUp::create ()
{
    return new NamingRecord;
}

void NamingSetUp::remove (AbstractRecord*& toDelete)
{
    delete toDelete;
    toDelete = (AbstractRecord*) 0;
}

/*
 * Public constructors and destructors
 */

/*
 * This constructor is used to create a new instance of an
 * NamingRecord.
 */

NamingRecord::NamingRecord (const Uid& gUid, const TypeName tn, pid_t pID, char* nodeName)
			   : AbstractRecord(gUid, tn, ANDPERSISTENT),
			     NS(0),
			     processID(pID),
			     haveReleased(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "NamingRecord::NamingRecord(" << tn
                 << ", " << gUid << ")" << endl;
#endif

    groupUid = new Uid(gUid);
    objName = ::new char[::strlen(tn)+1];
    ::memset(objName, '\0', ::strlen(tn)+1);
    ::memcpy(objName, tn, ::strlen(tn));
    hostName = ::new char[::strlen(nodeName)+1];
    ::memset(hostName, '\0', ::strlen(nodeName)+1);
    ::memcpy(hostName, nodeName, ::strlen(nodeName));
}

NamingRecord::NamingRecord ()
			   : AbstractRecord(),
			     groupUid(0),
			     NS(0),
			     objName(0),
			     processID(-1),
			     hostName(0),
			     haveReleased(FALSE)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "NamingRecord::NamingRecord ()" << endl;
#endif
}

NamingRecord::~NamingRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "NamingRecord::~NamingRecord () for " << order() << endl;
#endif

    if (NS)
	delete NS;
    if (groupUid)
	delete groupUid;
    if (hostName)
#ifndef GCC_ARRAY_DELETE_BUG	
	::delete [] hostName;
#else
	::delete hostName;
#endif
    if (objName)
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] objName;
#else
        ::delete objName;
#endif
}

/*
 * Public virtual functions and operators
 */

RecordType NamingRecord::typeIs () const
{
    return NAMING;
}

AnyType NamingRecord::value () const
{
    return (AnyType) processID;
}

void NamingRecord::setValue (AnyType)
{
    error_stream << WARNING << "NamingRecord::set_value() : called." << endl;
}

Boolean NamingRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "NamingRecord::nestedAbort() for " << order() << endl;
#endif

    return TRUE;
}

Boolean NamingRecord::nestedCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "NamingRecord::nestedCommit() for " << order() << endl;
#endif

    return TRUE;
}

PrepareOutcome NamingRecord::nestedPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "NamingRecord::nestedPrepare() for " << order() << endl;
#endif

    return PREP_OK;
}

Boolean NamingRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "NamingRecord::topLevelAbort() for " << order() << endl;
#endif

    return nestedAbort();
}

Boolean NamingRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "NamingRecord::topLevelCommit() for " 
		 << order() << endl;
#endif

    if (!haveReleased)
	return FALSE;              /* Get crash recovery to fix this for us. */
     
    return TRUE;
}

Boolean NamingRecord::activateDatabase ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PRIVATE;
    debug_stream << "Boolean NamingRecord::activateDatabase ()" << endl;
#endif

    int res = -1;
    
    NS = new NSInterface(res);
    
    if (res == 0)
	return TRUE;
    
    if (NS)
	delete NS;
    
    return FALSE;
}
    
PrepareOutcome NamingRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "NamingRecord::topLevelPrepare() for " 
		 << order() << endl;
#endif

    if (activateDatabase())
    {
	if (NS->releaseGroup(*groupUid, hostName, processID) == DONE)
	    haveReleased = TRUE;
    }

    if (!haveReleased)
	error_stream << "NamingRecord::topLevelPrepare - could not release group." << endl;

    return PREP_OK;
}

/*
 * Since this record only gets added for "normal" actions, if they abort then the group
 * will be automatically released at the naming service.
 */

Boolean NamingRecord::propagateOnAbort () const
{
    return FALSE;
}

Boolean NamingRecord::restore_state ( ObjectState& os, ObjectType )
{
    Boolean result = FALSE;

    if (objName)
    {
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] objName;
#else
	::delete objName;
#endif
	objName = (char*) 0;
    }
    if (hostName)
    {
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] hostName;
#else
	::delete hostName;
#endif
	hostName = (char*) 0;
    }
    if (!groupUid)
	groupUid = new Uid();
    
    result = groupUid->unpack(os) && os.unpack(objName) && os.unpack(processID);
    result = result && os.unpack(hostName) && os.unpack(haveReleased);
    
    return result;
}

Boolean NamingRecord::doSave () const
{
    return TRUE;
}

Boolean NamingRecord::save_state ( ObjectState& os, ObjectType )
{
    Boolean result = groupUid->pack(os) && os.packMappedString(objName) && os.pack(processID);

    result = result && os.packMappedString(hostName) && os.pack(haveReleased);

    return result;
}

ostream& NamingRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);
    strm << "NamingRecord for group " << *groupUid << " and type "
         << objName << endl;
    return strm;
}

const TypeName NamingRecord::type () const
{
    return ("/StateManager/AbstractRecord/NamingRecord");
}

void NamingRecord::merge ( const AbstractRecord * )
{
}

void NamingRecord::alter (AbstractRecord *)
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

Boolean NamingRecord::shouldAdd ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean NamingRecord::shouldAlter (const AbstractRecord*) const
{
    return FALSE;
}

Boolean NamingRecord::shouldMerge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean NamingRecord::shouldReplace ( const AbstractRecord * ) const
{
    return FALSE;
}
