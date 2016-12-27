/*
 * Copyright (C) 1994, 1995, 1996, 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CrashRecRecord.java,v 1.2 1998/07/06 10:49:50 nmcl Exp $
 */

package com.arjuna.JavaCosRecovery.CrashMan;

import com.arjuna.JavaGandiva.Common.Uid;
import java.io.PrintStream;
import java.net.InetAddress;

import java.net.UnknownHostException;

/*
 * Class: Crash Recovery Record (CrashRecRecord)
 * Base class of recovery records. The inherited classes are responsible
 * for performing specific recovery operations. These records are managed
 * by the CRRecordList class.
 */

public abstract class CrashRecRecord
{

public abstract boolean resolve ();
public abstract void id (PrintStream strm);
public abstract int type ();
public abstract Uid value ();
    
public void finalize ()
    {
	if (previous != null)
	    previous.next = next;
    
	if (next != null)
	    next.previous = previous;
    }
    
public void print (PrintStream strm)
    {
	strm.println("Recovery Status = "+recoveryStatus);
    }

public void setRecoveryStatus (int newStatus)
    {
	recoveryStatus = newStatus;
    }

public int getRecoveryStatus ()
    {
	return recoveryStatus;
    }

public boolean equals (CrashRecRecord crr)
    {
	return ( ( type() == crr.type() ) && ( value().equals(crr.value()) ) );
    }

    /*
     * These abstract methods used to be private in Arjuna. However,
     * because of restrictions on the Java language about which types
     * of methods can be overridden (nothing with private scope!) we
     * must make them protected.
     */
    
protected abstract void setThreshold (int t);
protected abstract int getThreshold ();
protected abstract void alert ();
protected abstract void calm ();
    
protected CrashRecRecord (PrintStream strm, boolean safe)
    {
	outStrm = strm;
	foundAtReboot = safe;
	recoveryStatus = RecoveryStatus.NOT_COMPLETE;
	noAttempts = 0;
	alerted = false;
	next = null;
	previous = null;
    }

protected boolean compareHosts (String host1, String host2)
    {
	boolean sameHost = false;

	try
	{
	    InetAddress host1OfficialName = InetAddress.getByName(host1);
	    InetAddress host2OfficialName = InetAddress.getByName(host2);

	    if (host1OfficialName.equals(host2OfficialName))
		sameHost = true;
	}
	catch (UnknownHostException e)
	{
	}

	return sameHost;
    }

protected void checkAlarms ()
    {
	if (getRecoveryStatus() != RecoveryStatus.COMPLETE)
	{
	    noAttempts++;

	    if ( (noAttempts > getThreshold()) && !(alerted) )
	    {
		alert(); // fire alert routine
		alerted = true;
	    }
	}
	else
	{
	    if (alerted)
	    {
		calm(); // fire calm routine
	    }
	}
    }

protected void resetAlarms ()
    {
	alerted = false;
	noAttempts = 0;
    }

protected int numberAttempts ()
    {
	return noAttempts;
    }

protected PrintStream outStrm;
protected boolean foundAtReboot;
protected CrashRecRecord next;
protected CrashRecRecord previous;

    /*
     * What is the point of these methods when they are not used?
     */
    
private void setNext (CrashRecRecord crr)
    {
	next = crr;
    }

private void setPrevious (CrashRecRecord crr)
    {
	previous = crr;
    }

private int recoveryStatus;
private int noAttempts;
private boolean alerted;

}
