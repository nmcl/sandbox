/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AARecordNative.java,v 1.2 1998/07/06 10:49:43 nmcl Exp $
 */

package com.arjuna.JavaCosRecovery.AADoctor;

import com.arjuna.JavaCosRecovery.CrashMan.*;
import com.arjuna.JavaGandiva.Common.Uid;
import com.arjuna.JavaArjunaLite.Atomic.*;
import java.io.PrintStream;

/*
 * Class: AARecordNative
 * Responsible for the recovery of local atomic actions
 */

/*
 * Need multiple implementations of this type because of the different
 * ways in which we can attempt to resolve shadowed states. The Arjuna
 * style is to use the process id of the process which created the action
 * and determine whether it is still alive. We cannot do this in Java without
 * native methods. Hence we have an implementation which uses pure Java and
 * one which uses native methods. There are trade offs: the latter cannot
 * tolerate process level failures, i.e., it will not recover states until
 * after the entire machine has crashed and recovered (assuming the state
 * recovery is not forced by an administrator.)
 *
 * So, we will have interface/implementation separation for this type of
 * record.
 */

public class AARecordNative extends CrashRecRecord
{
    
public AARecordNative ( PrintStream strm, boolean safe, Uid actUid )
    {
	super(strm, safe);

	AAUid = new Uid(actUid);
	status = AAStatus.UNRESOLVED_AA;
    }

public boolean resolve ()
    {
	if (getRecoveryStatus() == AAStatus.COMPLETE)
	    return true;

	outStrm.println("AARecordNative: Beginning recovery ("+AAUid+")");

	AtomicAction aa = new AtomicAction(AAUid);

	if ( !aa.activate() )
	{
	    outStrm.println("AARecordNative: Action was transient - no work necessary");
	    setRecoveryStatus(AAStatus.COMPLETE);
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

	    if (processAlive(aa.getCreatorPid()))
	    {
		// The process associated with this action is dead, commit it
		
		aa.crashRecoveryCommit();
		status = AAStatus.COMMITTED_AA;
	    }
	    
	    setRecoveryStatus(AAStatus.COMPLETE);
	}

	boolean success = ( (getRecoveryStatus() == AAStatus.COMPLETE) ? true : false );

	if (success)
	    outStrm.println("AARecordNative: Recovery successful ("+AAUid+")");
	else
	    outStrm.println("AARecordNative: Recovery unsuccessful ("+AAUid+")");
	
	checkAlarms();
	
	return success;
    }

public void print (PrintStream strm)
    {
	super.print(strm);
	strm.println("AARecordNative for uid : "+AAUid);
	strm.println("AARecordNative Status = "+status);
    }

public void id (PrintStream strm)
    {
	strm.println("AARecordNative for AA uid = "+AAUid);
    }

public int type ()
    {
	return AARECORD;
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

private static final int AAThreshold = 0;
    
};
