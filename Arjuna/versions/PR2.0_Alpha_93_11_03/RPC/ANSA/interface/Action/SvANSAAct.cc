/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: SvANSAAct.cc,v 1.1 1993/11/03 14:38:39 nmcl Exp $
 */

static const char RCSid[] = "$Id: SvANSAAct.cc,v 1.1 1993/11/03 14:38:39 nmcl Exp $";

/*
 * This class provides C++ interface to the server side of the
 * ANSA rpc protocol.
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

#ifndef ANSA_SVANSAACT_H_
#  include <RPC/ANSA/SvANSAAct.h>
#endif

#ifndef ANSAACTOP_H_
#  include "ANSAActOp.h"
#endif

#ifndef ACTIONH_H_
#  include <Arjuna/ActionH.h>
#endif

#ifndef ANSA_SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

ServerANSAAction_rpc::ServerANSAAction_rpc()
{
/*
 * #ifdef DEBUG
 *     debug_stream << CONSTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
 *     debug_stream << "ServerANSAAction_rpc::ServerANSAAction_rpc()\n" << flush;
 * #endif
 */
}

ServerANSAAction_rpc::~ServerANSAAction_rpc()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerANSAAction_rpc::~ServerANSAAction_rpc()\n" << flush;
#endif
}

int ServerANSAAction_rpc::Initialise(int argc, char *argv[])
{
/* #ifdef DEBUG
 *     debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
 *     debug_stream << "ServerANSAAction_rpc::Initialise(int argc, char *argv[])\n" << flush;
 * #endif
 */
    return ServerANSA_rpc::Initialise(argc,argv);
}

