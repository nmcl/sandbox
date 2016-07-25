/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: NameServ.cc
 */

#define STATEMANAGER_SERVER
#define LOCKMANAGER_SERVER
#define NAMESERVER_SERVER

#include "NameServ_stub.h"
#include "GroupView.h"
#include "ServerView.h"

#ifndef IOSTREAM_H_
#include <System/iostream.h>
#endif

#ifndef STDLIB_H_
#include <System/stdlib.h>
#endif

#ifndef MALLOC_H_
#include <System/malloc.h>
#endif

#ifndef MEMORY_H_
#include <System/memory.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef BOOLEAN_H_
#include <Common/Boolean.h>
#endif

#ifndef BUFFER_H_
#include <Common/Buffer.h>
#endif

#ifndef ATOMICA_H_
#include <Arjuna/AtomicA.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

NameServer::NameServer (unsigned short& res) : LockManager(ANDPERSISTENT)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "NameServer::NameServer (unsigned short& res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	gv_ok = FALSE;
	sv_ok = FALSE;
	GV = 0;
	SV = 0;

	if (A.End() == COMMITTED)
	    res = 0;
    }
    else
	A.Abort();
}

NameServer::NameServer (const Uid& u, unsigned short& res) : LockManager(u)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "NameServer::NameServer (const Uid& u, unsigned short& res)\n" << flush;
#endif

    AtomicAction A;

    A.Begin();

    gv_ok = FALSE;
    sv_ok = FALSE;
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
    debug_stream << DESTRUCTORS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "NameServer::~NameServer ()\n" << flush;
#endif

    terminate();

    if (GV)
	delete GV;

    if (SV)
	delete SV;
}

Boolean NameServer::ImposeNewGroupViewState (ObjectState& os, int& ret)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::ImposeNewGroupViewState (ObjectState& os, int& ret)" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;

    A.Begin();

    ret = 0;
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	{
	    result = GV->ImposeState(os);
	    
	    if (!result)
		A.Abort();
	    else
		result = (Boolean) (A.End() == COMMITTED);
	}
	else
	    A.Abort();
    }
    else
	A.Abort();
    
    return result;
}

Boolean NameServer::GetGroupViewState (ObjectState& os, int& ret, unsigned long& version)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "NameServer::GetGroupViewState (ObjectState& os, unsigned long& version)" << endl;
#endif

    AtomicAction A;
    Boolean result = FALSE;
    
    A.Begin();
    
    ret = 0;

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	{
	    result = GV->GetState(os, version);
	    
	    if (!result)
		A.Abort();
	    else
		result = (Boolean) (A.End() == COMMITTED);
	}
	else
	    A.Abort();
    }
    else
	A.Abort();
    
    return result;
}

Boolean NameServer::Add_Replicas (const ReplicaDescriptor& objects, const Uid& group, int& ret, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::Add_Replica\n" << flush;
#endif

    AtomicAction A;
    Boolean result, opcode;

    A.Begin();

    opcode = FALSE;
    ret = 0;

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	{
	    result = GV->Add_Replicas(objects, group, v_number);
	    
	    if (!result)
		A.Abort();
	    else
	    {
		if (A.End() == COMMITTED)
		    opcode = TRUE;
	    }
	}
	else
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		int res = -1;

		GV = new GroupView(res);

		if (res != 0)
		    A.Abort();
		else
		{
		    gv_ok = TRUE;
		    result = GV->Add_Replicas(objects, group, v_number);

		    if (!result)
			A.Abort();
		    else
		    {
			if (A.End() == COMMITTED)
			    opcode = TRUE;
		    }
		}
	    }
	    else
		A.Abort();
	}
    }
    else
	A.Abort();

    return opcode;
}

Boolean NameServer::Delete_Replicas (const Uid& group, const ReplicaDescriptor& objects, int& res,
				     unsigned long& v_number, const Boolean deleteAll)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::Delete_Replicas\n" << flush;
