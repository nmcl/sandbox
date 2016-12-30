/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: AtomicObject.java,v 1.2.4.1.4.1 2000/05/22 10:24:48 nmcl Exp $
 */

package CosTransactionsTests;

import com.arjuna.CosTransactions.OTS;
import com.arjuna.CosTransactions.OTS_Current;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaLite.Atomic.*;

import CosTransactionsTests.TestException;
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

public void finalize () throws Throwable
    {
	super.terminate();
	super.finalize();
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
	    e.printStackTrace();

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
	    e.printStackTrace();

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
	    e.printStackTrace();

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