void ServerANSAAction_rpc::GetWork (long& opcode, Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerANSAAction_rpc::GetWork (long& opcode, Buffer& call)\n" << flush;
#endif
    Boolean ForUser = FALSE;

    while (! ForUser)
    {
        {
	    Buffer TmpCall, hierBuff;

	    ServerANSA_rpc::GetWork (opcode, TmpCall, hierBuff);

	    if (! FixHierarchy(hierBuff))
	    {
		Buffer result;
		
		ServerANSA_rpc::SendResult (AAO_NOTDONE, result);
	    }
	    else
		switch (opcode)
		{
	        case AAO_NESTEDABORT:
	            DoLocal_NestedAbort(TmpCall);
		    break;
	        case AAO_NESTEDCOMMIT:
	            DoLocal_NestedCommit(TmpCall);
		    break;
	        case AAO_NESTEDPREPARE:
	            DoLocal_NestedPrepare(TmpCall);
		    break;
	        case AAO_TOPLEVELABORT:
	            DoLocal_TopLevelAbort(TmpCall);
		    break;
	        case AAO_TOPLEVELCOMMIT:
	            DoLocal_TopLevelCommit(TmpCall);
		    break;
	        case AAO_TOPLEVELPREPARE:
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

void ServerANSAAction_rpc::GetWork (long& opcode, Buffer& call1, Buffer& call2)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerANSAAction_rpc::GetWork (long& opcode, Buffer& call1, Buffer& call2)\n" << flush;
#endif

    error_stream << WARNING << "ServerANSAAction_rpc::DoGetWork (long& opcode, Buffer& call1, Buffer& call2) : Call!\n" << flush;
    ServerANSA_rpc::GetWork (opcode, call1, call2);
}

void ServerANSAAction_rpc::SendResult (long errcode, Buffer& result)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerANSAAction_rpc::SendResult (long errcode, Buffer& result)\n" << flush;
#endif

    ServerANSA_rpc::SendResult (errcode, result);
}

void ServerANSAAction_rpc::SendResult (long errcode, Buffer& result1, Buffer& result2)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PUBLIC;
    debug_stream << "ServerANSAAction_rpc::SendResult (long errcode, Buffer& result1, Buffer& result2)\n" << flush;
#endif
    
    error_stream << WARNING << "ServerANSAAction_rpc::SendResult (long errcode, Buffer& result1, Buffer& result2) : Call!\n";
    ServerANSA_rpc::SendResult (errcode, result1, result2);
}

//
// Private operation to preform abort, commit & prepare
// on the current nested/top-level actomic action.
//

Boolean ServerANSAAction_rpc::FixHierarchy(Buffer& hierBuff)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerANSAAction_rpc::FixHierarchy(Buffer& hierBuff)\n" << flush;
#endif
    int  hierSize = 0;
    char *coordinator = 0;

    if (! hierBuff.unpack(coordinator))
	return FALSE;

    ServerAtomicAction::set_coordin_nodename(coordinator);

#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerANSAAction_rpc::FixHierarchy() Coordinator Node Name \"" << coordinator << "\"\n" << flush;
#endif

#ifndef __GNUG__
    delete [] coordinator;
#else
    delete coordinator;
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

void ServerANSAAction_rpc::DoLocal_NestedAbort(Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerANSAAction_rpc::DoLocal_NestedAbort(Buffer& call)\n" << flush;
#endif

    long   errcode = AAO_NOTDONE;
    Buffer result;

    Uid AA;
    AA.unpack(call);

    if (ServerAtomicAction::Current()->DoNested_Abort(AA))
	errcode = AAO_DONE;
    else
	errcode = AAO_NOTDONE;

    ServerANSA_rpc::SendResult (errcode, result);
}

void ServerANSAAction_rpc::DoLocal_NestedCommit(Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerANSAAction_rpc::DoLocal_NestedCommit(Buffer& call)\n" << flush;
#endif

    long   errcode = AAO_NOTDONE;
    Buffer result;

    Uid AA;
    AA.unpack(call);

    if (ServerAtomicAction::Current()->DoNested_Commit(AA))
	errcode = AAO_DONE;
    else
	errcode = AAO_NOTDONE;

    ServerANSA_rpc::SendResult (errcode, result);
}

void ServerANSAAction_rpc::DoLocal_NestedPrepare(Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerANSAAction_rpc::DoLocal_NestedPrepare(Buffer& call)\n" << flush;
#endif

    long           errcode = AAO_NOTDONE;
    Buffer         result;
    PrepareOutcome preoutcome;

    Uid AA;
    AA.unpack(call);

    if (ServerAtomicAction::Current()->DoNested_Prepare(AA, preoutcome))
    {
	int pctmp = (int) preoutcome;

	errcode = AAO_DONE;
	result.pack(pctmp);
    }
    else
	errcode = AAO_NOTDONE;

    ServerANSA_rpc::SendResult (errcode, result);
}

void ServerANSAAction_rpc::DoLocal_TopLevelAbort(Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerANSAAction_rpc::DoLocal_TopLevelAbort(Buffer& call)\n" << flush;
#endif

    long   errcode = AAO_NOTDONE;
    Buffer result;

    Uid AA;
    AA.unpack(call);

    if (ServerAtomicAction::Current()->DoTopLevel_Abort(AA))
	errcode = AAO_DONE;
    else
	errcode = AAO_NOTDONE;

    ServerANSA_rpc::SendResult (errcode, result);
}

void ServerANSAAction_rpc::DoLocal_TopLevelCommit(Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerANSAAction_rpc::DoLocal_TopLevelCommit(Buffer& call)\n" << flush;
#endif

    long   errcode = AAO_NOTDONE;
    Buffer result;

    Uid AA;
    AA.unpack(call);

    if (ServerAtomicAction::Current()->DoTopLevel_Commit(AA))
	errcode = AAO_DONE;
    else
	errcode = AAO_NOTDONE;

    ServerANSA_rpc::SendResult (errcode, result);
}

void ServerANSAAction_rpc::DoLocal_TopLevelPrepare(Buffer& call)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_ACTION_RPC << VIS_PRIVATE;
    debug_stream << "ServerANSAAction_rpc::DoLocal_TopLevelPrepare(Buffer& call)\n" << flush;
#endif

    long   errcode = AAO_NOTDONE;
    Buffer result;
    PrepareOutcome preoutcome;

    Uid AA;
    AA.unpack(call);

    if (ServerAtomicAction::Current()->DoTopLevel_Prepare(AA, preoutcome))
    {
	int pctmp = (int) preoutcome;

	errcode = AAO_DONE;
	result.pack(pctmp);
    }
    else
	errcode = AAO_NOTDONE;

    ServerANSA_rpc::SendResult (errcode, result);
}
