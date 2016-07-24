/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


/*
 * $Id: ClANSAAct.cc,v 1.1 1993/11/03 14:38:35 nmcl Exp $
 */

static const char RCSid[] = "$Id: ClANSAAct.cc,v 1.1 1993/11/03 14:38:35 nmcl Exp $";


/*
 * This class provides C++ interface to the client side of the
 * ANSA rpc protocol, which is tailerd for "atomic actions".
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

#ifdef  DEBUG
#  ifndef DEBUG_H_
#    include <Common/Debug.h>
#  endif
#endif

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

#ifndef ANSA_CLANSAACT_H_
#  include <RPC/ANSA/ClANSAAct.h>
#endif

#ifndef ANSA_SERVERAA_H_
#  include <Arjuna/ServerAA.h>	
#endif

#ifndef ANSAACTOP_H_
#  include "ANSAActOp.h"
#endif

#ifndef ANSAINITR_H_
#  include "ANSAInitR.h"
#endif

#ifndef ANSACALLR_H_
#  include "ANSACallR.h"
#endif

#ifndef ANSATERMR_H_
#  include "ANSATermR.h"
#endif

#ifndef ANSACADVR_H_
#  include "ANSACadvR.h"
#endif

//
//	Fundermental generic operstions provided by
//	the RPC interface.
//

ClientANSAAction_rpc::ClientANSAAction_rpc(const char* serviceName) 
                                                : ClientANSA_rpc(serviceName),
						  is_zombie(FALSE),
						  obj_uid()
{
#ifdef DEBUG
        debug_stream << CONSTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientANSAAction_rpc::ClientANSAAction_rpc(const char* serviceName)\n" << flush;
#endif
}

ClientANSAAction_rpc::~ClientANSAAction_rpc()
{
#ifdef DEBUG
        debug_stream << DESTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientANSAAction_rpc::~ClientANSAAction_rpc()\n" << flush;
#endif

	if ((ClientANSA_rpc::GetServerStatus() == OPERATIONAL) || (ClientANSA_rpc::GetServerStatus() == INOPERATIONAL))
        {
                error_stream << WARNING << " ClientActionANSA_rpc::~ClientActionANSA_rpc : Call on active servers (Will soft terminate)\n";
                Terminate();
        }

	if ((! is_zombie) && (AtomicAction::Current() != NULL))
	{
		ANSACadaverRecord *record = new ANSACadaverRecord(obj_uid, this);

		if (! AtomicAction::Current()->add(record))
			delete record;
	}

}

RPC_Status ClientANSAAction_rpc::Initiate()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientANSAAction_rpc::Initiate()\n" << flush;
#endif

	RPC_Status oper_status = ClientANSA_rpc::Initiate();

	if ((oper_status != OPER_NOTDONE) && (AtomicAction::Current() != NULL))
	{
		ANSAInitiateRecord *record = new ANSAInitiateRecord(obj_uid,this);

		if (! AtomicAction::Current()->add(record))
			delete record;
	}
			
	return oper_status;
}

RPC_Status ClientANSAAction_rpc::Call(long  opcode,  Buffer& call,
				      long& errcode, Buffer& result)
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientANSAAction_rpc::Call(long  opcode,  Buffer& call, long& errcode, Buffer& result)\n" << flush;
#endif

	Buffer ActHierBuff, dummyResult;
	RPC_Status oper_status = OPER_NOTDONE;

	PackActionHierarchy(ActHierBuff);

	oper_status = ClientANSA_rpc::Call(opcode, call, ActHierBuff, errcode, result, dummyResult);

	if ((oper_status != OPER_NOTDONE) && (AtomicAction::Current() != NULL))
	{
		ANSACallRecord *record = new ANSACallRecord(obj_uid,this);

		if (! AtomicAction::Current()->add(record))
			delete record;
	}

	return oper_status;
}

RPC_Status ClientANSAAction_rpc::Terminate()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientANSAAction_rpc::Terminate()\n" << flush;
#endif

	RPC_Status oper_status = OPER_NOTDONE;

	if (AtomicAction::Current() != NULL)
	{
		oper_status = ClientANSA_rpc::SoftTerminate();
		
		if (oper_status == OPER_DONE)
		{
			ANSATerminateRecord *record = new ANSATerminateRecord(obj_uid,this);

			if (! AtomicAction::Current()->add(record))
				delete record;
		}
	}
	else
		oper_status = ClientANSA_rpc::Terminate();

	return oper_status;
}

//
//	Private member operations.
//

void ClientANSAAction_rpc::PackActionHierarchy(Buffer& hierBuff)
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
  	debug_stream << "ClientANSAAction_rpc::PackActionHierarchy(Buffer& hierBuff)\n" << flush;
#endif

	AtomicAction* currentPTR = AtomicAction::Current();
	int           ActInHier   = (currentPTR ? currentPTR->hierarchyDepth() :0);
        ActionHierarchy actHier(ActInHier);
        char *coordinatorNode = ServerAtomicAction::get_coordin_nodename();


        if (coordinatorNode == NULL)
        {
            coordinatorNode = new char[MAXHOSTNAMELEN];
            ::gethostname(coordinatorNode, MAXHOSTNAMELEN);
            ServerAtomicAction::set_coordin_nodename(coordinatorNode);

        }

#ifdef DEBUG
        debug_stream << TRIVIAL_FUNCS << FAC_ACTION_RPC << VIS_PRIVATE;
        debug_stream << "ClientANSAAction_rpc::PackActionHierarchy() Coordinator node name \"" << coordinatorNode << "\"\n" << flush;
#endif

        hierBuff.pack(coordinatorNode);
        hierBuff.pack(ActInHier);
        if (currentPTR)
            currentPTR->getHierarchy(actHier);

        actHier.pack(hierBuff);
#ifndef __GNUG__
        delete [] coordinatorNode;
#else
	delete coordinatorNode;
#endif
}

void ClientANSAAction_rpc::DoRemote_NestedAbort()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientANSAAction_rpc::DoRemote_NestedAdort()\n" << flush;
#endif

        long   rpc_res = OPER_UNKNOWN;
	long   errcode = AAO_NOTDONE;
        Buffer call, ActHierBuff;
        Buffer result, dummyResult;

        if (AtomicAction::Current() != NULL)
        	(AtomicAction::Current()->get_uid()).pack(call);

	PackActionHierarchy(ActHierBuff);

        rpc_res = ClientANSA_rpc::Call(AAO_NESTEDABORT, call, ActHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != AAO_DONE)
			error_stream << WARNING << "ClientANSAAction_rpc::DoRemote_NestedAdort() : Remotely failed\n";
	}
	else
		error_stream << WARNING << "ClientANSAAction_rpc::DoRemote_NestedAdort() : Locally(?) failed\n";
}

void ClientANSAAction_rpc::DoRemote_NestedCommit()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientANSAAction_rpc::DoRemote_NestedCommit()\n" << flush;
#endif

        long   rpc_res = OPER_UNKNOWN;
	long   errcode = AAO_NOTDONE;
        Buffer call, ActHierBuff;
        Buffer result, dummyResult;

        if (AtomicAction::Current() != NULL)
        	(AtomicAction::Current()->get_uid()).pack(call);

	PackActionHierarchy(ActHierBuff);

        rpc_res = ClientANSA_rpc::Call(AAO_NESTEDCOMMIT, call, ActHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != AAO_DONE)
			error_stream << WARNING << "ClientANSAAction_rpc::DoRemote_NestedCommit() : Remotely failed\n";
	}
	else
		error_stream << WARNING << "ClientANSAAction_rpc::DoRemote_NestedCommit() : Locally(?) failed\n";
}

PrepareOutcome ClientANSAAction_rpc::DoRemote_NestedPrepare()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientANSAAction_rpc::DoRemote_NestedPrepare()\n" << flush;
#endif

        long   rpc_res = OPER_UNKNOWN;
	long   errcode = AAO_NOTDONE;
        Buffer call, ActHierBuff;
        Buffer result, dummyResult;
	int    PreOutCome = PREP_NOTOK;

        if (AtomicAction::Current() != NULL)
        	(AtomicAction::Current()->get_uid()).pack(call);

	PackActionHierarchy(ActHierBuff);

        rpc_res = ClientANSA_rpc::Call(AAO_NESTEDPREPARE, call, ActHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != AAO_DONE)
		{
			error_stream << WARNING << "ClientANSAAction_rpc::DoRemote_NestedPrepare(() : Remotely failed\n";
			return PREP_NOTOK;
		}
		else
			result.unpack(PreOutCome);
	}
	else
	{
		error_stream << WARNING << "ClientANSAAction_rpc::DoRemote_NestedPrepare() : Locally(?) failed\n";
		return PREP_NOTOK;
	}

	return (PrepareOutcome) PreOutCome;
}

void ClientANSAAction_rpc::DoRemote_TopLevelAbort()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientANSAAction_rpc::DoRemote_TopLevelAbort()\n" << flush;
#endif

        long   rpc_res = OPER_UNKNOWN;
	long   errcode = AAO_NOTDONE;
        Buffer call, ActHierBuff;
        Buffer result, dummyResult;

        if (AtomicAction::Current() != NULL)
        	(AtomicAction::Current()->get_uid()).pack(call);

	PackActionHierarchy(ActHierBuff);

        rpc_res = ClientANSA_rpc::Call(AAO_TOPLEVELABORT, call, ActHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != AAO_DONE)
			error_stream << WARNING << "ClientANSAAction_rpc::DoRemote_TopLevelAbort() : Remotely failed\n";
	}
	else
		error_stream << WARNING << "ClientANSAAction_rpc::DoRemote_TopLevelAbort() : Locally(?) failed\n";
}

void ClientANSAAction_rpc::DoRemote_TopLevelCommit()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientANSAAction_rpc::DoRemote_TopLevelCommit()\n" << flush;
#endif

        long   rpc_res = OPER_UNKNOWN;
	long   errcode = AAO_NOTDONE;
        Buffer call, ActHierBuff;
        Buffer result, dummyResult;

        if (AtomicAction::Current() != NULL)
        	(AtomicAction::Current()->get_uid()).pack(call);

	PackActionHierarchy(ActHierBuff);

	while (rpc_res != OPER_DONE)
	{
	    rpc_res = ClientANSA_rpc::Call(AAO_TOPLEVELCOMMIT, call, ActHierBuff, errcode, result, dummyResult);

	    if (rpc_res != OPER_DONE)
	    {
		error_stream << WARNING << "Remote server not responding - retrying\n";
	    }
	}
}

PrepareOutcome ClientANSAAction_rpc::DoRemote_TopLevelPrepare()
{
#ifdef DEBUG
        debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
  	debug_stream << "ClientANSAAction_rpc::DoRemote_TopLevelPrepare()\n" << flush;
#endif


        long   rpc_res = OPER_UNKNOWN;
	long   errcode = AAO_NOTDONE;
        Buffer call, ActHierBuff;
        Buffer result, dummyResult;
	int    PreOutCome = PREP_NOTOK;

        if (AtomicAction::Current() != NULL)
        	(AtomicAction::Current()->get_uid()).pack(call);

	PackActionHierarchy(ActHierBuff);

        rpc_res = ClientANSA_rpc::Call(AAO_TOPLEVELPREPARE, call, ActHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != AAO_DONE)
		{
			error_stream << WARNING << "ClientANSAAction_rpc::DoRemote_TopLevelPrepare(() : Remotely failed\n";
			return PREP_NOTOK;
		}
		else
			result.unpack(PreOutCome);
	}
	else
	{
		error_stream << WARNING << "ClientANSAAction_rpc::DoRemote_TopLevelPrepare() : Locally(?) failed\n";
		return PREP_NOTOK;
	}

	return (PrepareOutcome) PreOutCome;
}

ClientANSAAction_rpc::ClientANSAAction_rpc(ClientANSAAction_rpc* doner)
                                                 : ClientANSA_rpc(doner)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientANSA_rpcAction_rpc::ClientANSAAction_rpc_rpc(ClientANSAAction_rpc_rpc* doner)\n" << flush;
#endif
    is_zombie = TRUE;
    obj_uid = doner->obj_uid;
}
