/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: SvRajAct.cc,v 1.24 1993/08/03 11:11:28 nmcl Exp $
 */

static const char RCSid[] = "$Id: SvRajAct.cc,v 1.24 1993/08/03 11:11:28 nmcl Exp $";

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
 *     debug_stream << CONSTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
 *     debug_stream << "ServerRajdootAction_rpc::ServerRajdootAction_rpc()\n" << flush;
 * #endif
 */
}

ServerRajdootAction_rpc::~ServerRajdootAction_rpc()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdootAction_rpc::~ServerRajdootAction_rpc()\n" << flush;
#endif
}

int ServerRajdootAction_rpc::Initialise(int argc, char *argv[])
{
/* #ifdef DEBUG
 *     debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
 *     debug_stream << "ServerRajdootAction_rpc::Initialise(int argc, char *argv[])\n" << flush;
 * #endif
 */
    return ServerRajdoot_rpc::Initialise(argc,argv);
}

void ServerRajdootAction_rpc::DoGetWork (long& opcode, Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdootAction_rpc::DoGetWork (long& opcode, Buffer& call)\n" << flush;
#endif
    Boolean ForUser = FALSE;

    while (! ForUser)
    {
        {
	    Buffer TmpCall, hierBuff;

	    ServerRajdoot_rpc::DoGetWork (opcode, TmpCall, hierBuff);

	    if (! FixHierarchy(hierBuff))
	    {
		Buffer result;
		
		ServerRajdoot_rpc::SendResult (RAO_NOTDONE, result);
	    }
	    else
		switch (opcode)
		{
	        case RAO_NESTEDABORT:
	            DoLocal_NestedAbort(TmpCall);
		    break;
	        case RAO_NESTEDCOMMIT:
	            DoLocal_NestedCommit(TmpCall);
		    break;
	        case RAO_NESTEDPREPARE:
	            DoLocal_NestedPrepare(TmpCall);
		    break;
	        case RAO_TOPLEVELABORT:
	            DoLocal_TopLevelAbort(TmpCall);
		    break;
	        case RAO_TOPLEVELCOMMIT:
	            DoLocal_TopLevelCommit(TmpCall);
		    break;
	        case RAO_TOPLEVELPREPARE:
	            DoLocal_TopLevelPrepare(TmpCall);
		    break;

	        default:
		    call = TmpCall;
	            ForUser = TRUE;
		    break;
		}
        }
    }
}

void ServerRajdootAction_rpc::DoGetWork ( long& opcode, 
					  Buffer& call1, 
					  Buffer& call2)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdootAction_rpc::DoGetWork (long& opcode, Buffer& call1, Buffer& call2)\n" << flush;
#endif

    error_stream << WARNING << "ServerRajdootAction_rpc::DoGetWork (long& opcode, Buffer& call1, Buffer& call2) : Call!\n" << flush;
    ServerRajdoot_rpc::DoGetWork (opcode, call1, call2);
}

void ServerRajdootAction_rpc::SendResult ( long errcode, Buffer& result)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdootAction_rpc::SendResult (long errcode, Buffer& result)\n" << flush;
#endif

    ServerRajdoot_rpc::SendResult (errcode, result);
}

void ServerRajdootAction_rpc::SendResult ( long errcode, 
					   Buffer& result1, 
					   Buffer& result2)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerRajdootAction_rpc::SendResult (long errcode, Buffer& result1, Buffer& result2)\n" << flush;
#endif
    
    error_stream << WARNING << "ServerRajdootAction_rpc::SendResult (long errcode, Buffer& result1, Buffer& result2) : Call!\n";
    ServerRajdoot_rpc::SendResult (errcode, result1, result2);
}

//
// Private operation to preform abort, commit & prepare
// on the current nested/top-level actomic action.
//

Boolean ServerRajdootAction_rpc::FixHierarchy ( Buffer& hierBuff )
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::FixHierarchy(Buffer& hierBuff)\n" << flush;
#endif

    int  hierSize = 0;
    char *coordinator = 0;
    
    if (! hierBuff.unpack(coordinator))
	return FALSE;

    ServerAtomicAction::set_coordin_nodename(coordinator);

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::FixHierarchy() Coordinator Node Name \"" << coordinator << "\"\n" << flush;
#endif

