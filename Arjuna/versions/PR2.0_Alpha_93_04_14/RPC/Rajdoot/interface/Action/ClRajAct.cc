/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ClRajAct.cc,v 1.22 1993/04/27 14:45:39 ngdp Exp $
 */

static const char RCSid[] = "$Id: ClRajAct.cc,v 1.22 1993/04/27 14:45:39 ngdp Exp $";


/*
 * This class provides C++ interface to the client side of the
 * Rajdoot rpc protocol, which is tailerd for "atomic actions".
 *
 */

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif 

#  ifndef DEBUG_H_
#    include <Common/Debug.h>
#  endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef ACTIONH_H_
#  include <Arjuna/ActionH.h>
#endif

#ifndef ACTIONA_H_
#  include <Arjuna/AtomicA.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef RAJDOOT_CLRAJACT_H_
#  include <RPC/Rajdoot/ClRajAct.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>	
#endif

#ifndef RAJACTOP_H_
#  include "RajActOp.h"
#endif

#ifndef RAJINITR_H_
#  include "RajInitR.h"
#endif

#ifndef RAJCALLR_H_
#  include "RajCallR.h"
#endif

#ifndef RAJTERMR_H_
#  include "RajTermR.h"
#endif

#ifndef RAJCADVR_H_
#  include "RajCadvR.h"
#endif

//
//	Fundermental generic operstions provided by
//	the RPC interface.
//

ClientRajdootAction_rpc::ClientRajdootAction_rpc(const char* serviceName) 
                                                : ClientRajdoot_rpc(serviceName),
						  is_zombie(FALSE),
						  obj_uid()
{
#ifdef DEBUG
        debug_stream << CONSTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientRajdootAction_rpc::ClientRajdootAction_rpc( "
	    << serviceName << " )\n" << flush;
#endif
}

ClientRajdootAction_rpc::~ClientRajdootAction_rpc()
{
#ifdef DEBUG
        debug_stream << DESTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientRajdootAction_rpc::~ClientRajdootAction_rpc()\n" << flush;
#endif

	if ((ClientRajdoot_rpc::GetServerStatus() == OPERATIONAL) || (ClientRajdoot_rpc::GetServerStatus() == INOPERATIONAL))
        {
                error_stream << WARNING << " ClientActionRajdoot_rpc::~ClientActionRajdoot_rpc : Call on active servers (Will soft terminate)\n";
                Terminate();
        }

	if ((! is_zombie) && (AtomicAction::Current() != 0))
	{
		RajdootCadaverRecord *record = new RajdootCadaverRecord(obj_uid, this);

		if (! AtomicAction::Current()->add(record))
			delete record;
	}

}

RPC_Status ClientRajdootAction_rpc::Initiate()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientRajdootAction_rpc::Initiate()\n" << flush;
#endif

	RPC_Status oper_status = ClientRajdoot_rpc::Initiate();

	if ((oper_status != OPER_NOTDONE) && (AtomicAction::Current() != 0))
	{
		RajdootInitiateRecord *record = new RajdootInitiateRecord(obj_uid,this);

		if (! AtomicAction::Current()->add(record))
			delete record;
	}
			
	return oper_status;
}

RPC_Status ClientRajdootAction_rpc::Call(long  opcode,  Buffer& call,
				   	 long& errcode, Buffer& result)
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientRajdootAction_rpc::Call(long  opcode,  Buffer& call, long& errcode, Buffer& result)\n" << flush;
#endif

	Buffer ActHierBuff;
	Buffer dummyResult;
	RPC_Status oper_status = OPER_NOTDONE;

	PackActionHierarchy(ActHierBuff);

	oper_status = ClientRajdoot_rpc::Call(opcode, call, ActHierBuff, errcode, result, dummyResult);

	if ((oper_status != OPER_NOTDONE) && (AtomicAction::Current() != 0))
	{
		RajdootCallRecord *record = new RajdootCallRecord(obj_uid,this);

		if (! AtomicAction::Current()->add(record))
			delete record;
	}

	return oper_status;
}

