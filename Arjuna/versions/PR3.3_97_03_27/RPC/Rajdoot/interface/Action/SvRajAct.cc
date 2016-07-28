/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: SvRajAct.cc,v 1.10 1995/07/03 14:29:34 ngdp Exp $
 */

static const char RCSid[] = "$Id: SvRajAct.cc,v 1.10 1995/07/03 14:29:34 ngdp Exp $";

/*
 * This class provides C++ interface to the server side of the
 * Rajdoot rpc protocol.
 *
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifdef DEBUG
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

#ifndef ABSTRACTR_H_
#  include <Arjuna/AbstractR.h>
#endif

#ifndef RAJDOOT_SVRAJACT_H_
#  include <RPC/Rajdoot/SvRajAct.h>
#endif

#ifndef RAJACTOP_H_
#  include "RajActOp.h"
#endif

#ifndef ACTIONH_H_
#  include <Arjuna/ActionH.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

ServerRajdootAction_rpc::ServerRajdootAction_rpc()
{
/*
 * #ifdef DEBUG
 *     debug_stream.lock();
 *     debug_stream << CONSTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
 *     debug_stream << "ServerRajdootAction_rpc::ServerRajdootAction_rpc()" << endl;
 *     debug_stream.unlock();
 * #endif
 */
}

ServerRajdootAction_rpc::~ServerRajdootAction_rpc()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdootAction_rpc::~ServerRajdootAction_rpc()" << endl;
    debug_stream.unlock();
    
#endif
}

int ServerRajdootAction_rpc::initialise(int argc, char *argv[])
{
/* #ifdef DEBUG
 *     debug_stream.lock();
 *     debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
 *     debug_stream << "ServerRajdootAction_rpc::initialise(int argc, char *argv[])" << endl;
 *     debug_stream.unlock();
 * #endif
 */
    return ServerRajdoot_rpc::initialise(argc,argv);
}

void ServerRajdootAction_rpc::doGetWork (Int32& opcode, Buffer& callBuff)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdootAction_rpc::doGetWork (Int32& opcode, Buffer& callBuff)" << endl;
    debug_stream.unlock();
    
#endif
    Boolean ForUser = FALSE;

    while (! ForUser)
    {
	Buffer TmpCall, hierBuff;

	ServerRajdoot_rpc::doGetWork (opcode, TmpCall, hierBuff);

	if (! fixHierarchy(hierBuff))
	{
	    Buffer result;
		
	    ServerRajdoot_rpc::sendResult (RAO_NOTDONE, result);
	}
	else
	    switch (opcode)
	    {
	    case RAO_NESTEDABORT:
		doLocalNestedAbort(TmpCall);
		break;
	    case RAO_NESTEDCOMMIT:
		doLocalNestedCommit(TmpCall);
		break;
	    case RAO_NESTEDPREPARE:
		doLocalNestedPrepare(TmpCall);
		break;
	    case RAO_TOPLEVELABORT:
		doLocalTopLevelAbort(TmpCall);
		break;
	    case RAO_TOPLEVELCOMMIT:
		doLocalTopLevelCommit(TmpCall);
		break;
	    case RAO_TOPLEVELPREPARE:
		doLocalTopLevelPrepare(TmpCall);
		break;

	    default:
		callBuff = TmpCall;
		ForUser = TRUE;
		break;
	    }
    }
}

void ServerRajdootAction_rpc::doGetWork ( Int32& opcode, 
					  Buffer& call1, 
					  Buffer& call2)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdootAction_rpc::doGetWork (Int32& opcode, Buffer& call1, Buffer& call2)" << endl;
    debug_stream.unlock();
    
#endif

    error_stream << WARNING << "ServerRajdootAction_rpc::doGetWork (Int32& opcode, Buffer& call1, Buffer& call2) : Call!" << endl;
    ServerRajdoot_rpc::doGetWork (opcode, call1, call2);
}

void ServerRajdootAction_rpc::sendResult ( Int32 errcode, Buffer& result)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdootAction_rpc::sendResult (Int32 errcode, Buffer& result)" << endl;
    debug_stream.unlock();
    
#endif

    ServerRajdoot_rpc::sendResult (errcode, result);
}

void ServerRajdootAction_rpc::sendResult ( Int32 errcode, 
					   Buffer& result1, 
					   Buffer& result2)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdootAction_rpc::sendResult (Int32 errcode, Buffer& result1, Buffer& result2)" << endl;
    debug_stream.unlock();
    
#endif
    
    error_stream << WARNING << "ServerRajdootAction_rpc::sendResult (Int32 errcode, Buffer& result1, Buffer& result2) : Call!\n";
    ServerRajdoot_rpc::sendResult (errcode, result1, result2);
}

//
// Private operation to preform abort, commit & prepare
// on the current nested/top-level actomic action.
//

