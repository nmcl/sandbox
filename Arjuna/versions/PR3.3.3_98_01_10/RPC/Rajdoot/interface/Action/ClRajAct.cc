/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: ClRajAct.cc,v 1.13 1995/09/25 15:05:03 nmcl Exp $
 */

static const char RCSid[] = "$Id: ClRajAct.cc,v 1.13 1995/09/25 15:05:03 nmcl Exp $";


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

ClientRajdootAction_rpc::ClientRajdootAction_rpc ( const char* const serviceName) 
						 : ClientRajdoot_rpc(serviceName),
						   atomicEnabled(TRUE),
						   isZombie(FALSE),
						   objUid()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdootAction_rpc::ClientRajdootAction_rpc( "
		 << serviceName << " )" << endl;
    debug_stream.unlock();
    
#endif
}

ClientRajdootAction_rpc::~ClientRajdootAction_rpc ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdootAction_rpc::~ClientRajdootAction_rpc()" << endl;
    debug_stream.unlock();
    
#endif

    if (ClientRajdoot_rpc::getServerStatus() == UNINITIATED)
	return;
    
    if (ClientRajdoot_rpc::getServerStatus() == INIT_UNABLE)
	terminate();

    if ((ClientRajdoot_rpc::getServerStatus() == OPERATIONAL) || (ClientRajdoot_rpc::getServerStatus() == INOPERATIONAL))
    {
	error_stream << WARNING
		     << "ClientActionRajdoot_rpc::~ClientActionRajdoot_rpc : Call on active servers (Will soft terminate)" << endl;
	terminate();
    }

    if (atomicEnabled && (! isZombie) && (AtomicAction::Current() != 0))
    {
	RajdootCadaverRecord *record = new RajdootCadaverRecord(objUid, this);

	if (AtomicAction::Current()->add(record) != AtomicAction::ADDED)
	    delete record;
    }
}

RPC_Status ClientRajdootAction_rpc::initiate ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdootAction_rpc::initiate()" << endl;
    debug_stream.unlock();
    
#endif

	RPC_Status oper_status = ClientRajdoot_rpc::initiate();

	if (atomicEnabled && (oper_status != OPER_NOTDONE) && (AtomicAction::Current() != 0))
	{
		RajdootInitiateRecord *record = new RajdootInitiateRecord(objUid,this);

		if (AtomicAction::Current()->add(record) != AtomicAction::ADDED)
			delete record;
	}
			
	return oper_status;
}

RPC_Status ClientRajdootAction_rpc::call (Int32  opcode,  Buffer& callBuff,
					  Int32& errcode, Buffer& result)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdootAction_rpc::call(Int32  opcode,  Buffer& callBuff, Int32& errcode, Buffer& result)" << endl;
    debug_stream.unlock();
    
#endif

	Buffer actHierBuff;
	Buffer dummyResult;
	RPC_Status oper_status = OPER_NOTDONE;

	if (atomicEnabled)
            packActionHierarchy(actHierBuff);
        else
            packNullActionHierarchy(actHierBuff);

	oper_status = ClientRajdoot_rpc::call(opcode, callBuff, actHierBuff, errcode, result, dummyResult);

	if (atomicEnabled && (oper_status != OPER_NOTDONE) && (AtomicAction::Current() != 0))
	{
		RajdootCallRecord *record = new RajdootCallRecord(objUid,this);

		if (AtomicAction::Current()->add(record) != AtomicAction::ADDED)
			delete record;
	}

	return oper_status;
}

RPC_Status ClientRajdootAction_rpc::terminate ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdootAction_rpc::terminate()" << endl;
    debug_stream.unlock();
    
#endif

	RPC_Status oper_status = OPER_NOTDONE;

	if (atomicEnabled && AtomicAction::Current() != 0)
	{
		oper_status = ClientRajdoot_rpc::softTerminate();
		
		if (oper_status == OPER_DONE)
		{
			RajdootTerminateRecord *record = new RajdootTerminateRecord(objUid,this);

			if (AtomicAction::Current()->add(record) != AtomicAction::ADDED)
				delete record;
		}
	}
	else
		oper_status = ClientRajdoot_rpc::terminate();

	return oper_status;
}

//
//	Private member operations.
//

void ClientRajdootAction_rpc::packActionHierarchy ( Buffer& hierBuff )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ClientRajdootAction_rpc::packActionHierarchy(Buffer& hierBuff)"
		 << endl;
    debug_stream.unlock();
    
