/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AtomicObject.java,v 1.3 1998/07/06 13:29:40 nmcl Exp $
 */

package JavaCosTransactionsTests;

import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;
import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaArjunaLite.Atomic.*;

import JavaCosTransactionsTests.TestException;
import org.omg.CosTransactions.SubtransactionsUnavailable;
import java.io.IOException;

public class AtomicObject extends LockManager
{

public AtomicObject ()
    {
	super(ObjectType.ANDPERSISTENT);

	OTS_Current current = OTS.current();

	_value = 0;

	try
	{
	    current.begin();

	    if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
	    {
		_value = 0;

		current.commit(false);
	    }
	    else
		current.rollback();
	}
	catch (Exception e)
	{
	    System.out.println("AtomicObject "+e);
	}
    }

public AtomicObject (Uid u)
    {
	super(u);
    }

public void finalize ()
    {
	super.terminate();
    }

public synchronized boolean incr (int value)
    {
	boolean res = false;
	OTS_Current current = OTS.current();

	try
	{
	    current.begin();

	    if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
	    {
		_value = _value + value;

		current.commit(false);
		res = true;
	    }
	    else
		current.rollback();
	}
	catch (Exception e)
	{
	    System.out.println(e);

	    res = false;
	}

	return res;
    }

public synchronized boolean set (int value)
    {
	boolean res = false;
	OTS_Current current = OTS.current();    

	try
	{
	    current.begin();

	    if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
	    {
		_value = value;

		current.commit(false);
		res = true;
	    }
	    else
		current.rollback();
	}
	catch (Exception e)
	{
	    System.out.println(e);

	    res = false;
	}

	return res;
    }

public synchronized int get () throws TestException
    {
	boolean res = false;
	OTS_Current current = OTS.current();    
	int value = -1;

	try
	{
	    current.begin();

	    if (setlock(new Lock(LockMode.READ), 0) == LockResult.GRANTED)
	    {
		value = _value;

		current.commit(false);
		res = true;
	    }
	    else
		current.rollback();
	}
	catch (Exception e)
	{
	    System.out.println(e);

	    res = false;
	}

	if (!res)
	    throw new TestException();
	else
	    return value;
    }

public boolean save_state (OutputObjectState os, int t)
    {
	boolean result = true;

	try
	{
	    os.packInt(_value);
	}
	catch (IOException e)
	{
	    result = false;
	}

	return result;
    }

public boolean restore_state (InputObjectState os, int t)
    {
	boolean result = true;

	try
	{
	    _value = os.unpackInt();
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

private int _value;

};
