/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AARecord.java,v 1.3 1998/07/06 10:49:43 nmcl Exp $
 */

package com.arjuna.JavaCosRecovery.AADoctor;

import com.arjuna.JavaCosRecovery.CrashMan.*;
import com.arjuna.JavaCosRecovery.AAStatus.AAStatus;
import com.arjuna.JavaGandiva.Common.Uid;
import com.arjuna.JavaArjunaLite.Atomic.*;
import java.io.PrintStream;

/*
 * Class: AARecord
 * Responsible for the recovery of local atomic actions
 */

public class AARecord extends CrashRecRecord
{
    
public AARecord ( PrintStream strm, boolean safe, Uid actUid )
    {
	super(strm, safe);

	AAUid = new Uid(actUid);
	
	status = AAStatus.UNRESOLVED_AA;
    }

public boolean resolve ()
    {
	/*
	 * If we were discovered after reboot then do nothing.
	 * Fake it!!
	 */
	
	if (!super.foundAtReboot)
	    return true;
	
	if (getRecoveryStatus() == RecoveryStatus.COMPLETE)
	    return true;

	outStrm.println("AARecord: Beginning recovery ("+AAUid+")");

	RecoveryAction aa = new RecoveryAction(AAUid);

	if ( !aa.activate() )
	{
	    outStrm.println("AARecord: Action was transient - no work necessary");
	    setRecoveryStatus(RecoveryStatus.COMPLETE);
	}
	else
	{
	    /*
	     * This only makes sense for process level failures. Unless
	     * we want to use native methods, it will be difficult to determine
	     * what the process was that created the action, and whether it
	     * is still alive.
	     *
	     * Therefore, currently we assume only node level recovery, and
	     * can safely assume that the process has failed!
	     */

	    int fakeProcessId = 0;
	    
	    if (processAlive(fakeProcessId))  // aa.getCreatorPid()
	    {
		// The process associated with this action is dead, commit it
		
		aa.crashRecoveryCommit();
		status = AAStatus.COMMITTED_AA;
	    }
	    
	    setRecoveryStatus(RecoveryStatus.COMPLETE);
	}

	boolean success = ( (getRecoveryStatus() == RecoveryStatus.COMPLETE) ? true : false );

	if (success)
	    outStrm.println("AARecord: Recovery successful ("+AAUid+")");
	else
	    outStrm.println("AARecord: Recovery unsuccessful ("+AAUid+")");
	
	checkAlarms();
	
	return success;
    }

public void print (PrintStream strm)
    {
	super.print(strm);
	strm.println("AARecord for uid : "+AAUid);
	strm.println("AARecord Status = "+status);
    }

public void id (PrintStream strm)
    {
	strm.println("AARecord for AA uid = "+AAUid);
    }

public int type ()
    {
	return CRRecordType.AARECORD;
    }

public Uid value ()
    {
	return AAUid;
    }

protected void setThreshold (int t)
    {
	AAThreshold = t;
    }

protected int getThreshold ()
    {
	return AAThreshold;
    }

protected void alert ()
    {
	// native methods
	
	/*
	Mailer m = new Mailer(outStrm);

	m.writeln("Crash recovery has made "+numberAttempts()+" attempts to resolve a local atomic action");
	m.writeln("with uid = "+AAUid);
	m.writeln("\nAttempts are continuing. "+"If this matter is resolved you will be informed.");

	m.sendMessage();
	*/
    }

protected void calm ()
    {
	// native methods
	
	/*
	Mailer m = new Mailer(outStrm);

	m.writeln("Re: Atomic Action with uid = "+AAUid);
	m.writeln("After "+numberAttempts()+"attempts crash recovery has "+" successfully resolved this action.");
    
	m.sendMessage();
	*/
	
	resetAlarms();
    }

private boolean processAlive (int processId)
    {
	return true;
    }
    
private Uid AAUid;
private int status;

private static int AAThreshold = 0;
    
};