Boolean ServerRajdootAction_rpc::fixHierarchy ( Buffer& hierBuff )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::fixHierarchy(Buffer& hierBuff)" << endl;
    debug_stream.unlock();
    
#endif

    int  hierSize = 0;
    char *coordinator = 0;
    
    if (! hierBuff.unpack(coordinator))
	return FALSE;

    ServerAtomicAction::setCoordinNodename(coordinator);

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::fixHierarchy() Coordinator Node Name \"" << coordinator << "\"" << endl;
    debug_stream.unlock();
    
#endif

#ifndef GCC_ARRAY_DELETE_BUG
    ::delete [] coordinator;
#else
    ::delete coordinator;
#endif

    Boolean result = FALSE;
    
    if ((result = hierBuff.unpack(hierSize)))
    {
	ActionHierarchy newHierarchy(hierSize);
	
	if (hierSize > 0)
	    result = newHierarchy.unpack(hierBuff);

	if (result)
	    newHierarchy.establishHierarchy();

    }

    return result;
}

void ServerRajdootAction_rpc::doLocalNestedAbort(Buffer& callBuff)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::doLocalNestedAbort(Buffer& callBuff)" << endl;
    debug_stream.unlock();
    
#endif

    Int32   errcode = RAO_NOTDONE;
    Buffer result;

    Uid AA;
    AA.unpack(callBuff);

    if (ServerAtomicAction::currentSAA()->doNestedAbort(AA))
	errcode = RAO_DONE;
    else
	errcode = RAO_NOTDONE;

    ServerRajdoot_rpc::sendResult (errcode, result);
}

void ServerRajdootAction_rpc::doLocalNestedCommit(Buffer& callBuff)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::doLocalNestedCommit(Buffer& callBuff)" << endl;
    debug_stream.unlock();
    
#endif

    Int32   errcode = RAO_NOTDONE;
    Buffer result;

    Uid AA;
    AA.unpack(callBuff);

    if (ServerAtomicAction::currentSAA()->doNestedCommit(AA))
	errcode = RAO_DONE;
    else
	errcode = RAO_NOTDONE;

    ServerRajdoot_rpc::sendResult (errcode, result);
}

void ServerRajdootAction_rpc::doLocalNestedPrepare(Buffer& callBuff)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::doLocalNestedPrepare(Buffer& callBuff)" << endl;
    debug_stream.unlock();
    
#endif

    Int32           errcode = RAO_NOTDONE;
    Buffer         result;
    PrepareOutcome preoutcome;

    Uid AA;
    AA.unpack(callBuff);

    if (ServerAtomicAction::currentSAA()->doNestedPrepare(AA, preoutcome))
    {
	int pctmp = (int) preoutcome;

	errcode = RAO_DONE;
	result.pack(pctmp);
    }
    else
	errcode = RAO_NOTDONE;

    ServerRajdoot_rpc::sendResult (errcode, result);
}

void ServerRajdootAction_rpc::doLocalTopLevelAbort(Buffer& callBuff)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::doLocalTopLevelAbort(Buffer& callBuff)" << endl;
    debug_stream.unlock();
    
#endif

    Int32   errcode = RAO_NOTDONE;
    Buffer result;

    Uid AA;
    AA.unpack(callBuff);

    if (ServerAtomicAction::currentSAA()->doTopLevelAbort(AA))
	errcode = RAO_DONE;
    else
	errcode = RAO_NOTDONE;

    ServerRajdoot_rpc::sendResult (errcode, result);
}

void ServerRajdootAction_rpc::doLocalTopLevelCommit(Buffer& callBuff)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::doLocalTopLevelCommit(Buffer& callBuff)" << endl;
    debug_stream.unlock();
    
#endif

    Int32   errcode = RAO_NOTDONE;
    Buffer result;

    Uid AA;
    AA.unpack(callBuff);

    if (ServerAtomicAction::currentSAA()->doTopLevelCommit(AA))
	errcode = RAO_DONE;
    else
	errcode = RAO_NOTDONE;

    ServerRajdoot_rpc::sendResult (errcode, result);
}

void ServerRajdootAction_rpc::doLocalTopLevelPrepare(Buffer& callBuff)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::doLocalTopLevelPrepare(Buffer& callBuff)" << endl;
    debug_stream.unlock();
    
#endif

    Int32   errcode = RAO_NOTDONE;
    Buffer result;
    PrepareOutcome preoutcome;

    Uid AA;
    AA.unpack(callBuff);

    if (ServerAtomicAction::currentSAA()->doTopLevelPrepare(AA, preoutcome))
    {
	int pctmp = (int) preoutcome;

	errcode = RAO_DONE;
	result.pack(pctmp);
    }
    else
	errcode = RAO_NOTDONE;

    ServerRajdoot_rpc::sendResult (errcode, result);
}
