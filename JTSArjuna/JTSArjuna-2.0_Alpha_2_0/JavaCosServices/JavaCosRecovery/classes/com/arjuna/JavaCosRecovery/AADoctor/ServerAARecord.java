/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerAARecord.java,v 1.3 1998/07/06 10:49:45 nmcl Exp $
 */

package com.arjuna.JavaCosRecovery.AADoctor;

import com.arjuna.JavaCosRecovery.CrashMan.*;
import com.arjuna.JavaCosRecovery.AAStatus.*;
import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaArjunaLite.Atomic.*;
import java.io.PrintStream;

import JavaArjunaLite.Common.ObjectStoreException;

/*
 * Class: ServerAARecord
 * Responsible for the recovery of server atomic actions
 */

public class ServerAARecord extends CrashRecRecord
{
    
public ServerAARecord ( PrintStream strm, boolean safe, Uid actUid)
    {
	super(strm, safe);

	serverAAUid = new Uid(actUid);
	coordin_nodename = null;
	recoveringHost = null;
	status = SAAStatus.UNRESOLVED_SAA;
    }

public ServerAARecord (PrintStream strm, boolean safe, Uid actUid, String host)
    {
	super(strm, safe);

	serverAAUid = new Uid(actUid);
	coordin_nodename = null;
	recoveringHost = host;
	status = SAAStatus.UNRESOLVED_SAA;
    }

public boolean resolve ()
    {
	if (getRecoveryStatus() == RecoveryStatus.COMPLETE)
	    return true;

	outStrm.println("SAARecord: Beginning recovery ("+serverAAUid+")");

	ServerAtomicAction saa = new ServerAtomicAction(serverAAUid);

	if ( !(saa.activate()) )
	{
	    /*
	     * Activate's failure indicates that the object is no longer
	     * around. The server process could have been alive when it was
	     * first detected and has subsequently removed it. We can't do
	     * anymore.
	     */

	    status = SAAStatus.IGNORE_SAA;
	}
	else
	{
	    // We've successfully activated the state
	    coordin_nodename = saa.getCoordinNodename();
	
	    if ( (coordin_nodename == null) ||
		 ((coordin_nodename.compareTo("") == 0)) )
	    {
		outStrm.println("SAARecord: *WARNING* "+"Cannot determine co-ordinator, cannot recover action.");
	    }
	    else
	    {
		if (compareHosts(coordin_nodename, recoveringHost))
		{
		    outStrm.println("SAARecord: Recovering node ("+recoveringHost+") is the co-ordinating node");
		    getLocalAAStatus(saa.getSavingUid());
		}
		else
		{
		    getRemoteAAStatus(saa.getSavingUid());
		}
	    }
	}

	switch (status)
	{
        case SAAStatus.COMMIT_SAA:
	    {
		outStrm.println("SAARecord: Action should be committed");

		if ( saa.doTopLevelCommit(serverAAUid) )
		{
		    outStrm.println("SAARecord: Action committed successfully");
		    status = SAAStatus.COMMITTED_SAA;
		    setRecoveryStatus(RecoveryStatus.COMPLETE);
		}
		else
		{
		    outStrm.println("SAARecord: *WARNING* Action commit failed");
		}
	    }
        break;
        case SAAStatus.ABORT_SAA:
	    {
		outStrm.println("SAARecord: Action should be aborted");

		if ( saa.doTopLevelAbort(serverAAUid) )
		{
		    outStrm.println("SAARecord: Action aborted successfully");
		    status = SAAStatus.ABORTED_SAA;
		    setRecoveryStatus(RecoveryStatus.COMPLETE);
		}
		else
		{
		    outStrm.println("SAARecord: *WARNING* Action abort failed");
		}
	    }
        break;
        case SAAStatus.IGNORE_SAA:
	    {
		outStrm.println("SAARecord: Action was transient - no recovery required");
		setRecoveryStatus(RecoveryStatus.COMPLETE);
	    }
        break;
        default:
            break;
	}

	boolean success = ( (getRecoveryStatus() == RecoveryStatus.COMPLETE) ? true : false );

	if (success)
	    outStrm.println("SAARecord: Recovery successful ("+serverAAUid+")");
	else
	    outStrm.println("SAARecord: Recovery unsuccessful ("+serverAAUid+")");

	checkAlarms();
	
	return success;
    }

public void print (PrintStream strm)
    {
	super.print(strm);
	strm.println("ServerAARecord for uid : "+serverAAUid);
	strm.println("ServerAARecord Status = "+status);
    }

public void id (PrintStream strm)
    {
	strm.println("ServerAARecord for SAA uid = "+serverAAUid);
    }

public int type ()
    {
	return CRRecordType.SAARECORD;
    }

public Uid value ()
    {
	return serverAAUid;
    }

protected void setThreshold (int t)
    {
	SAAThreshold = t;
    }

protected int getThreshold ()
    {
	return SAAThreshold;
    }

protected void alert ()
    {
	Mailer m = new Mailer(outStrm);

	m.println("Crash recovery has made "+numberAttempts()
		  +" attempts to resolve the following server atomic action:");
	m.println("\tServerAtomicAction uid  = "+serverAAUid);
	m.println("\tName of co-ordinating host = "+coordin_nodename);
    
	m.println("\nAttempts are continuing. "+
		  "If this matter is resolved you will be informed.");

	m.sendMessage();
    }

protected void calm ()
    {
	Mailer m = new Mailer(outStrm);

	m.println("Crash recovery has successfully resolved the following "
		  +"server atomic action\nafter "
		  +numberAttempts()+"attempts.\n");

	m.println("\tServerAtomicAction uid  = "+serverAAUid);
	m.println("\tName of co-ordinating host = "+coordin_nodename);
    
	m.sendMessage();
	
	resetAlarms();
    }
    
private void getRemoteAAStatus (Uid u)
    {
	/*
	 * Try to find the status of the AtomicAction at the co-ord node.
	 * Override the default handler to prevent core dumps in the case of
	 * unsuccessful initiate
	 */

	outStrm.println("SAARecord: About to activate AAStatus server on "+coordin_nodename);

	/*
	ClientRpc RpcClientAAS = new ClientRpc("AAStatus");
	RpcClientAAS.setHost(coordin_nodename);
	ClientRpcManager RpcConAAS = new ClientRpcManager(RpcClientAAS);
	*/

	int res = 0;
	
	/*
	 * Needs reworking. Can only do when stub generator updated.
	 
	AAStatus coordAct = new AAStatus(res, RpcConAAS);
	*/

	AAStatus coordAct = null;
	
	res = 0;
	
	if (res != 0)
	{
	    //	    int ac = coordAct.isCommitted(u, foundAtReboot);
	    int ac = ActionCondition.ACTION_ATTACHED;  // just to compile it.
	    
	    switch (ac)
	    {
            case ActionCondition.ACTION_COMMITTED:
                status = SAAStatus.COMMIT_SAA;
                break;
            case ActionCondition.ACTION_ABORTED:
                status = SAAStatus.ABORT_SAA;
                break;
            case ActionCondition.ACTION_ATTACHED:
                // If the action is still in progress, no work is necessary
                status = SAAStatus.IGNORE_SAA;
                break;
            default:
                outStrm.println("SAARecord: *WARNING* "+"Unexpected result from AAStatus server");
                break;
	    }
	}
	else
	{
	    /*
	     * Unable to contact co-ordinator, must retry later
	     */

	    outStrm.println("SAARecord: *WARNING* Failed to start AAStatus server on "+coordin_nodename);
	    
	    status = SAAStatus.UNRESOLVED_SAA;
	}

	// Tidy up the RPC stuff

	/*
	coordAct = null;
	RpcConAAS = null;
	RpcClientAAS = null;
	*/
    }

private void getLocalAAStatus (Uid u)
    {
	int res = -1;
	AAStatus aas = new AAStatus(res);
	int ac = aas.isCommitted(u, foundAtReboot);
	
	switch(ac)
	{
        case ActionCondition.ACTION_COMMITTED:
            status = SAAStatus.COMMIT_SAA;
            break;
        case ActionCondition.ACTION_ABORTED:
            status = SAAStatus.ABORT_SAA;
            break;
        case ActionCondition.ACTION_ATTACHED:
            // If the action is still in progress, no work is necessary
            status = SAAStatus.IGNORE_SAA;
            break;
        default:
            outStrm.println("SAARecord: *WARNING* "+"Unexpected result from AAStatus server");
            break;
	}
    }

private Uid serverAAUid;
private String coordin_nodename;
private String recoveringHost;
private int status;

private static int SAAThreshold = 2;

}
