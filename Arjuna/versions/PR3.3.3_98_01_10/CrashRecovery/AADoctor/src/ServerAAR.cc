/*
 * Copyright (C) 1993, 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerAAR.cc,v 1.10 1994/10/10 11:12:58 ngdp Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef OBJSTATE_H_
#include <Arjuna/ObjState.h>
#endif

#ifndef LOCALOSTYPES_H_
#  include <ObjectStore/LocalOSTypes.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif 

#ifndef MAILER_H_
#  include <Mailer.h>
#endif

#include <AAStatus_stub.h>

#ifndef SERVERAAR_H_
#  include <ServerAAR.h>
#endif

/*
 * Enum: Server Atomic Action Status (SAAStatus)
 * Status of a server stomic action record
 */

ostream& operator<< (ostream& strm, SAAStatus s)
{
    switch (s)
    {
        case UNRESOLVED_SAA:
            strm << "unresolved";
            break;
        case COMMIT_SAA:
            strm << "to be committed";
            break;
        case ABORT_SAA:
            strm << "to be aborted";
            break;
        case IGNORE_SAA:
            strm << "to be ignored";
            break;
        case COMMITTED_SAA:
            strm << "committed";
            break;
        case ABORTED_SAA:
            strm << "aborted";
            break;
        default:
            strm << "unknown";
            break;
    }
    return strm;
}

/*
 * Class: ServerAARecord
 * Responsible for the recovery of server atomic actions
 */

/*
 * Initialise statics
 */

int ServerAARecord::SAAThreshold = 2;

//-Public-Functions------------------------------------------------------------

ServerAARecord::ServerAARecord ( ostream& strm, Boolean safe,
                                 const Uid& actUid)
                               : CrashRecRecord(strm, safe),
                                 serverAAUid(actUid),
                                 coordin_nodename(0),
                                 recoveringHost(0),
                                 status(UNRESOLVED_SAA)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ServerAARecord::ServerAARecord("
                 << actUid << ")" << endl;
#endif
}

ServerAARecord::ServerAARecord ( ostream& strm, Boolean safe, 
                                 const Uid& actUid, char *host )
                               : CrashRecRecord(strm, safe),
                                 serverAAUid(actUid),
                                 coordin_nodename(0),
                                 recoveringHost(host),
                                 status(UNRESOLVED_SAA)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ServerAARecord::ServerAARecord("
                 << actUid << "," << host << ")" << endl;
#endif
}

ServerAARecord::~ServerAARecord()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ServerAARecord::~ServerAARecord()" << endl;
#endif
}

Boolean ServerAARecord::resolve()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ServerAARecord::resolve()" << endl;
#endif

    if (getRecoveryStatus() == COMPLETE)
        return(TRUE);

    outStrm << "SAARecord: Beginning recovery (" << serverAAUid << ")"
            << endl;

    ServerAtomicAction saa(serverAAUid);

    if ( !(saa.activate()) )
    {
        /*
         * Activate's failure indicates that the object is no longer around.
         * The server process could have been alive when it was first detected
         * and has subsequently removed it. We can't do anymore.
         */

        status = IGNORE_SAA;
    }    
    else
    {
        // We've successfully activated the state
        coordin_nodename = saa.getCoordinNodename();
	
	if ( (coordin_nodename == 0) ||
	     ((::strcmp(coordin_nodename, "") == 0)) )
	{
	    outStrm << "SAARecord: *WARNING* "
		    << "Cannot determine co-ordinator, cannot recover action."
		    << endl;
	}
	else
	{
            if (compareHosts(coordin_nodename, recoveringHost))
            {
		outStrm << "SAARecord: Recovering node (" << recoveringHost
			<< ") is the co-ordinating node" << endl;
		getLocalAAStatus(saa.getSavingUid());
	    }
	    else
	    {
		getRemoteAAStatus(saa.getSavingUid());
	    }
	}
    }

    switch(status)
    {
        case COMMIT_SAA:
        {
            outStrm << "SAARecord: Action should be committed" << endl;

            if ( saa.doTopLevelCommit(serverAAUid) )
            {
                outStrm << "SAARecord: Action committed successfully" << endl;
                status = COMMITTED_SAA;
                setRecoveryStatus(COMPLETE);
            }
            else
            {
                outStrm << "SAARecord: *WARNING* Action commit failed" << endl;
            }
        }
        break;

        case ABORT_SAA:
        {
            outStrm << "SAARecord: Action should be aborted" << endl;

            if ( saa.doTopLevelAbort(serverAAUid) )
            {
                outStrm << "SAARecord: Action aborted successfully" << endl;
                status = ABORTED_SAA;
                setRecoveryStatus(COMPLETE);
            }
            else
            {
                 outStrm << "SAARecord: *WARNING* Action abort failed" << endl;
            }
        }
        break;

        case IGNORE_SAA:
        {
            outStrm << "SAARecord: Action was transient - no recovery required"
                    << endl;
            setRecoveryStatus(COMPLETE);
        }
        break;

        default:
            break;
    }

    Boolean success = ( (getRecoveryStatus() == COMPLETE) ? TRUE : FALSE );

    if(success)
        outStrm << "SAARecord: Recovery successful ("
                << serverAAUid << ")" << endl;
    else
        outStrm << "SAARecord: Recovery unsuccessful ("
                << serverAAUid << ")" << endl;

    checkAlarms();
    return (success);
}


