/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_Thread.java,v 1.1 1998/07/31 15:20:16 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions;

import org.omg.CosTransactions.*;
import com.arjuna.JavaCosTransactions.*;
import java.lang.Thread;
import java.util.Hashtable;

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
	    }

	    _currentControl = null;
	}
    }

public static void create (Object thread) throws IllegalArgumentException
    {
	if (thread instanceof Thread)
	{
	    try
	    {
		OTS_Current current = OTS.current();

		if (current != null)
		{
		    Control cont = current.get_control();

		    if (cont != null)
		    {
			synchronized (_threads)
		        {
			    _threads.put(thread, cont);
			}
			
			cont = null;
		    }

		    current = null;
		}
	    }
	    catch (Exception e)
	    {
	    }
	}
	else
	    throw new IllegalArgumentException();
    }

public static void destroy (Object thread) throws IllegalArgumentException
    {
	if (thread instanceof Thread)
	{
	    synchronized (_threads)
	    {
		_threads.remove(thread);
	    }
	}
	else
	    throw new IllegalArgumentException();
    }

public static boolean resumeTransaction ()
    {
	try
	{
	    Control cont = (Control) _threads.get(Thread.currentThread());

	    if (cont != null)
	    {
		OTS_Current current = OTS.current();

		if (current != null)
		{
		    current.resume(cont);
		    current = null;
		}
	    }

	    cont = null;

	    synchronized (_threads)
	    {
		_threads.remove(Thread.currentThread());
	    }

	    return true;
	}
	catch (Exception e)
	{
	    return false;
	}
    }

public static boolean suspendTransaction ()
    {
	try
	{
	    OTS_Current current = OTS.current();

	    if (current != null)
	    {
		Control c = current.suspend();

		c = null;
		current = null;
	    }

	    return true;
	}
	catch (Exception e)
	{
	    return false;
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

private static Hashtable _threads = new Hashtable();
    
};