#endif

    AtomicAction* currentPTR = AtomicAction::Current();
    int actInHier   = (currentPTR ? currentPTR->hierarchyDepth() : 0);
    ActionHierarchy *actHier = 0;
    char *coordinatorNode = ServerAtomicAction::getCoordinNodename();	

    if (coordinatorNode == 0)
    {
	coordinatorNode = new char[MAXHOSTNAMELEN];
	::gethostname(coordinatorNode, MAXHOSTNAMELEN);
	ServerAtomicAction::setCoordinNodename(coordinatorNode);
    }

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << TRIVIAL_FUNCS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ClientRajdootAction_rpc::packActionHierarchy() Coordinator is \""
		 << coordinatorNode << "\"" << endl;
    debug_stream.unlock();
    
#endif

    hierBuff.packMappedString(coordinatorNode);
    hierBuff.pack(actInHier);

    if (actInHier > 0)
    {
	actHier = currentPTR->getHierarchy();
	actHier->pack(hierBuff);
    }
    
#ifndef GCC_ARRAY_DELETE_BUG
    ::delete [] coordinatorNode;
#else
    ::delete coordinatorNode;
#endif
}

void ClientRajdootAction_rpc::packNullActionHierarchy ( Buffer& hierBuff )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ClientRajdootAction_rpc::packNullActionHierarchy(Buffer& hierBuff)"
		 << endl;
    debug_stream.unlock();
    
#endif

    ActionHierarchy actHier(0);
    char *coordinatorNode = ServerAtomicAction::getCoordinNodename();
    
    if (coordinatorNode == 0)
    {
	coordinatorNode = new char[MAXHOSTNAMELEN];
	::gethostname(coordinatorNode, MAXHOSTNAMELEN);
	ServerAtomicAction::setCoordinNodename(coordinatorNode);
    }

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << TRIVIAL_FUNCS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ClientRajdootAction_rpc::packNullActionHierarchy() Coordinator is \""
		 << coordinatorNode << "\"" << endl;
    debug_stream.unlock();
    
#endif

    hierBuff.packMappedString(coordinatorNode);
    hierBuff.pack(0);
	
#ifndef GCC_ARRAY_DELETE_BUG
    ::delete [] coordinatorNode;
#else
    ::delete coordinatorNode;
#endif
}

Boolean ClientRajdootAction_rpc::doRemoteNestedAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdootAction_rpc::doRemoteNestedAdort()" << endl;
    debug_stream.unlock();
    
#endif

        Int32   rpc_res = OPER_UNKNOWN;
	Int32   errcode = RAO_NOTDONE;
        Buffer actHierBuff;
        Buffer callBuff, result, dummyResult;

        if (AtomicAction::Current() != 0)
        	(AtomicAction::Current()->get_uid()).pack(callBuff);

	packActionHierarchy(actHierBuff);

        rpc_res = ClientRajdoot_rpc::call(RAO_NESTEDABORT, callBuff, actHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != RAO_DONE)
			error_stream << WARNING << "ClientRajdootAction_rpc::doRemoteNestedAdort() : Remotely failed" << endl;
		else
		    return TRUE;
	}
	else
		error_stream << WARNING << "ClientRajdootAction_rpc::doRemoteNestedAdort() : Locally(?) failed" << endl;

	return FALSE;
}

Boolean ClientRajdootAction_rpc::doRemoteNestedCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdootAction_rpc::doRemoteNestedCommit()" << endl;
    debug_stream.unlock();
    
#endif

        Int32   rpc_res = OPER_UNKNOWN;
	Int32   errcode = RAO_NOTDONE;
        Buffer actHierBuff;
        Buffer callBuff, result, dummyResult;

        if (AtomicAction::Current() != 0)
        	(AtomicAction::Current()->get_uid()).pack(callBuff);

	packActionHierarchy(actHierBuff);

        rpc_res = ClientRajdoot_rpc::call(RAO_NESTEDCOMMIT, callBuff, actHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != RAO_DONE)
			error_stream << WARNING << "ClientRajdootAction_rpc::doRemoteNestedCommit() : Remotely failed" << endl;
		else
		    return TRUE;
	}
	else
		error_stream << WARNING << "ClientRajdootAction_rpc::doRemoteNestedCommit() : Locally(?) failed" << endl;

	return FALSE;
}

PrepareOutcome ClientRajdootAction_rpc::doRemoteNestedPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdootAction_rpc::doRemoteNestedPrepare()" << endl;
    debug_stream.unlock();
    
#endif

        Int32   rpc_res = OPER_UNKNOWN;
	Int32   errcode = RAO_NOTDONE;
        Buffer actHierBuff;
        Buffer callBuff, result, dummyResult;
	int    PreOutCome = PREP_NOTOK;

        if (AtomicAction::Current() != 0)
        	(AtomicAction::Current()->get_uid()).pack(callBuff);

	packActionHierarchy(actHierBuff);

        rpc_res = ClientRajdoot_rpc::call(RAO_NESTEDPREPARE, callBuff, actHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != RAO_DONE)
		{
			error_stream << WARNING << "ClientRajdootAction_rpc::doRemoteNestedPrepare(() : Remotely failed" << endl;
			return PREP_NOTOK;
		}
		else
			result.unpack(PreOutCome);
	}
	else
	{
		error_stream << WARNING << "ClientRajdootAction_rpc::doRemoteNestedPrepare() : Locally(?) failed" << endl;
		return PREP_NOTOK;
	}

	return (PrepareOutcome) PreOutCome;
}

