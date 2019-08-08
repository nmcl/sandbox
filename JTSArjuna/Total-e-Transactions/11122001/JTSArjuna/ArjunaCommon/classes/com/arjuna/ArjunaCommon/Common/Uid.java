/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Uid.java,v 1.3.2.1.6.2.2.1.6.3.16.4.2.3.2.2.4.1.2.1.28.1.4.1 2001/06/28 13:59:04 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.net.InetAddress;
import java.lang.Math;
import java.lang.Cloneable;
import java.io.Serializable;
import java.io.PrintStream;

import java.io.IOException;
import java.net.UnknownHostException;
import java.lang.StringIndexOutOfBoundsException;
import java.lang.NumberFormatException;
import java.lang.CloneNotSupportedException;
import java.lang.OutOfMemoryError;

/**
 * Implements a unique identity class.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Uid.java,v 1.3.2.1.6.2.2.1.6.3.16.4.2.3.2.2.4.1.2.1.28.1.4.1 2001/06/28 13:59:04 nmcl Exp $
 * @since JTS 1.0.
 */

public class Uid implements Cloneable, Serializable
{

    /**
     * Create a new instance.
     */

public Uid ()
    {
	hostAddr = -1;
	process = 0;
	sec = 0;
	other = 0;
	_hashValue = -1;	
	_valid = true;
	
    	/*
	 * Do licence checking first.
         */

	LicenceError le = null;
	
	if (!Uid.ticilDone)
	{
	    try
	    {
		Uid._enabled = com.arjuna.ArjunaCommon.Core.Lector.confirm(com.arjuna.ArjunaCommon.Core.LicenceType.TRANSACTION_CORE);

		Uid.ticilDone = true;
	    }
	    catch (LicenceError e)
	    {
		/*
		 * Delay any fatal exception until later!
		 */

		Uid.ticilDone = false;
		
		le = new LicenceError(e.toString());
	    }
	}

	try
	{
	    hostAddr = Utility.hostInetAddr();	/* calculated only once */
	    process = Utility.getpid();

	    if (Uid.initTime == 0)    
		Uid.initTime = (int) (System.currentTimeMillis()/1000);

	    sec = Uid.initTime;

	    other = Uid.getValue();
	}
	catch (UnknownHostException e)
	{
	    ErrorStream.warning().println("Uid - cannot get local host.");
	    _valid = false;
	}

	/*
	 * If we have a licence problem then throw an error now to
	 * try to obfuscate where we did the checking!
	 */

	if (!Uid._enabled)
	{
	    if (le != null)
		throw le;
	    
	    ErrorStream.fatal().println("Arjuna licence information is illegal. Please apply for a new licence.");

	    Thread.currentThread().stop();
	    
	    // just in case we get here!

	    throw new com.arjuna.ArjunaCommon.Common.LicenceError("Arjuna licence information is illegal. Please apply for a new licence.");
	}
    }

    /**
     * Create a copy of the specified identifier.
     */

public Uid (Uid copyFrom)
    {
	copy(copyFrom);
    }

    /**
     * Create Uid from string representation. If the string does not
     * represent a valid Uid then the instance will be set to nullUid.
     */

public Uid (String uidString)
    {
	this(uidString, true);
    }

    /**
     * Create Uid from string representation. boolean arg says whether
     * to give up if an error is detected or to simply replace with nullUid.
     */
    
public Uid (String uidString, boolean errsOk)
    {
	char theBreakChar = Uid.getBreakChar(uidString);
	
	hostAddr = -1;
	process = 0;
	sec = 0;
	other = 0;
	_hashValue = -1;
	_valid = false;

	if (uidString.length() > 0)
	{
	    int startIndex = 0;
	    int endIndex = 0;
	    String s = null;

	    try
	    {
		while (uidString.charAt(endIndex) != theBreakChar)
		    endIndex++;

		s = uidString.substring(startIndex, endIndex);
		hostAddr = (int) Utility.hexStringToLong(s);
		
		startIndex = endIndex+1;
		endIndex++;
	
		while (uidString.charAt(endIndex) != theBreakChar)
		    endIndex++;

		s = uidString.substring(startIndex, endIndex);
		process = (int) Utility.hexStringToLong(s);
	    	
		startIndex = endIndex+1;
		endIndex++;
	
		while (uidString.charAt(endIndex) != theBreakChar)
		    endIndex++;

		s = uidString.substring(startIndex, endIndex);
		sec = (int) Utility.hexStringToLong(s);

		s = uidString.substring(endIndex+1, uidString.length());
		other = (int) Utility.hexStringToLong(s);

		_valid = true;
	    }
	    catch (NumberFormatException e)
	    {
		if (!errsOk)
		    ErrorStream.warning().println("Uid general parsing error: "+uidString);

		_valid = false;
	    }
	    catch (StringIndexOutOfBoundsException e)
	    {
		_valid = false;		
	    }
	}
	else
	{
	    this.copy(Uid.nullUid());
	}

	if (!_valid)
	{
	    if (errsOk)
	    {
		try
		{
		    this.copy(Uid.nullUid());
		}
		catch (Exception e)
		{
		    ErrorStream.fatal().println("Uid.Uid string constructor could not create nullUid for incorrect string: "+uidString);

		    throw new FatalError("nullUid error for: "+uidString);
		}
	    }
	    else
	    {
		ErrorStream.fatal().println("Uid.Uid string constructor incorrect: "+uidString);

		throw new FatalError("Invalid string: "+uidString);
	    }
	}
    }

