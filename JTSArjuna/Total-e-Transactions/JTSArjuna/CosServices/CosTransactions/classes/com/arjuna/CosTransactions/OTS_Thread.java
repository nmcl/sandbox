/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_Thread.java,v 1.1.8.2.24.1.4.1.10.1 2001/04/06 13:35:21 nmcl Exp $
 */

package com.arjuna.CosTransactions;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.ArjunaCommon.Common.FatalError;
import org.omg.CosTransactions.*;
import com.arjuna.CosTransactions.*;
import java.lang.Thread;

import java.lang.IllegalArgumentException;

/**
 * Create an OTS transactional thread. When the thread is created it is
 * automatically registered with the current transaction, and removed
 * when the transaction terminates.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_Thread.java,v 1.1.8.2.24.1.4.1.10.1 2001/04/06 13:35:21 nmcl Exp $
 * @since JTS 1.2.4.
 */

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
		ErrorStream.stream(ErrorStream.FATAL).println("OTS_Thread.run - could not resume transaction! "+e);

		throw new FatalError("OTS_Thread.run - could not resume transaction! "+e);
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
