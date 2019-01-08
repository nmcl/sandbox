/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecoverableObject.java,v 1.2 1998/07/06 13:27:09 nmcl Exp $
 */

package JavaArjunaLiteTests;

import com.arjuna.JavaArjunaLite.Atomic.*;
import com.arjuna.JavaGandiva.Common.*;

import java.io.IOException;

public class RecoverableObject extends LockManager
{

public RecoverableObject ()
    {
	AtomicAction A = new AtomicAction();

	A.begin();

	if (setlock(new Lock(LockMode.WRITE), 0) == LockResult.GRANTED)
	{
	    state = 0;

	    if (A.commit() == ActionStatus.COMMITTED)
		System.out.println("Created recoverable object "+get_uid());
	    else
		System.out.println("Action.commit error.");
	}
	else
	{
	    A.abort();
	    
	    System.out.println("setlock error.");
	}
    }

public void finalize ()
    {
	super.terminate();
    }

public boolean set (int value)
    {
	AtomicAction A = new AtomicAction();

	A.begin();

	if (setlock(new Lock(LockMode.WRITE)) == LockResult.GRANTED)
	{
	    state = value;

	    if (A.commit() == ActionStatus.COMMITTED)
		return true;
	    else
		return false;
	}

	A.abort();

	return false;
    }

public int get ()
    {
	AtomicAction A = new AtomicAction();
	int value = -1;

	A.begin();

	if (setlock(new Lock(LockMode.READ)) == LockResult.GRANTED)
	{
	    value = state;

	    if (A.commit() == ActionStatus.COMMITTED)
		return value;
	    else
		return -1;
	}

	A.abort();

	return -1;
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
	return "/StateManager/LockManager/RecoverableObject";
    }
    
private int state;
    
};