    /**
     * Override Object.hashCode. We always return a positive value.
     */

    /*
     * generateHash should have been called by now.
     */
    
public int hashCode ()
    {
	if (_hashValue == -1)
	    generateHash();
	
	return _hashValue;
    }

    /**
     * Routines to pack and unpack Uids into Buffers (for RPC, storage etc)
     */

public synchronized void pack (OutputBuffer packInto) throws IOException
    {
	if (_valid)
	{
	    packInto.packInt(hostAddr);
	    packInto.packInt(process);
	    packInto.packInt(sec);
	    packInto.packInt(other);
	}
	else
	    throw new IOException("Invalid Uid object.");
    }

public synchronized void unpack (InputBuffer unpackFrom) throws IOException
    {
	/*
	 * Prevent nullUid being overwritten by unpack.
	 */

	if (this == Uid.nullUid())
	    throw new IOException("Cannot unpack into nullUid!");

	_valid = true;  // should not be able to pack an invalid Uid.

	try
	{
	    hostAddr = unpackFrom.unpackInt();
	    process = unpackFrom.unpackInt();
	    sec = unpackFrom.unpackInt();
	    other = unpackFrom.unpackInt();
	}
	catch (IOException e)
	{
	    _valid = false;

	    throw e;
	}
    }

    /**
     * Print a human-readable form of the Uid.
     */

public void print (PrintStream strm)
    {
	strm.print("<Uid:"+this.toString()+">");
    }

public String stringForm ()
    {
	return new String(Utility.intToHexString(hostAddr)+Uid.breakChar+Utility.intToHexString(process)+Uid.breakChar+Utility.intToHexString(sec)+Uid.breakChar+Utility.intToHexString(other));
    }

    /**
     * Same as stringForm()
     */

public String toString ()
    {
	return stringForm();
    }

    /**
     * Create a copy of this instance.
     */

public Object clone () throws CloneNotSupportedException
    {
	return new Uid(this);
    }

    /**
     * Copy the specified Uid over this instance.
     */

public synchronized void copy (Uid toCopy)
    {
	if (toCopy == this)
	    return;
	
	hostAddr = toCopy.hostAddr;
	process = toCopy.process;
	sec = toCopy.sec;
	other = toCopy.other;
	_hashValue = toCopy._hashValue;
	_valid = toCopy._valid;
    }
    
    /**
     * Uid comparisons.
     */

    /**
     * Override Object.equals
     */
     
public boolean equals (Object o)
    {
	if (o instanceof Uid)
	    return this.equals((Uid) o);
	else
	    return false;
    }

public boolean equals (Uid u)
    {
	if (u == null)
	    return false;
	
	if (u == this)
	    return true;
	
	return ((other == u.other) &&
		(sec == u.sec) &&
		(process == u.process) &&
		(hostAddr == u.hostAddr));
    }

public boolean notEquals (Uid u)
    {
	if (u == null)
	    return true;
	
	if (u == this)
	    return false;
	
	return ((other != u.other) ||
		(sec != u.sec) ||
		(process != u.process) ||
		(hostAddr != u.hostAddr));
    }

public boolean lessThan (Uid u)
    {
	if (u == null)
	    return false;
	
	if (u == this)
	    return false;
	
	if (hostAddr < u.hostAddr)
	    return true;
	else if (hostAddr == u.hostAddr)
	{
	    if (process < u.process)
		return true;
	    else if (process == u.process)
	    {
		if (sec < u.sec)
		    return true;
		else if ((sec == u.sec) && (other < u.other))
		    return true;
	    }
	}
	return false;
    }

public boolean greaterThan (Uid u)
    {
	if (u == null)
	    return false;
	
	if (u == this)
	    return false;
	
	if (hostAddr > u.hostAddr)
	    return true;
	else if (hostAddr == u.hostAddr)
	{
	    if (process > u.process)
		return true;
	    else if (process == u.process)
	    {
		if (sec > u.sec)
		    return true;
		else if ((sec == u.sec) && (other > u.other))
		    return true;
	    }
	}
	return false;
    }

