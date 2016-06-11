/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: JavaR.cc,v 1.15 1995/02/28 13:05:49 nmcl Exp $
 */

/*
 *
 * Java Record Class
 *
 */

#include <System/errno.h>
#include <System/unistd.h>
#include <System/sys/param.h>
#include <System/protos.h>
#include <System/netdb.h>
#include <System/sys/socket.h>
#include <System/netinet/in.h>
#include <System/string.h>
#include <System/iostream.h>
#include <System/fstream.h>
#include <System/memory.h>
#include <System/stdio.h>

#if defined(DEBUG ) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef JAVAR_H_
#  include "JavaR.h"
#endif

#ifndef OPCODE_H_
#  include "Opcode.h"
#endif

static const char RCSid[] = "$Id: JavaR.cc,v 1.15 1995/02/28 13:05:49 nmcl Exp $";


/*
 * Public constructors and destructors
 */

/*
 * This constructor is used to create a new instance of a
 * JavaRecord.
 */

JavaRecord::JavaRecord (const Uid& javaUid, const TypeName tn, long amount,
			const char* name, int s)
		       : AbstractRecord(javaUid, tn, ANDPERSISTENT),
			 cashAmount(amount),
			 sock(s),
			 tid(javaUid)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "JavaRecord::JavaRecord(" << tn
                 << ", " << gUid << ")" << endl;
#endif

    id = ::new char[::strlen(name)+1];
    ::strcpy(id, name);
}

JavaRecord::JavaRecord ()
		       : AbstractRecord(),
			 id(0),
			 cashAmount(0),
			 sock(-1),
			 tid(NIL_UID)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ABSTRACT_REC << VIS_PROTECTED;
    debug_stream << "JavaRecord::JavaRecord ()" << endl;
#endif
}

JavaRecord::~JavaRecord ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "JavaRecord::~JavaRecord() for " << order() << endl;
#endif

    if (id)
	::delete [] id;
}

/*
 * Public virtual functions and operators
 */

RecordType JavaRecord::typeIs () const
{
    return USER_DEF_LAST0;
}

AnyType JavaRecord::value () const
{
    return (AnyType) id;
}

void JavaRecord::setValue (AnyType)
{
    error_stream << WARNING << "JavaRecord::set_value() : called." << endl;
}

Boolean JavaRecord::nestedAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "JavaRecord::nestedAbort() for " << order() << endl;
#endif

    return TRUE;
}

Boolean JavaRecord::nestedCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "JavaRecord::nestedCommit() for " << order() << endl;
#endif

    return TRUE;
}

PrepareOutcome JavaRecord::nestedPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "JavaRecord::nestedPrepare() for " << order() << endl;
#endif

    return PREP_OK;
}

Boolean JavaRecord::topLevelAbort ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "JavaRecord::topLevelAbort() for " << order() << endl;
#endif

    (void) transmitOutcome(OP_ABORT);
    return TRUE;
}

Boolean JavaRecord::topLevelCommit ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "JavaRecord::topLevelCommit() for " 
		 << order() << endl;
#endif

    Boolean result = transmitOutcome(OP_COMMIT);

    if (!result)
    {
	ofstream output(outcomeFile, ios::app);

	if (!output)
	    cerr << "Fatal error - cannot store outcome!" << endl;
	else
	    output << id << " " << tid << " " << cashAmount << endl;
    }

    return result;
}

PrepareOutcome JavaRecord::topLevelPrepare ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ABSTRACT_REC << VIS_PUBLIC;
    debug_stream << "JavaRecord::topLevelPrepare() for " 
		 << order() << endl;
#endif

    return ((transmitOutcome(OP_PREPARE)) ? PREP_OK : PREP_NOTOK);
}

Boolean JavaRecord::propagateOnAbort () const
{
    return TRUE;
}

Boolean JavaRecord::save_state ( ObjectState& os, ObjectType )
{
    Boolean res;
    
    if (id)
	res = os.pack(1) && os.packMappedString(id);
    else
	res = os.pack(0);

    if (res)
    {
	res = tid.pack(os);

	if (res)
	    return os.pack(cashAmount);
    }
    
    return FALSE;
}

Boolean JavaRecord::restore_state ( ObjectState& os, ObjectType )
{
    int ptr = -1;
    Boolean res = os.unpack(ptr);

    if (id)
    {
	::delete [] id;
	id = (char*) 0;
    }
    
    if (ptr == 1)
	res = os.unpack(id);

    if (res)
	res = os.unpack(cashAmount);

    if (res)
	res = tid.unpack(os);
    
    return res;
}

Boolean JavaRecord::doSave () const
{
    return TRUE;
}

ostream& JavaRecord::print ( ostream& strm ) const
{
    AbstractRecord::print(strm);
    strm << "JavaRecord < " << id << ", " << cashAmount << " >" << endl;
    return strm;
}

const TypeName JavaRecord::type () const
{
    return ("/StateManager/AbstractRecord/JavaRecord");
}

void JavaRecord::merge ( const AbstractRecord * )
{
}

void JavaRecord::alter (AbstractRecord *)
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

Boolean JavaRecord::shouldAdd ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean JavaRecord::shouldAlter (const AbstractRecord*) const
{
    return FALSE;
}

Boolean JavaRecord::shouldMerge ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean JavaRecord::shouldReplace ( const AbstractRecord * ) const
{
    return FALSE;
}

Boolean JavaRecord::transmitOutcome (long outcome)
{
    long length = REPLY_SIZE;
    char* result = ::new char[length];
    char* reply = ::new char[length];
    Boolean res = FALSE;

    ::memset(result, '\0', length);

    /*
     * Send back money regardless of operation!
     */

    cout << "opcode is " << outcome << endl;
    
    if (outcome == OP_COMMIT)
	sprintf(result, "%i\n%i\n", outcome, cashAmount);
    else
	sprintf(result, "%i\n", outcome);

    cout << "Sending " << result << endl;
    
    if (send(sock, result, ::strlen(result), 0) == ::strlen(result))
    {
	/*
	 * We're not really bothered about what the reply is, only
	 * that we get one.
	 */

	cout << "Send reply ok" << endl;
	
	if (recv(sock, reply, length, 0) > 0)
	{
	    res = TRUE;
	    cout << "Received reply ok" << endl;
	}
    }
    else
	cout << "Send error. errno " << errno << endl;

    ::delete [] result;
    ::delete [] reply;
    
    return res;
}