#endif

    AtomicAction A;
    Boolean opcode, result;
    int left = 0;

    A.Begin();

    opcode = FALSE;
    res = 0;

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (gv_ok)
	{
	    result = GV->Delete_Replicas(objects, group, deleteAll, left, v_number);

	    if (result)
	    {
		if (A.End() == COMMITTED)
		    opcode = TRUE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();

    return opcode;
}

Boolean NameServer::Release_Group (const Uid& group, int& res, unsigned long& v_number)
{
    AtomicAction A;
    Boolean result, opcode = FALSE;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::Release_Group (const Uid& group, int& res, unsigned long& v_number)\n" << flush;
#endif

    res = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (gv_ok)
	{
	    result = GV->Release_Group(group, v_number);

	    if (result)
	    {
		if (A.End() == COMMITTED)
		    opcode = TRUE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();

    return opcode;
}

Boolean NameServer::Traverse_Database (ReplicaDescriptor& objects, Uid& group,
				       int& position, int& ret, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::Traverse_Database (...)" << endl;
#endif
    
    AtomicAction A;
    Boolean opcode = FALSE, result;

    ret = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (gv_ok)
	{
	    result = GV->Traverse_Database(objects, group, position, v_number);

	    if (result)
	    {
		if (A.End() == COMMITTED)
		    opcode = TRUE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();

    return opcode;
}

Boolean NameServer::Get_View (ReplicaDescriptor& objects, const Uid& group, int& ret,
			      unsigned long& v_number, const LockMode mode)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::Get_View\n" << flush;
#endif

    AtomicAction A;
    Boolean result, opcode = FALSE;

    ret = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (gv_ok)
	{
	    result = GV->Get_View(objects, group, v_number, mode);

	    if (result)
	    {
		if (A.End() == COMMITTED)
		    opcode = TRUE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();

    return opcode;
}

Boolean NameServer::GetExclusiveGroupView (ReplicaDescriptor& objects, const Uid& group, int& ret,
					   unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::GetExclusiveGroupView" << endl;
#endif

    AtomicAction A;
    Boolean result, opcode = FALSE;
    
    ret = 0;
    
    A.Begin();
    
    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (gv_ok)
	{
	    result = GV->GetExclusiveGroupView(objects, group, v_number);
	    
	    if (result)
	    {
		if (A.End() == COMMITTED)
		    opcode = TRUE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();
    
    return opcode;
}

Boolean NameServer::ChangeReplicaStatus (const ReplicaDescriptor& objects, const Uid& group,
					 const Boolean state, int& res, unsigned long& v_number)
{
    AtomicAction A;
    Boolean opcode = FALSE, result;

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::ChangeReplicaStatus(...)\n" << flush;
#endif

    A.Begin();

    res = 0;

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (gv_ok)
	{
	    result = GV->ChangeReplicaStatus(objects, group, state, v_number);

	    if (result)
	    {
		if (A.End() == COMMITTED)
		    opcode = TRUE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();

    return opcode;
}

Boolean NameServer::NeedsUpdating (const Uid& recovered, ReplicaDescriptor& info,
				   int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::NeedsUpdating (...)" << endl;
#endif
    
    AtomicAction A;
    Boolean opcode = FALSE, result;

    res = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (gv_ok)
	{
	    result = GV->NeedsUpdating(recovered, info, v_number);

	    if (result)
	    {
		if (A.End() == COMMITTED)
		    opcode = TRUE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();

    return opcode;
}

Boolean NameServer::GetNodeUpdateList (ReplicaDescriptor& toupdate, ReplicaDescriptor& unknownList,
				       string hostname, Boolean& unknownState, int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::GetNodeUpdateList (...)" << endl;
#endif
    
    AtomicAction A;
    Boolean result, opcode = FALSE;

    res = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (gv_ok)
	{
	    result = GV->GetNodeUpdateList(toupdate, unknownList, hostname, unknownState, v_number);

	    if (result)
	    {
		if (A.End() == COMMITTED)
		    opcode = TRUE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();

    return opcode;
}

Boolean NameServer::CheckAndSet (const Uid& group, int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::CheckAndSet\n" << flush;
#endif

    AtomicAction A;
    Boolean opcode, result;

    res = 0;
    opcode = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->CheckAndSet(group, v_number);

	if (!result)
	    A.Abort();
	else
	{
	    if (A.End() == COMMITTED)
		opcode = TRUE;
	}
    }
    else
	A.Abort();

    return opcode;
}

Boolean NameServer::AlterLocation (const Uid& group, const Uid& replica,
				   string location, int& res, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::AlterLocation (...)" << endl;
#endif
    
    AtomicAction A;
    Boolean opcode = FALSE, result;

    res = 0;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (GV)
	    result = GV->AlterLocation(group, replica, location, v_number);

	if (!result)
	    A.Abort();
	else
	{
	    if (A.End() == COMMITTED)
		opcode = TRUE;
	}
    }
    else
	A.Abort();

    return opcode;
}

void NameServer::StoreName (Boolean& done, Buffer key_buff, Buffer data_buff, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void NameServer::StoreName (...)" << endl;
#endif
    
    AtomicAction A;

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (sv_ok)
	{
	    SV->StoreName(done, key_buff, data_buff, v_number);
	    
	    if (A.End() != COMMITTED)
		done = FALSE;
	}
	else
	{
	    if (setlock(new Lock(WRITE), 0) == GRANTED)
	    {
		int res = -1;

		SV = new ServerView(res);
		sv_ok = TRUE;

		if (res != 0)
		    A.Abort();
		else
		{
		    SV->StoreName(done, key_buff, data_buff, v_number);

		    if (done)
		    {
			if (A.End() != COMMITTED)
			    done = FALSE;
		    }
		    else
			A.Abort();
		}
	    }
	    else
		A.Abort();
	}
    }
    else
	A.Abort();
}

void NameServer::FetchName (Boolean& done, Buffer key_buff, Buffer& data_buff, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void NameServer::FetchName (...)" << endl;
#endif
    
    AtomicAction A;

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (sv_ok)
	{
	    SV->FetchName(done, key_buff, data_buff, v_number);

	    if (done)
	    {
		if (A.End() != COMMITTED)
		    done = FALSE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();
}

void NameServer::RemoveName (Boolean& done, Buffer key_buff, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void NameServer::RemoveName (...)" << endl;
#endif
    
    AtomicAction A;
    int left = 0;

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(WRITE), 0) == GRANTED)
    {
	if (sv_ok)
	{
	    SV->RemoveName(done, key_buff, left, v_number);

	    if (done)
	    {
		if (A.End() != COMMITTED)
		    done = FALSE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();
}

void NameServer::ReplaceName (Boolean& done, Buffer key_buff, Buffer data_buff, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void NameServer::ReplaceName (Boolean& done, Buffer key_buff, Buffer data_buff, unsigned long& v_number)" << endl;
#endif
    
    AtomicAction A;

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (sv_ok)
	{
	    SV->ReplaceName(done, key_buff, data_buff, v_number);

	    if (done)
	    {
		if (A.End() != COMMITTED)
		    done = FALSE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();
}

void NameServer::GetNextName (Boolean& done, Buffer& key_buff, int& position, unsigned long& v_number)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void NameServer::GetNextName (Boolean& done, Buffer& key_buff, int& position, unsigned long& v_number)" << endl;
#endif
    
    AtomicAction A;

    done = FALSE;

    A.Begin();

    if (setlock(new Lock(READ), 0) == GRANTED)
    {
	if (sv_ok)
	{
	    SV->GetNextName(done, key_buff, position, v_number);

	    if (done)
	    {
		if (A.End() != COMMITTED)
		    done = FALSE;
	    }
	    else
		A.Abort();
	}
	else
	    A.Abort();
    }
    else
	A.Abort();
}

void NameServer::Reset (int& res)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "void NameServer::Reset (int& res)" << endl;
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
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::save_state (ObjectState& os, ObjectType)" << endl;
#endif
    
    Boolean res;

    res = os.pack(gv_ok) && os.pack(sv_ok);

    if (gv_ok)
	res = (GV->get_uid()).pack(os);

    if (res)
    {
	if (sv_ok)
	    res = (SV->get_uid()).pack(os);
    }

    return res;
}

Boolean NameServer::restore_state (ObjectState& os, ObjectType)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_USER1 << VIS_PUBLIC;
    debug_stream << "Boolean NameServer::restore_state (ObjectState& os, ObjectType)" << endl;
#endif
    
    Boolean res;
    int err;

    res = os.unpack(gv_ok) && os.unpack(sv_ok);

    if (gv_ok)
    {
	Uid objUid(NIL_UID);
	res = objUid.unpack(os);

	if (res && (GV == 0))
	{
	    err = -1;
	    GV = new GroupView(objUid, err);
	    res = (Boolean) (err == 0);
	}
    }

    if (res)
    {
	if (sv_ok)
	{
	    Uid objUid(NIL_UID);
	    res = objUid.unpack(os);

	    if (res && (SV == 0))
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