    /**
     * Is the Uid valid?
     */

public synchronized final boolean valid ()
    {
	return _valid;
    }

    /**
     * Return a null Uid (0:0:0:0)
     */

public static final synchronized Uid nullUid ()
    {
	/*
	 * Only create a single instance of this.
	 */

	if (NIL_UID == null)
	    NIL_UID = new Uid("0:0:0:0");

	return NIL_UID;
    }

    /*
     * Serialization methods. Similar to buffer packing. If the Uid is
     * invalid the an IOException is thrown.
     *
     * @since JTS 2.1.
     */

private void writeObject (java.io.ObjectOutputStream out) throws IOException
    {
	if (_valid)
	{
	    out.writeInt(hostAddr);
	    out.writeInt(process);
	    out.writeInt(sec);
	    out.writeInt(other);
	}
	else
	    throw new IOException("Invalid Uid object.");
    }

    /*
     * Serialization methods. Similar to buffer unpacking. If the
     * unserialization fails then an IOException is thrown.
     *
     * @since JTS 2.1.
     */
    
private void readObject (java.io.ObjectInputStream in) throws IOException, ClassNotFoundException
    {
	_valid = true;  // should not be able to pack an invalid Uid.

	try
	{
	    hostAddr = in.readInt();
	    process = in.readInt();
	    sec = in.readInt();
	    other = in.readInt();
	}
	catch (IOException e)
	{
	    _valid = false;
	    
	    throw e;
	}
    }
    
private static final synchronized int getValue ()
    {
	if ((Uid.uidsCreated & 0xf0000000) > 0)
	{
	    Uid.uidsCreated = 0;
	    Uid.initTime = (int) (System.currentTimeMillis()/1000);
	}

	return Uid.uidsCreated++;
    }
    
    /*
     * Is an idempotent operation, so can be called more than once
     * without adverse effect.
     */

private final synchronized void generateHash ()
    {
	if (_valid)
	{
	    if (true)
		_hashValue = hostAddr ^ process ^ sec ^ other;
	    else
	    {
		int g = 0;
		String p = toString();
		int len = p.length();
		int index = 0;
    
		if (len > 0)
		{
		    while (len-- > 0)
		    {
			_hashValue = (_hashValue << 4) + (int) (p.charAt(index));
			g = _hashValue & 0xf0000000;
			
			if (g > 0)
			{
			    _hashValue = _hashValue ^ (g >> 24);
			    _hashValue = _hashValue ^ g;
			}
		    
			index++;
		    }
		}
	    }
	    
	    /*
	     * Integers are signed in Java, so we have to be careful
	     * and only return a signed value.
	     */
		
	    if (_hashValue < 0)
		_hashValue = -_hashValue;
	}
	else
	    ErrorStream.warning().println("Uid.generateHash called for invalid Uid. Will ignore.");
    }

    /*
     * Since we may be given a Uid from the file system (which uses
     * '_' to separate fields, we need to be able to convert.
     */
    
private static final char getBreakChar (String uidString)
    {
	if (uidString == null)
	    return Uid.breakChar;

	if (uidString.indexOf(fileBreakChar) != -1)
	    return Uid.fileBreakChar;
	else
	    return Uid.breakChar;
    }
    
    // use ints rather than longs for interoperability with C++ version

private int     hostAddr;
private int     process;
private int     sec;
private int     other;
private int     _hashValue;
private boolean _valid;
    
private static int     uidsCreated = 0;
private static int     initTime = 0;
private static boolean _enabled = false;
private static boolean ticilDone = false;
    
private static char breakChar = ':';
private static char fileBreakChar = '_';
    
private static Uid NIL_UID = null;
private static int myAddr = 0;
    
}
