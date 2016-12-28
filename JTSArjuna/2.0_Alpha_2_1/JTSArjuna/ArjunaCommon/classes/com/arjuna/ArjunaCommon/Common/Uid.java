/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: Uid.java,v 1.3.2.1 2000/04/25 08:35:56 nmcl Exp $
 */

package com.arjuna.ArjunaCommon.Common;

import java.net.InetAddress;
import java.lang.Math;
import java.lang.Cloneable;
import java.io.PrintStream;

import java.io.IOException;
import java.net.UnknownHostException;
import java.lang.StringIndexOutOfBoundsException;
import java.lang.NumberFormatException;
import java.lang.CloneNotSupportedException;
import java.lang.OutOfMemoryError;

public class Uid implements Cloneable
{

public Uid ()
    {
	hostAddr = -1;
	process = 0;
	sec = 0;
	other = 0;
	_hashValue = -1;	
	_valid = true;
	
	try
	{
	    hostAddr = Utility.hostInetAddr();	/* calculated only once */

	    if (Uid.processId == 0)
		Uid.processId = Uid.getpid();
	    
	    process = Uid.processId;

	    if (Uid.initTime == 0)    
		Uid.initTime = (int) (System.currentTimeMillis()/1000);

	    sec = Uid.initTime;

	    other = Uid.getValue();
	    
	    generateHash(); 
	}
	catch (UnknownHostException e)
	{
	    System.err.println("Uid - cannot get local host.");
	    _valid = false;
	}

    	/*	
	 * Now do licence checking.
         */

	if (!Uid.ticilDone)
	{
	    String li = PropertyManager.getProperty("JAVA_ARJUNA_LICENCE");
	
	    Uid.ticilDone = true;
	
	    if (li != null)
	    {
		boolean ok = true;
		int index1 = li.indexOf('-');
		int index2 = 0;
		String hString = null;
		String nmString = null;
		String dString = null;
		String cString = null;
		
		if (index1 != -1)
		    hString = li.substring(0, index1);
		else
		    ok = false;

		if (ok)
		{
		    index2 = li.indexOf('-', index1 +1);
		    
		    if (index2 != -1)
		    {
			nmString = li.substring(index1 +1, index2);
			index1 = index2;
		    }
		    else
			ok = false;
		}

		if (ok)
		{
		    index2 = li.indexOf('-', index1 +1);

		    if (index2 != -1)
		    {
			dString = li.substring(index1 +1, index2);
			index1 = index2;
		    }
		    else
			ok = false;
		}

		if (ok)
		{
		    cString = li.substring(index1 +1);
		}

		if (ok)
		{
		    /*
		     * We have to use hexStringToLong rather than
		     * hexStringToInt because Java throws an exception for
		     * large (valid!) integer strings such as d0a15428.
		     */
		    
		    int suppliedHost = (int) Utility.hexStringToLong(hString);
		    int suppliedNetmask = (int) Utility.hexStringToLong(nmString);
		    int suppliedDate = (int) Utility.hexStringToLong(dString);
		    int suppliedCheck = (int) Utility.hexStringToLong(cString);

		    int originalNetmask = suppliedNetmask ^ 0xfc451ade;
		    int localHost = hostAddr ^ 0x4bf2abc4;
		    int network = localHost & originalNetmask;
		    int suppliedNetwork = suppliedHost & originalNetmask;

		    int calc = 0xa6a6a6a6;
		    int a = suppliedDate;
		    int b = network;
		    int c = suppliedNetmask;

		    for (int i = 0; i < 17; i++)
		    {
			a = (37 * (a ^ calc)) + 41;
			b = (27 * (b ^ calc)) + 31;
			c = (43 * (c ^ calc)) + 29;
			calc = a ^ b ^ c;
		    }

		    suppliedDate = suppliedDate ^ 0xbeca54d2;
		    
		    /*
		     * Check for 'infinite' timeout period.
		     */
		    
		    if ((calc != suppliedCheck) || (suppliedNetwork != network) || ((suppliedDate <= initTime) && (suppliedDate != 0xffffffff)))
		    {
			System.err.println("Arjuna licence information is invalid. Please apply for a new licence.");
			System.exit(1);
		    }
		}
		else
		{
		    System.err.println("Arjuna licence environment variable has invalid format.");
		    System.exit(1);
		}
	    }
	    else
	    {
		System.err.println("Arjuna licence environment variable JAVA_ARJUNA_LICENCE not found.");
		System.exit(1);
	    }
	}
    }

public Uid (Uid copyFrom)
    {
	try
	{
	    copy(copyFrom);
	}
	catch (UidException e)
	{
	    _valid = false;
	}
    }

