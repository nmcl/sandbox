/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StackImple.javatmpl,v 1.2 1998/07/15 15:35:15 nmcl Exp $
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
import com.arjuna.JavaCosTransactions.ArjunaOTS.*;
import TestModule.*;
import org.omg.CORBA  .IntHolder;
import org.omg.CosTransactions.*;

import JavaCosTransactionsTests.TestException;
import org.omg.CosTransactions.SubtransactionsUnavailable;
import java.io.IOException;
import org.omg.CORBA  .SystemException;

public class StackImple extends LockManager implements  TestModule.stackOperations 
{

public StackImple ()
    {
	super (ObjectType.ANDPERSISTENT);

	top = 0;

	for (int i = 0; i < ARRAY_SIZE; i++)
	    array[i] = 0;

	AtomicTransaction A = new AtomicTransaction();

	try
	{
	    A.begin();

	    if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
		A.commit(false);
	    else
		A.rollback();
	}
	catch (Exception e1)
	{
	    System.err.println(e1);

	    try
	    {
		A.rollback();
	    }
	    catch (Exception e2)
	    {
		System.err.println(e2);
	    }

	    System.exit(1);
	}
    }

public StackImple (Uid uid)
    {
	super(uid);
	
	top = 0;

	for (int i = 0; i < ARRAY_SIZE; i++)
	    array[i] = 0;

	AtomicTransaction A = new AtomicTransaction();

	try
	{
	    A.begin();

	    if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
		A.commit(false);
	    else
		A.rollback();
	}
	catch (Exception e1)
	{
	    System.err.println(e1);

	    try
	    {
		A.rollback();
	    }
	    catch (Exception e2)
	    {
		System.err.println(e2);
	    }

	    System.exit(1);
	}	
    }

public void finalize ()
    {
	terminate();
    }

public int push (int val) throws SystemException
    {
	AtomicTransaction A = new AtomicTransaction();
	int res = 0;

	try
	{
	    A.begin();

	    if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
	    {
		if (top < ARRAY_SIZE)
		{
		    array[top] = val;
		    top++;
		}
		else
		    res = -1;

		if (res == 0)
		{
		    A.commit(false);
		}
		else
		    A.rollback();
	    }
	    else
		A.rollback();
	}
	catch (Exception e1)
	{
	    try
	    {
		A.rollback();
	    }
	    catch (Exception e2)
	    {
		System.err.println(e2);
	    }

	    res = -1;
	}

	return res;
    }

public int pop (IntHolder val) throws SystemException
    {
	AtomicTransaction A = new AtomicTransaction();
	int res = 0;

	try
	{
	    A.begin();

	    if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
	    {
		if (top > 0)
		{
		    top--;
		    val.value = array[top];
		}
		else
		    res = -1;

		if (res == 0)
		{
		    A.commit(false);
		}
		else
		    A.rollback();
	    }
	    else
	    {
		A.rollback();
	    }
	}
	catch (Exception e1)
	{
	    try
	    {
		A.rollback();
	    }
	    catch (Exception e2)
	    {
		System.err.println(e2);
	    }

	    res = -1;
	}
	
	return res;
    }

public void printStack () throws SystemException
    {
	AtomicTransaction A = new AtomicTransaction();

	try
	{
	    A.begin();
    
	    if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
	    {
		if (top > 0)
		{
		    System.out.println("\nContents of stack:");
	
		    for (int i = 0; i < top; i++)
			System.out.println("\t"+array[i]);
		}
		else
		    System.out.println("\nStack is empty.");

		A.commit(false);
	    }
	    else
	    {
		System.out.println("printStack: could not set WRITE lock.");
	
		A.rollback();
	    }
	}
	catch (Exception e1)
	{
	    try
	    {
		A.rollback();
	    }
	    catch (Exception e2)
	    {
		System.err.println(e2);
	    }
	}
    }

public boolean save_state (OutputObjectState objectState, int ot)
    {
	try
	{
	    objectState.packInt(top);

	    for (int i = 0; i < top; i++)
	    {
		objectState.packInt(array[i]);
	    }

	    return true;
	}
	catch (IOException e)
	{
	    return false;
	}
    }

public boolean restore_state (InputObjectState objectState, int ot)
    {
	try
	{
	    top = objectState.unpackInt();

	    for (int j = 0; j < ARRAY_SIZE; j++)
		array[j] = 0;
    
	    for (int i = 0; i < top; i++)
	    {
		array[i] = objectState.unpackInt();
	    }

	    return true;
	}
	catch (IOException e)
	{
	    return false;
	}
    }

public String type ()
    {
	return "/StateManager/LockManager/StackImple";
    }

public static final int ARRAY_SIZE = 10;

private int[] array = new int[ARRAY_SIZE];
private int top;

};
