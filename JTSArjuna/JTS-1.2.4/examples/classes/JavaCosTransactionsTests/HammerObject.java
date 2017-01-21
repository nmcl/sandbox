/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HammerObject.javatmpl,v 1.3 1998/07/06 13:29:48 nmcl Exp $
 */

/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability.h,v 1.4 1998/06/30 16:34:10 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2 1998/11/12 08:52:40 nmcl Exp $
 */













































package JavaCosTransactionsTests;

import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.JavaCosTransactions.OTS_Current;
import com.arjuna.JavaCosTransactions.OTS_ExplicitInterposition;
import com.arjuna.JavaGandiva.Common.*;
import com.arjuna.JavaArjunaLite.Atomic.*;
import TestModule.*;
import org.omg.CosTransactions.*;
import org.omg.CORBA  .IntHolder;

import JavaCosTransactionsTests.TestException;
import org.omg.CosTransactions.SubtransactionsUnavailable;
import org.omg.CORBA  .SystemException;
import java.io.IOException;

/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CosTranExceptions.h,v 1.3 1998/07/06 13:29:23 nmcl Exp $
 */





/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CosTranNames.h,v 1.3 1998/06/29 09:35:35 nmcl Exp $
 */




/*
 * Simple renaming macros for portability.
 *
 * Slightly different from C++ version in that we don't need the
 * typedefs. However, we do need some mapping for OTS exceptions which
 * should now be system exceptions.
 */

/*
 * CosTransaction module exceptions
 */






/*
 * We need to check this. WRONG_TRANSACTION is supposed to
 * be a system exception (in the latest draft of the OTS),
 * but two ORBs have it as a user exception.
 */





/**/





/*
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */


import org.omg.CORBA  .WrongTransaction;
import org.omg.CORBA  .INVALID_TRANSACTION;
import org.omg.CORBA  .TRANSACTION_REQUIRED;
import org.omg.CORBA  .TRANSACTION_ROLLEDBACK;





public class HammerObject extends LockManager implements  TestModule.HammerOperations 
{
    
public HammerObject ()
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
		current.commit(true);
	    }
	    else
		current.rollback();
	}
	catch (Exception e)
	{
	    System.err.println("HammerObject: "+e);
	}
    }

public HammerObject (Uid u)
    {
	super(u);
    }

public void finalize ()
    {
	super.terminate();
    }

public boolean incr (int value, Control control) throws SystemException
    {
	boolean res = false;
	OTS_ExplicitInterposition inter = new OTS_ExplicitInterposition();

	if (!inter.registerTransaction(control))
	{
	    System.err.println("WARNING HammerObject.incr - could not do interposition");
	    return false;
	}

	OTS_Current current = OTS.current();

	try
	{
	    current.begin();

	    if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
	    {
		_value = _value + value;

		current.commit(true);
		res = true;
	    }
	    else
		current.rollback();
	}
	catch (Exception e)
	{
	    System.err.println("HammerObject.incr: "+e);
	    
	    res = false;
	}

	inter.unregisterTransaction();
	
	return res;
    }

public boolean set (int value, Control control) throws SystemException 
    {
	boolean res = false;
	OTS_ExplicitInterposition inter = new OTS_ExplicitInterposition();

	if (!inter.registerTransaction(control))
	{
	    System.err.println("WARNING HammerObject.set - could not do interposition");
	    return false;
	}
    
	OTS_Current current = OTS.current();    

	try
	{
	    current.begin();

	    if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
	    {
		_value = value;

		current.commit(true);
		res = true;
	    }
	    else
		current.rollback();
	}
	catch (Exception e)
	{
	    System.err.println("HammerObject.set: "+e);

	    res = false;
	}

	inter.unregisterTransaction();
	
	return res;
    }

public boolean get (IntHolder value, Control control) throws SystemException
    {
	boolean res = false;
	OTS_ExplicitInterposition inter = new OTS_ExplicitInterposition();

	if (!inter.registerTransaction(control))
	{
	    System.err.println("WARNING HammerObject.incr - could not do interposition");
	    return false;
	}

	OTS_Current current = OTS.current();

	try
	{
	    current.begin();

	    if (setlock(new Lock(LockMode.READ), 0) == LockResult.GRANTED)
	    {
		value.value = _value;

		current.commit(true);
		res = true;
	    }
	    else
		current.rollback();
	}
	catch (Exception e)
	{
	    System.err.println("HammerObject.get: "+e);
	    
	    res = false;
	}

	inter.unregisterTransaction();
	
	return res;
    }

public boolean save_state (OutputObjectState os, int ot)
    {
	try
	{
	    os.packInt(_value);

	    return true;
	}
	catch (IOException e)
	{
	    return false;
	}
    }

public boolean restore_state (InputObjectState os, int ot)
    {
	try
	{
	    _value = os.unpackInt();

	    return true;
	}
	catch (IOException e)
	{
	    return false;
	}
    }

public String type ()
    {
	return "/StateManager/LockManager/HammerObject";
    }

private int _value;
 
};