RPC_Status ClientRajdootAction_rpc::Terminate()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientRajdootAction_rpc::Terminate()\n" << flush;
#endif

	RPC_Status oper_status = OPER_NOTDONE;

	if (AtomicAction::Current() != 0)
	{
		oper_status = ClientRajdoot_rpc::SoftTerminate();
		
		if (oper_status == OPER_DONE)
		{
			RajdootTerminateRecord *record = new RajdootTerminateRecord(obj_uid,this);

			if (! AtomicAction::Current()->add(record))
				delete record;
		}
	}
	else
		oper_status = ClientRajdoot_rpc::Terminate();

	return oper_status;
}

//
//	Private member operations.
//

void ClientRajdootAction_rpc::PackActionHierarchy( Buffer& hierBuff )
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
  	debug_stream << "ClientRajdootAction_rpc::PackActionHierarchy(Buffer& hierBuff)\n" << flush;
#endif

	AtomicAction* currentPTR = AtomicAction::Current();
	int           ActInHier   = (currentPTR ? currentPTR->hierarchyDepth() :0);
	ActionHierarchy actHier(ActInHier);
	char *coordinatorNode = ServerAtomicAction::get_coordin_nodename();
	

	if (coordinatorNode == 0)
	{
	    coordinatorNode = new char[MAXHOSTNAMELEN];
	    ::gethostname(coordinatorNode, MAXHOSTNAMELEN);
	    ServerAtomicAction::set_coordin_nodename(coordinatorNode);

	}

#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_ACTION_RPC << VIS_PRIVATE;
  	debug_stream << "ClientRajdootAction_rpc::PackActionHierarchy() Coordinator node name \"" << coordinatorNode << "\"\n" << flush;
#endif

	hierBuff.pack(coordinatorNode);	
	hierBuff.pack(ActInHier);
	if (currentPTR)
	    currentPTR->getHierarchy(actHier);
	
	actHier.pack(hierBuff);
	delete [] coordinatorNode;
}

void ClientRajdootAction_rpc::DoRemote_NestedAbort()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientRajdootAction_rpc::DoRemote_NestedAdort()\n" << flush;
#endif

        long   rpc_res = OPER_UNKNOWN;
	long   errcode = RAO_NOTDONE;
        Buffer ActHierBuff;
        Buffer call, result, dummyResult;

        if (AtomicAction::Current() != 0)
        	(AtomicAction::Current()->get_uid()).pack(call);

	PackActionHierarchy(ActHierBuff);

        rpc_res = ClientRajdoot_rpc::Call(RAO_NESTEDABORT, call, ActHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != RAO_DONE)
			error_stream << WARNING << "ClientRajdootAction_rpc::DoRemote_NestedAdort() : Remotely failed\n";
	}
	else
		error_stream << WARNING << "ClientRajdootAction_rpc::DoRemote_NestedAdort() : Locally(?) failed\n";
}

void ClientRajdootAction_rpc::DoRemote_NestedCommit()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientRajdootAction_rpc::DoRemote_NestedCommit()\n" << flush;
#endif

        long   rpc_res = OPER_UNKNOWN;
	long   errcode = RAO_NOTDONE;
        Buffer ActHierBuff;
        Buffer call, result, dummyResult;

        if (AtomicAction::Current() != 0)
        	(AtomicAction::Current()->get_uid()).pack(call);

	PackActionHierarchy(ActHierBuff);

        rpc_res = ClientRajdoot_rpc::Call(RAO_NESTEDCOMMIT, call, ActHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != RAO_DONE)
			error_stream << WARNING << "ClientRajdootAction_rpc::DoRemote_NestedCommit() : Remotely failed\n";
	}
	else
		error_stream << WARNING << "ClientRajdootAction_rpc::DoRemote_NestedCommit() : Locally(?) failed\n";
}