    /*
     * Create Uid from string representation. boolean arg says whether
     * to give up if an error is detected or to simply replace with NIL_UID
     */

public Uid (String uidString)
    {
	this(uidString, true);
    }
    
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
		System.err.println("Uid general parsing error: "+uidString);
		_valid = false;
	    }
	    catch (StringIndexOutOfBoundsException e)
	    {
		_valid = false;		
	    }
	}
	else
	{
	    try
	    {
		this.copy(Uid.nullUid());
	    }
	    catch (UidException e)
	    {
	    }
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
		    System.err.println("Uid.Uid string constructor could not create nullUid for incorrect string: "+uidString);
		    System.exit(0);    
		}
	    }
	    else
	    {
		System.err.println("Uid.Uid string constructor incorrect: "+uidString);
		System.exit(0);
	    }
	}

	if (_valid)
	    generateHash();
    }

    /*
     * Override Object.hashCode. We always return a positive value.
     *
     * generateHash should have been called by now.
     */
    
public int hashCode ()
    {
	return _hashValue;
    }

    /*
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
	_valid = true;  // should not be able to pack an invalid Uid.

	try
	{
	    hostAddr = unpackFrom.unpackInt();
	    process = unpackFrom.unpackInt();
	    sec = unpackFrom.unpackInt();
	    other = unpackFrom.unpackInt();

	    generateHash();
	}
	catch (IOException e)
	{
	    _valid = false;

	    throw e;
	}
    }

public void print (PrintStream strm)
    {
	strm.print("<Uid:"+this.toString()+">");
    }

public String stringForm ()
    {
	return new String(Utility.intToHexString(hostAddr)+Uid.breakChar+Utility.intToHexString(process)+Uid.breakChar+Utility.intToHexString(sec)+Uid.breakChar+Utility.intToHexString(other));
    }
    
public String toString ()
    {
	return stringForm();
    }

public Object clone () throws CloneNotSupportedException
    {
	return new Uid(this);
    }
    
public synchronized void copy (Uid toCopy) throws UidException
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
    
    /*
     * Uid comparisons.
     */

    /*
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

public synchronized final boolean valid ()
    {
	return _valid;
    }

public static synchronized Uid nullUid ()
    {
	if (NIL_UID == null)
	    NIL_UID = new Uid("0:0:0:0");

	return NIL_UID;
    }

private static synchronized int getValue ()
    {
	if ((Uid.uidsCreated & 0xf0000000) > 0)
	{
	    Uid.uidsCreated = 0;
	    Uid.initTime = (int) (System.currentTimeMillis()/1000);
	}

	return Uid.uidsCreated++;
    }
    
private final synchronized void generateHash ()
    {
	if (_valid)
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

		/*
		 * Integers are signed in Java, so we have to be careful
		 * and only return a signed value.
		 */
		
		if (_hashValue < 0)
		    _hashValue = -_hashValue;
	    }
	}
	else
	    System.err.println("Uid.generateHash called for invalid Uid. Will ignore.");
    }

    /*
     * This had better be unique between processes on the same
     * machine. If not we're in trouble!
     */
    
private static int getpid ()
    {
	return Math.abs((new Object()).hashCode());
    }

    /*
     * Since we may be given a Uid from the file system (which uses
     * '_' to separate fields, we need to be able to convert.
     */
    
private static char getBreakChar (String uidString)
    {
	if (uidString == null)
	    return Uid.breakChar;

	if (uidString.indexOf(fileBreakChar) != -1)
	    return Uid.fileBreakChar;
	else
	    return Uid.breakChar;
    }
    
private int hostAddr;      // use ints rather than longs for interoperability with C++ version
private int process;
private int sec;
private int other;
private int _hashValue;
private boolean _valid;
    
private static int uidsCreated = 0;
private static int processId = 0;
private static int initTime = 0;
private static boolean ticilDone = false;
    
private static char breakChar = ':';
private static char fileBreakChar = '_';
    
private static Uid NIL_UID = null;

private static int myAddr = 0;
    
}
