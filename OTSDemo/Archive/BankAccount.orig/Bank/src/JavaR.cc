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

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
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

JavaRecord::JavaRecord (const Uid& javaUid, int amount, int outcome,
			const char* name, ReliablePort& port)
		       : cashAmount(amount),
			 tid(javaUid)
{
    id = ::new char[::strlen(name)+1];
    ::strcpy(id, name);

    applet = new Applet(port, outcome);
}

JavaRecord::~JavaRecord ()
{
    if (id)
	::delete [] id;

    if (applet)
	delete applet;
}

Vote JavaRecord::prepare  (CORBA_(Environment)& IT_env) THROWS((SystemException))
{
    cout << "Action preparing." << endl;
    
    if (applet->prepare())
	return COS_TRAN(VoteCommit);
    else
	return COS_TRAN(VoteRollback);
}

void JavaRecord::rollback (CORBA_(Environment)& IT_env) THROWS((SystemException, HeuristicCommit, HeuristicMixed, HeuristicHazard))
{
    cout << "Action aborting." << endl;
    
    (void) applet->abort();
}

void JavaRecord::commit (CORBA_(Environment)& IT_env) THROWS((SystemException, NotPrepared, HeuristicRollback, HeuristicMixed, HeuristicHazard))
{
    cout << "Action committing." << endl;
    
    Boolean result = applet->commit(cashAmount);

    if (!result)
    {
	ofstream output(outcomeFile, ios::app);

	if (!output)
	{
	    cerr << "Fatal error - cannot store outcome!" << endl;
	    EX_THROW(HeuristicRollback());
	}
	else
	    output << id << " " << tid << " " << cashAmount << endl;
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