Boolean ClientRajdootAction_rpc::doRemoteTopLevelAbort ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdootAction_rpc::doRemoteTopLevelAbort()" << endl;
    debug_stream.unlock();
    
#endif

        Int32   rpc_res = OPER_UNKNOWN;
	Int32   errcode = RAO_NOTDONE;
        Buffer actHierBuff;
        Buffer callBuff, result, dummyResult;

        if (AtomicAction::Current() != 0)
        	(AtomicAction::Current()->get_uid()).pack(callBuff);

	packActionHierarchy(actHierBuff);

        rpc_res = ClientRajdoot_rpc::call(RAO_TOPLEVELABORT, callBuff, actHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != RAO_DONE)
			error_stream << WARNING << "ClientRajdootAction_rpc::doRemoteTopLevelAbort() : Remotely failed" << endl;
		else
		    return TRUE;
	}
	else
		error_stream << WARNING << "ClientRajdootAction_rpc::doRemoteTopLevelAbort() : Locally(?) failed" << endl;

	return FALSE;
}

Boolean ClientRajdootAction_rpc::doRemoteTopLevelCommit ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdootAction_rpc::doRemoteTopLevelCommit()" << endl;
    debug_stream.unlock();
    
#endif

        Int32   rpc_res = OPER_UNKNOWN;
	Int32   errcode = RAO_NOTDONE;
        Buffer actHierBuff;
        Buffer callBuff, result, dummyResult;

        if (AtomicAction::Current() != 0)
        	(AtomicAction::Current()->get_uid()).pack(callBuff);

	packActionHierarchy(actHierBuff);

	rpc_res = ClientRajdoot_rpc::call(RAO_TOPLEVELCOMMIT, callBuff, actHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != RAO_DONE)
			error_stream << WARNING << "ClientRajdootAction_rpc::doRemoteTopLevelCommit() : Remotely failed" << endl;
		else
		    return TRUE;
	}
	else
		error_stream << WARNING << "ClientRajdootAction_rpc::doRemoteTopLevelCommit() : Locally(?) failed" << endl;

	return FALSE;
}

PrepareOutcome ClientRajdootAction_rpc::doRemoteTopLevelPrepare ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdootAction_rpc::doRemoteTopLevelPrepare()" << endl;
    debug_stream.unlock();
    
#endif


        Int32   rpc_res = OPER_UNKNOWN;
	Int32   errcode = RAO_NOTDONE;
        Buffer actHierBuff;
        Buffer callBuff, result, dummyResult;
	int    PreOutCome = PREP_NOTOK;

        if (AtomicAction::Current() != 0)
        	(AtomicAction::Current()->get_uid()).pack(callBuff);

	packActionHierarchy(actHierBuff);

        rpc_res = ClientRajdoot_rpc::call(RAO_TOPLEVELPREPARE, callBuff, actHierBuff, errcode, result, dummyResult);

        if (rpc_res == OPER_DONE)
	{
		if (errcode != RAO_DONE)
		{
			error_stream << WARNING << "ClientRajdootAction_rpc::doRemoteTopLevelPrepare(() : Remotely failed" << endl;
			return PREP_NOTOK;
		}
		else
			result.unpack(PreOutCome);
	}
	else
	{
		error_stream << WARNING << "ClientRajdootAction_rpc::doRemoteTopLevelPrepare() : Locally(?) failed" << endl;
		return PREP_NOTOK;
	}

	return (PrepareOutcome) PreOutCome;
}

void ClientRajdootAction_rpc::enableAtomic ()
{
    if (getServerStatus() == UNINITIATED)
        atomicEnabled = TRUE;
    else
        error_stream << WARNING << "ClientRajdootAction_rpc::enableAtomic() server is initiated" << endl;
}

void ClientRajdootAction_rpc::disableAtomic ()
{
    if (getServerStatus() == UNINITIATED)
        atomicEnabled = FALSE;
    else
        error_stream << WARNING << "ClientRajdootAction_rpc::disableAtomic() server is initiated" << endl;
}

ClientRajdootAction_rpc::ClientRajdootAction_rpc ( ClientRajdootAction_rpc* doner )
                                                 : ClientRajdoot_rpc(doner)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ClientRajdoot_rpcAction_rpc::ClientRajdootAction_rpc_rpc(ClientRajdootAction_rpc_rpc* doner)" << endl;
    debug_stream.unlock();
    
#endif
    isZombie = TRUE;
    objUid = doner->objUid;
}
