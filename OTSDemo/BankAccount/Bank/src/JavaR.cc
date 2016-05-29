/*
 * Copyright (C) 1995-1997,
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
#include <System/strstream.h>

#include <TCPConnection.h>

#include "Bank.h"

#ifndef JAVAR_H_
#  include "JavaR.h"
#endif

#ifndef OPCODE_H_
#  include "Opcode.h"
#endif

#ifndef APPLET_H_
#  include "Applet.h"
#endif

static const char RCSid[] = "$Id: JavaR.cc,v 1.15 1995/02/28 13:05:49 nmcl Exp $";

/*
 * Public constructors and destructors
 */

/*
 * This constructor is used to create a new instance of a
 * JavaRecord.
 */

JavaRecord::JavaRecord (const Uid& tranUid, int amount, Bank::Outcome outcome,
			TCPConnection* port)
		       : AbstractRecord(tranUid, "/StateManager/AbstractRecord/JavaRecord", ANDPERSISTENT),
			 cashAmount(amount),
			 tid(tranUid),
			 aborted(FALSE)
{
    char uidString[128];

    ::memset(uidString, '\0', 128);

    ostrstream output(uidString, 128);

    output << tid;
    
    applet = new Applet(uidString, port, outcome);
}

/*
 * Crash recovery.
 */

JavaRecord::JavaRecord (const Uid& tranUid, TCPConnection* port)
		       : AbstractRecord(tranUid),
			 cashAmount(-1),
			 tid(tranUid),
			 aborted(FALSE)
{
    if (tranUid != NIL_UID)
    {
	char uidString[128];    
	BankActionStatus state;

	::memset(uidString, '\0', 128);

	ostrstream output(uidString, 128);
	
	output << tid;
	
	if (!restoreState(state))
	{
	    aborted = TRUE;

	    applet = new Applet(port, NO_ACTION);
	}
	else
	    applet = new Applet(uidString, port, Bank::DONE);
    }
    else
    {
	aborted = TRUE;

	cout << "Received invalid Uid for recovery." << endl;
	
	applet = new Applet(port, INVALID_UID);
    }
}

JavaRecord::~JavaRecord ()
{
    if (applet)
	delete applet;
}

Outcome JavaRecord::nestedAbort ()
{
    return FINISH_OK;
}

Outcome JavaRecord::nestedCommit ()
{
    return FINISH_OK;
}

Outcome JavaRecord::nestedPrepare ()
{
    return READONLY;
}

Outcome JavaRecord::topLevelPrepare ()
{
    cout << "Action preparing." << endl;

    if (!aborted)
    {
	if (!saveState(PREPARE))
	{
	    cout << "saveState failed." << endl;
	    
	    return PREP_NOTOK;
	}

	if (applet->prepare())
	    return PREP_OK;
	else
	    cout << "Applet prepare message failed." << endl;
    
	removeState();
    }
    else
    {
	applet->prepare();
    }
    
    return PREP_NOTOK;
}

Outcome JavaRecord::topLevelAbort ()
{
    cout << "Action aborting." << endl;
    
    if (!aborted)
    {
	removeState();
    }

    (void) applet->abort();
    
    return FINISH_OK;
}

Outcome JavaRecord::topLevelCommit ()
{
    if (!aborted)
    {
	cout << "Action committing." << endl;
    
	Boolean result = applet->commit(cashAmount);

	if (!result)
	{
	    if (!saveState(COMMIT))
	    {
		cerr << "Fatal error - cannot store outcome!" << endl;
		return HEURISTIC_ROLLBACK;
	    }
	}
	else
	    removeState();
    }

    return FINISH_OK;
}

Boolean JavaRecord::saveState (BankActionStatus state)
{
    char* tidName = getName();
    ofstream output(tidName);
    Boolean result = TRUE;
    
    if (!output)
	result = FALSE;
    else
    {
	output << state << " " << cashAmount;

	result = TRUE;
    }

    ::delete [] tidName;

    return result;
}

Boolean JavaRecord::removeState ()
{
    Boolean result = FALSE;
    char* name = getName();

    if (::unlink(name))
	result = TRUE;

    ::delete [] name;

    return result;
}

Boolean JavaRecord::restoreState (BankActionStatus& state)
{
    char* tidName = getName();
    ifstream input(tidName);
    Boolean result = TRUE;
    int tmpState;

    if (!input)
	result = FALSE;
    else
    {
	input >> tmpState >> cashAmount;

	state = (BankActionStatus) tmpState;

	cout << "Recovering action which " << ((state == COMMIT) ? "committed" : "prepared") << endl;
	cout << "Cash amount " << cashAmount << endl;
	
	result = TRUE;
    }

    ::delete [] tidName;

    return result;
}

RecordType JavaRecord::typeIs () const
{
    return USER_DEF_FIRST0;
}

AnyType JavaRecord::value () const
{
    return (AnyType) cashAmount;
}

void JavaRecord::setValue (AnyType)
{
    cerr << "Error - JavaRecord::setValue called!" << endl;
}

Boolean JavaRecord::save_state (ObjectState&, ObjectType)
{
    return TRUE;
}

Boolean JavaRecord::restore_state (ObjectState&, ObjectType)
{
    return TRUE;
}

const TypeName JavaRecord::type () const
{
    return "/StateManager/AbstractRecord/JavaRecord";
}

char* JavaRecord::getName () const
{
    size_t sz = 128+::strlen(outcomeDir);
    char* uidString = new char[sz];

    ::memset(uidString, '\0', sz);

    ostrstream output(uidString, sz);

    output << outcomeDir << tid;

    char* returnString = ::new char[sz+1];
    ::strcpy(returnString, uidString);

    ::delete [] uidString;
    
    return returnString;
}

void JavaRecord::merge ( const AbstractRecord * )
{
}

void JavaRecord::alter ( AbstractRecord * )
{
}

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
