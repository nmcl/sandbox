/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerAAR.cc,v 1.7 1993/10/06 13:44:13 ndbi Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef LPOBJSTORE_H_
#  include <Arjuna/LPObjStore.h>
#endif

#ifndef SERVERAA_H_
#  include <Arjuna/ServerAA.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif 

#include "AAStatus_stub.h"

#ifndef SERVERAAR_H_
#  include "ServerAAR.h"
#endif

ServerAARecord::ServerAARecord ( const Uid& actUid )
                               : CrashRecRecord(),
                                 serverAAUid(actUid),
                                 coordin_nodename(0),
                                 status(UNRESOLVED_SAA)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ServerAARecord::ServerAARecord(const Uid&)\n" << flush;
#endif
}

ServerAARecord::~ServerAARecord()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ServerAARecord::~ServerAARecord()\n" << flush;
#endif
}

ostream& ServerAARecord::print ( ostream& strm ) const
{
    CrashRecRecord::print(strm);
    strm << "ServerAARecord for uid : " << serverAAUid << endl;
    strm << "ServerAARecord Status = " << status << endl;
    return strm;
}

ostream& ServerAARecord::id ( ostream& strm ) const
{
    strm << "ServerAARecord for SAA uid = " << serverAAUid << endl;
    return strm;
}

Boolean ServerAARecord::resolve()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_CRASH_RECOVERY << VIS_PUBLIC;
    debug_stream << "ServerAARecord::resolve()\n" << flush;
#endif

    Boolean (*old_handler) ();
    LocalPersistentObjectStore store; 
    ServerAtomicAction saa(serverAAUid);
    saa.activate(&store);
    coordin_nodename = saa.get_coordin_nodename();
    Boolean resolved = FALSE;

    /*
     * Try to find the status of the AtomicAction at the co-ord node.
     * Override the default handler to prevent core dumps in the case of
     * unsuccessful initiate
     */
    // SHOULD PUT RES PARAMETER INTO AASTATUS CALLS

    cout << "\nBEGINNING SAA UPDATE (" << serverAAUid << ")" << endl;

    cout << "About to activate AAStatus server on host "
         << coordin_nodename << endl;

    ClientRpc* RpcClientAAS = new ClientRpc("AAStatus");
    RpcClientAAS->SetHost(coordin_nodename);
    old_handler = setHandler(CrashRecRecord::rpc_handler);
    RpcControl* RpcConAAS = new RpcControl(RpcClientAAS);
    setHandler(old_handler);
    AAStatus *coordAct = new AAStatus(RpcConAAS);

    if (coordAct != 0)
    {
        if ( coordAct->isCommitted(saa.get_saving_uid()) )
        {
            cout << "Server AA (" << serverAAUid 
                 << ") should be committed" << endl;
            status = COMMIT_SAA;

            if ( saa.DoTopLevel_Commit(serverAAUid) )
            {
                cout << "Server AA (" << serverAAUid
                     << ") committed successfully" << endl;
                status = COMMITTED_SAA;
            }
            else
            {
                cerr << "** WARNING ** Server AA (" << serverAAUid
                     << ") commit failed" << endl;
            }
        }
        else
        {
            cout << "Server AA (" << serverAAUid 
                 << ") should be aborted" << endl;
            status = ABORT_SAA;

            if ( saa.DoTopLevel_Abort(serverAAUid) )
            {
                cout << "Server AA (" << serverAAUid
                     << ") aborted successfully" << endl;
                status = ABORTED_SAA;
            }
            else
            {
                cerr << "** WARNING ** Server AA (" << serverAAUid
                     << ") abort failed" << endl;
            }
        }

        if ( (status == COMMITTED_SAA) || (status == ABORTED_SAA) )
        {
            resolved = TRUE;
            cout << "Removing saa with uid " << serverAAUid << ".\n" << flush;
            //store.remove_state (serverAAUid, saa.type());
        }
        delete coordAct;
    }
    else
    {
        /*
         * Unable to contact co-ordinator, must retry later
         */

        cerr << "** WARNING **  Failed to start AAStatus server on host "
             << coordin_nodename << endl;

        status = UNRESOLVED_SAA;
    }

    delete RpcConAAS;
    delete RpcClientAAS;

    if (resolved)
    {
        cout << "SAA UPDATE SUCCESSFUL (" << serverAAUid
             << ")\n" << endl;
        setRecoveryStatus(COMPLETE);
    }
    else
    {
        cout << "SAA UPDATE UNSUCCESSFUL (" << serverAAUid
             << ")\n" << endl;
    }
    return (resolved);
}

