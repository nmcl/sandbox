/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: AtomicObject.java,v 1.2 2003/06/19 11:56:26 nmcl Exp $
 */

package com.hp.mwtests.ts.txoj.common.resources;

import com.arjuna.ats.arjuna.*;
import com.arjuna.ats.arjuna.coordinator.*;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.txoj.*;
import com.arjuna.ats.txoj.common.*;
import java.io.*;

import com.hp.mwtests.ts.txoj.common.exceptions.TestException;
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
    
public void finalize () throws Throwable
    {
	super.terminate();
	super.finalize();
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
	boolean result = super.save_state(os, ot);
	
	if (!result)
	    return false;
	
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
	boolean result = super.restore_state(os, ot);

	if (!result)
	    return false;
	
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