#ifndef __GNUG__
    ::delete [] coordinator;
#else
    ::delete coordinator;
#endif

    if (hierBuff.unpack(hierSize))
    {
	ActionHierarchy newHierarchy(hierSize);
	
	if (newHierarchy.unpack(hierBuff))
	    newHierarchy.establishHierarchy();

	return TRUE;
    }

    return FALSE;
}

void ServerRajdootAction_rpc::DoLocal_NestedAbort(Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::DoLocal_NestedAbort(Buffer& call)\n" << flush;
#endif

    long   errcode = RAO_NOTDONE;
    Buffer result;

    Uid AA;
    AA.unpack(call);

    if (ServerAtomicAction::Current()->DoNested_Abort(AA))
	errcode = RAO_DONE;
    else
	errcode = RAO_NOTDONE;

    ServerRajdoot_rpc::SendResult (errcode, result);
}

void ServerRajdootAction_rpc::DoLocal_NestedCommit(Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::DoLocal_NestedCommit(Buffer& call)\n" << flush;
#endif

    long   errcode = RAO_NOTDONE;
    Buffer result;

    Uid AA;
    AA.unpack(call);

    if (ServerAtomicAction::Current()->DoNested_Commit(AA))
	errcode = RAO_DONE;
    else
	errcode = RAO_NOTDONE;

    ServerRajdoot_rpc::SendResult (errcode, result);
}

void ServerRajdootAction_rpc::DoLocal_NestedPrepare(Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::DoLocal_NestedPrepare(Buffer& call)\n" << flush;
#endif

    long           errcode = RAO_NOTDONE;
    Buffer         result;
    PrepareOutcome preoutcome;

    Uid AA;
    AA.unpack(call);

    if (ServerAtomicAction::Current()->DoNested_Prepare(AA, preoutcome))
    {
	int pctmp = (int) preoutcome;

	errcode = RAO_DONE;
	result.pack(pctmp);
    }
    else
	errcode = RAO_NOTDONE;

    ServerRajdoot_rpc::SendResult (errcode, result);
}

void ServerRajdootAction_rpc::DoLocal_TopLevelAbort(Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::DoLocal_TopLevelAbort(Buffer& call)\n" << flush;
#endif

    long   errcode = RAO_NOTDONE;
    Buffer result;

    Uid AA;
    AA.unpack(call);

    if (ServerAtomicAction::Current()->DoTopLevel_Abort(AA))
	errcode = RAO_DONE;
    else
	errcode = RAO_NOTDONE;

    ServerRajdoot_rpc::SendResult (errcode, result);
}

void ServerRajdootAction_rpc::DoLocal_TopLevelCommit(Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::DoLocal_TopLevelCommit(Buffer& call)\n" << flush;
#endif

    long   errcode = RAO_NOTDONE;
    Buffer result;

    Uid AA;
    AA.unpack(call);

    if (ServerAtomicAction::Current()->DoTopLevel_Commit(AA))
	errcode = RAO_DONE;
    else
	errcode = RAO_NOTDONE;

    ServerRajdoot_rpc::SendResult (errcode, result);
}

void ServerRajdootAction_rpc::DoLocal_TopLevelPrepare(Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerRajdootAction_rpc::DoLocal_TopLevelPrepare(Buffer& call)\n" << flush;
#endif

    long   errcode = RAO_NOTDONE;
    Buffer result;
    PrepareOutcome preoutcome;

    Uid AA;
    AA.unpack(call);

    if (ServerAtomicAction::Current()->DoTopLevel_Prepare(AA, preoutcome))
    {
	int pctmp = (int) preoutcome;

	errcode = RAO_DONE;
	result.pack(pctmp);
    }
    else
	errcode = RAO_NOTDONE;

    ServerRajdoot_rpc::SendResult (errcode, result);
}