PrepareOutcome ClientRajdootAction_rpc::DoRemote_NestedPrepare()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientRajdootAction_rpc::DoRemote_NestedPrepare()\n" << flush;
#endif

        long   rpc_res = OPER_UNKNOWN;
	long   errcode = RAO_NOTDONE;
        Buffer ActHierBuff;
        Buffer call, result, dummyResult;
	int    PreOutCome = PREP_NOTOK;

        if (AtomicAction::Current() != 0)
        	(AtomicAction::Current()->get_uid()).pack(call);

	PackActionHierarchy(ActHierBuff);

        rpc_res = ClientRajdoot_rpc::Call(RAO_NESTEDPREPARE, call, ActHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != RAO_DONE)
		{
			error_stream << WARNING << "ClientRajdootAction_rpc::DoRemote_NestedPrepare(() : Remotely failed\n";
			return PREP_NOTOK;
		}
		else
			result.unpack(PreOutCome);
	}
	else
	{
		error_stream << WARNING << "ClientRajdootAction_rpc::DoRemote_NestedPrepare() : Locally(?) failed\n";
		return PREP_NOTOK;
	}

	return (PrepareOutcome) PreOutCome;
}

void ClientRajdootAction_rpc::DoRemote_TopLevelAbort()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientRajdootAction_rpc::DoRemote_TopLevelAbort()\n" << flush;
#endif

        long   rpc_res = OPER_UNKNOWN;
	long   errcode = RAO_NOTDONE;
        Buffer ActHierBuff;
        Buffer call, result, dummyResult;

        if (AtomicAction::Current() != 0)
        	(AtomicAction::Current()->get_uid()).pack(call);

	PackActionHierarchy(ActHierBuff);

        rpc_res = ClientRajdoot_rpc::Call(RAO_TOPLEVELABORT, call, ActHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != RAO_DONE)
			error_stream << WARNING << "ClientRajdootAction_rpc::DoRemote_TopLevelAbort() : Remotely failed\n";
	}
	else
		error_stream << WARNING << "ClientRajdootAction_rpc::DoRemote_TopLevelAbort() : Locally(?) failed\n";
}

void ClientRajdootAction_rpc::DoRemote_TopLevelCommit()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientRajdootAction_rpc::DoRemote_TopLevelCommit()\n" << flush;
#endif

        long   rpc_res = OPER_UNKNOWN;
	long   errcode = RAO_NOTDONE;
        Buffer ActHierBuff;
        Buffer call, result, dummyResult;

        if (AtomicAction::Current() != 0)
        	(AtomicAction::Current()->get_uid()).pack(call);

	PackActionHierarchy(ActHierBuff);

	while (rpc_res != OPER_DONE)
	{
	    rpc_res = ClientRajdoot_rpc::Call(RAO_TOPLEVELCOMMIT, call, ActHierBuff, errcode, result, dummyResult);

	    if (rpc_res != OPER_DONE)
	    {
		error_stream << WARNING << "Remote server not responding - retrying\n";
	    }
	}
}

PrepareOutcome ClientRajdootAction_rpc::DoRemote_TopLevelPrepare()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientRajdootAction_rpc::DoRemote_TopLevelPrepare()\n" << flush;
#endif


        long   rpc_res = OPER_UNKNOWN;
	long   errcode = RAO_NOTDONE;
        Buffer ActHierBuff;
        Buffer call, result, dummyResult;
	int    PreOutCome = PREP_NOTOK;

        if (AtomicAction::Current() != 0)
        	(AtomicAction::Current()->get_uid()).pack(call);

	PackActionHierarchy(ActHierBuff);

        rpc_res = ClientRajdoot_rpc::Call(RAO_TOPLEVELPREPARE, call, ActHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != RAO_DONE)
		{
			error_stream << WARNING << "ClientRajdootAction_rpc::DoRemote_TopLevelPrepare(() : Remotely failed\n";
			return PREP_NOTOK;
		}
		else
			result.unpack(PreOutCome);
	}
	else
	{
		error_stream << WARNING << "ClientRajdootAction_rpc::DoRemote_TopLevelPrepare() : Locally(?) failed\n";
		return PREP_NOTOK;
	}

	return (PrepareOutcome) PreOutCome;
}

ClientRajdootAction_rpc::ClientRajdootAction_rpc(ClientRajdootAction_rpc* doner)
                                                 : ClientRajdoot_rpc(doner)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpcAction_rpc::ClientRajdootAction_rpc_rpc(ClientRajdootAction_rpc_rpc* doner)\n" << flush;
#endif
    is_zombie = TRUE;
    obj_uid = doner->obj_uid;
}
