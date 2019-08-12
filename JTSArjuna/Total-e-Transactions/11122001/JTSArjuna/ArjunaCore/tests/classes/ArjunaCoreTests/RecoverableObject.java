/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: RecoverableObject.java,v 1.1.4.1.6.1 2000/12/13 16:42:44 nmcl Exp $
 */

package ArjunaCoreTests;

import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCommon.Common.*;

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

	try
	{
	    super.finalize();
	}
	catch (Throwable e)
	{
	}
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
	return "/StateManager/LockManager/RecoverableObject";
    }
    
private int state;
    
};