void ServerAARecord::print(ostream& strm)
{
    CrashRecRecord::print(strm);
    strm << "ServerAARecord for uid : " << serverAAUid << endl;
    strm << "ServerAARecord Status = " << status << endl;
}

void ServerAARecord::id(ostream& strm)
{
    strm << "ServerAARecord for SAA uid = " << serverAAUid << endl;
}

const CRRecordType ServerAARecord::type() const
{
    return SAARECORD;
}

const Uid& ServerAARecord::value() const
{
    return(serverAAUid);
}

//-Private-Functions-----------------------------------------------------------

void ServerAARecord::getRemoteAAStatus(const Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "ServerAARecord::getRemoteAAStatus( " << u << ")" << endl;
#endif

    /*
     * Try to find the status of the AtomicAction at the co-ord node.
     * Override the default handler to prevent core dumps in the case of
     * unsuccessful initiate
     */

    outStrm << "SAARecord: About to activate AAStatus server on "
            << coordin_nodename << endl;

    ClientRpc* RpcClientAAS = new ClientRpc("AAStatus");
    RpcClientAAS->setHost(coordin_nodename);
    old_handler = setHandler(CrashRecRecord::rpc_handler);
    ClientRpcManager* RpcConAAS = new ClientRpcManager(RpcClientAAS);
    int res = 0;
    AAStatus *coordAct = new AAStatus(res, RpcConAAS);

    if (res)
    {
        ActionCondition ac = coordAct->isCommitted(u, foundAtReboot);
        switch(ac)
        {
            case ACTION_COMMITTED:
                status = COMMIT_SAA;
                break;
            case ACTION_ABORTED:
                status = ABORT_SAA;
                break;
            case ACTION_ATTACHED:
                // If the action is still in progress, no work is necessary
                status = IGNORE_SAA;
                break;
            default:
                outStrm << "SAARecord: *WARNING* "
                        << "Unexpected result from AAStatus server" << endl;
                break;
        }
    }
    else
    {
        /*
         * Unable to contact co-ordinator, must retry later
         */

        outStrm << "SAARecord: *WARNING* Failed to start AAStatus server on "
                << coordin_nodename << endl;

        status = UNRESOLVED_SAA;
    }

    // Tidy up the RPC stuff
    if (coordAct)
        delete coordAct;

    if (RpcConAAS)
        delete RpcConAAS;

    if (RpcClientAAS)
        delete RpcClientAAS;

    setHandler(old_handler);
}

void ServerAARecord::getLocalAAStatus(const Uid& u)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PRIVATE;
    debug_stream << "ServerAARecord::getLocalAAStatus( " << u << ")" << endl;
#endif
    int res = -1;
    LocalAAStatus aas(res);
    ActionCondition ac = aas.isCommitted(u, foundAtReboot);
    switch(ac)
    {
        case ACTION_COMMITTED:
            status = COMMIT_SAA;
            break;
        case ACTION_ABORTED:
            status = ABORT_SAA;
            break;
        case ACTION_ATTACHED:
            // If the action is still in progress, no work is necessary
            status = IGNORE_SAA;
            break;
        default:
            outStrm << "SAARecord: *WARNING* "
                    << "Unexpected result from AAStatus server" << endl;
            break;
    }
}

void ServerAARecord::setThreshold(int t)
{
    SAAThreshold = t;
}

int ServerAARecord::getThreshold()
{
    return(SAAThreshold);
}

void ServerAARecord::alert()
{
    Mailer m(outStrm);

    m << "Crash recovery has made " << numberAttempts()
      << " attempts to resolve the following server atomic action:" << endl;

    m << "\tServerAtomicAction uid  = " << serverAAUid << endl;
    m << "\tName of co-ordinating host = " << coordin_nodename << endl;
    
    m << "\nAttempts are continuing. "
      << "If this matter is resolved you will be informed." << endl;

    m.sendMessage();
}

void ServerAARecord::calm()
{
    Mailer m(outStrm);

    m << "Crash recovery has successfully resolved the following "
      << "server atomic action\nafter "
      << numberAttempts() << "attempts.\n" << endl;

    m << "\tServerAtomicAction uid  = " << serverAAUid << endl;
    m << "\tName of co-ordinating host = " << coordin_nodename << endl;
    
    m.sendMessage();
    resetAlarms();
}
