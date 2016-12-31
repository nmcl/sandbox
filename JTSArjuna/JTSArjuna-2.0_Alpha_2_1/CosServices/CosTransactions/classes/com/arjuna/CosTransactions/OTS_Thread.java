/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_Thread.java,v 1.1 2000/02/25 14:05:25 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import org.omg.CosTransactions.*;
import com.arjuna.CosTransactions.*;
import java.lang.Thread;

import java.lang.IllegalArgumentException;

public class OTS_Thread extends Thread
{

    /*
     * Can't use finalize since it may be called by
     * some other thread than this one.
     */
    
public void terminate ()
    {
	try
	{
	    OTS_Current current = OTS.current();

	    if (current != null)
	    {
		Control c = current.suspend();

		c = null;
	    }
	}
	catch (Exception e)
	{
	}
    }
    
public void run ()
    {
	if (_currentControl != null)
	{
	    try
	    {
		OTS_Current current = OTS.current();

		if (current != null)
		{
		    current.resume(_currentControl);
		    current = null;
		}
	    }
	    catch (Exception e)
	    {
		System.err.println("OTS_Thread.run - could not resume transaction! "+e);
		System.exit(0);
	    }

	    _currentControl = null;
	}
    }
    
protected OTS_Thread ()
    {
	try
	{
	    OTS_Current current = OTS.current();

	    if (current != null)
	    {
		_currentControl = current.get_control();
		current = null;
	    }
	}
	catch (Exception e)
	{
	    _currentControl = null;
	}
    }

private Control _currentControl;
    
};
