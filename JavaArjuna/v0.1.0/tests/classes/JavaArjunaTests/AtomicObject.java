/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicObject.java,v 1.5 1998/07/20 09:10:07 nmcl Exp $
 */

package JavaArjunaLiteTests;

import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaGandiva.Common.*;
import java.io.*;

import java.io.IOException;

public class AtomicObject extends LockManager
{

public AtomicObject ()
    {
	this((ObjectName) null);
    }
    
public AtomicObject (ObjectName objName)
    {
	super(ObjectType.ANDPERSISTENT, objName);

	state = 0;

	AtomicAction A = new AtomicAction();

	A.begin();

	if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
	{
	    if (A.commit() == ActionStatus.COMMITTED)
		System.out.println("Created persistent object " + get_uid());
	    else
		System.out.println("Action.commit error.");
	}
	else
	{
	    A.abort();
	    
	    System.out.println("setlock error.");
	}

	String debug = System.getProperty("DEBUG", null);

	if (debug != null)
	    printDebug = true;
    }

public AtomicObject (Uid u)
    {
	this(u, (ObjectName) null);
    }
    
public AtomicObject (Uid u, ObjectName objName)
    {
	super(u, objName);

	state = -1;

	AtomicAction A = new AtomicAction();

	A.begin();

	if (setlock(new Lock(LockMode.READ), 0) == LockResult.GRANTED)
	{
	    System.out.println("Recreated object "+u);
	    A.commit();
	}
	else
	{
	    System.out.println("Error recreating object "+u);
	    A.abort();
	}

	String debug = System.getProperty("DEBUG", null);

	if (debug != null)
	    printDebug = true;	
    }
    
public void finalize ()
    {
	super.terminate();
    }

public void incr (int value) throws TestException
    {
	AtomicAction A = new AtomicAction();

	A.begin();

	if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
	{
	    state += value;

	    if (A.commit() != ActionStatus.COMMITTED)
		throw new TestException("Action commit error.");
	    else
		return;
	}
	else
	{
	    if (printDebug)
		System.out.println("Error - could not set write lock.");
	}
	
	A.abort();

	throw new TestException("Write lock error.");
    }
	
public void set (int value) throws TestException
    {
	AtomicAction A = new AtomicAction();

	A.begin();

	if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
	{
	    state = value;

	    if (A.commit() != ActionStatus.COMMITTED)
		throw new TestException("Action commit error.");
	    else
		return;
	}
	else
	{
	    if (printDebug)
		System.out.println("Error - could not set write lock.");
	}

	A.abort();

	throw new TestException("Write lock error.");
    }

public int get () throws TestException
    {
	AtomicAction A = new AtomicAction();
	int value = -1;

	A.begin();

	if (setlock(new Lock(LockMode.READ), 0) == LockResult.GRANTED)
	{
	    value = state;

	    if (A.commit() == ActionStatus.COMMITTED)
		return value;
	    else
		throw new TestException("Action commit error.");
	}
	else
	{
	    if (printDebug)
		System.out.println("Error - could not set read lock.");
	}

	A.abort();

	throw new TestException("Read lock error.");
    }

public boolean save_state (OutputObjectState os, int ot)
    {
	boolean result = true;
	
	try
	{
	    os.packInt(state);
	}
	catch (IOException e)
	{
	    result = false;
	}

	return result;
    }

public boolean restore_state (InputObjectState os, int ot)
    {
	boolean result = true;
	
	try
	{
	    state = os.unpackInt();
	}
	catch (IOException e)
	{
	    result = false;
	}

	return result;
    }
    
public String type ()
    {
	return "/StateManager/LockManager/AtomicObject";
    }
    
private int state;
private boolean printDebug;
    
};
