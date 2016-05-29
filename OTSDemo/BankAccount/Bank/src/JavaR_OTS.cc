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

JavaRecord::JavaRecord (const Uid& tranUid, int amount, int outcome,
			ReliablePort& port)
		       : cashAmount(amount),
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

JavaRecord::JavaRecord (const Uid& tranUid, ReliablePort& port)
		       : cashAmount(-1),
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

Vote JavaRecord::prepare  (CORBA_(Environment)& IT_env) THROWS((SystemException))
{
    cout << "Action preparing." << endl;

    if (!aborted)
    {
	if (!saveState(PREPARE))
	{
	    cout << "saveState failed." << endl;
	    
	    return COS_TRAN(VoteRollback);
	}
	
	if (applet->prepare())
	    return COS_TRAN(VoteCommit);
	else
	    cout << "Applet prepare message failed." << endl;

	removeState();
    }
    else
    {
	applet->prepare();
    }
    
    return COS_TRAN(VoteRollback);
}

void JavaRecord::rollback (CORBA_(Environment)& IT_env) THROWS((SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard))
{
    if (!aborted)
    {
	cout << "Action aborting." << endl;

	removeState();
    }
    
    (void) applet->abort();
}

void JavaRecord::commit (CORBA_(Environment)& IT_env) THROWS((SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard))
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
		EX_THROW(HeuristicRollback());
	    }
	}
	else
	    removeState();
    }
}

void JavaRecord::forget (CORBA_(Environment)& IT_env) THROWS((SystemException))
{
}

void JavaRecord::commit_one_phase (CORBA_(Environment)& env) THROWS((SystemException, HeuristicHazard))
{
    if (prepare(env) == COS_TRAN(VoteCommit))
	commit(env);
    else
	EX_THROW(HeuristicHazard());
}

Vote JavaRecord::prepare  () THROWS((SystemException))
{
    return this->prepare(default_environment);
}

void JavaRecord::rollback () THROWS((SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard))
{
    this->rollback(default_environment);
}

void JavaRecord::commit () THROWS((SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard))
{
    this->commit(default_environment);
}

void JavaRecord::forget () THROWS((SystemException))
{
    this->forget(default_environment);
}

void JavaRecord::commit_one_phase () THROWS((SystemException, HeuristicHazard))
{
    this->commit_one_phase(default_environment);
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

char* JavaRecord::getName () const
{
    size_t sz = 128+::strlen(outcomeDir);
    char uidString[sz];

    ::memset(uidString, '\0', sz);
    ::strcpy(uidString, outcomeDir);
    
    ostrstream output(uidString, 128);

    output << tid;

    char* returnString = ::new char[::strlen(uidString)+1];
    ::strcpy(returnString, uidString);
    
    return returnString;
}
