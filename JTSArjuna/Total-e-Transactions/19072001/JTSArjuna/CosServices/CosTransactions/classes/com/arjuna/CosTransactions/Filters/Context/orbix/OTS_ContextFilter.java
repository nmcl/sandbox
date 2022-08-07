/*
 * Copyright (C) 1999, 2000,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ContextFilter.java,v 1.3.4.1.2.1.2.3.4.2.2.1.22.1 2000/11/16 15:34:24 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Context.orbix;

import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;
import IE.Iona.OrbixWeb.Features.Filter;
import IE.Iona.OrbixWeb._OrbixWeb;
import org.omg.CORBA.*;
import org.omg.CORBA.portable.*;
import java.util.Hashtable;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CosTransactions.Unavailable;

/*
 * Only add and remove transactional context if the
 * destination object is inherited from TransactionalObject.
 *
 * When we have finished implementing fully checked transactions
 * we will use the filter to prevent remote access to the factory
 * and the terminator.
 */

/*
 * If we're not within a transaction when we call a TransactionalObject
 * the spec. implies that we should raise a TransactionExpected exception.
 * This is too restrictive, and removed the ability to build modular
 * applications. Therefore, by default we will *not* do this, but this
 * can be overridden with an appropriate environment variable. (Could do
 * this through a separate base class.)
 */

public class OTS_ContextFilter extends Filter implements ORBFilter
{

/*
 * Only the transaction creator can terminate the transaction. So don't
 * propagate the terminator.
 */

/*
 * Propagate/expect context if:
 *
 * (i) we are a TransactionalObject.
 * (ii) we define otsAlwaysPropagate to TRUE.
 *
 * Throw an exception at the client/server side if there is no context if:
 *
 * (i) we define otsNeedTranContext to FALSE.
 */

/*
 * This is a process-level filter in Orbix, so has to be
 * deliberately different to the others.
 */

public boolean outRequestPreMarshal (Request r)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextFilter::outRequestPreMarshal ( "+r+" )");
	}

	OutputStream s = _OrbixWeb.Request(r).create_output_stream();
	Thread currentThread = Thread.currentThread();

	/*
	 * Must remember to put something into the
	 * message for the server to pull out since it
	 * will not know we have been disabled!
	 */
	
	if (OTS_ContextFilter.isDisabled(currentThread))
	{
	    s.write_long(0);
	    
	    return true;
	}
	else
	{
	    /*
	     * Now disable interceptors to prevent us getting
	     * into an infinite loop!
	     */

	    OTS_ContextFilter.disable(currentThread);
	}
	    
	try
	{
	    TransactionalObject ptr = org.omg.CosTransactions.TransactionalObjectHelper.narrow(r.target());
	    
	    if (ptr == null)
		throw new BAD_PARAM();
	
	    OTS_Current current = OTS.current();
	    Control theControl = current.get_control();
	    int size = 0;
	    String stringRef = null;
	    boolean problem = false;

	    if (theControl != null)
	    {
		try
		{
		    Coordinator theCoordinator = theControl.get_coordinator();
		
		    if (theCoordinator != null)
		    {
			stringRef = ORBInterface.orb().object_to_string(theCoordinator);
			size = stringRef.length();
		    }
		    else
			problem = true;
		}
		catch (Unavailable e)
		{
		    problem = true;
		}
	    }
	    else
		problem = true;

	    if (problem)
	    {
		/*
		 * Only throw an exception if we have no transaction context
		 * and we require one.
		 */
	    
		if (otsNeedTranContext)
		{
		    OTS_ContextFilter.enable(currentThread);
		    
		    throw new TRANSACTION_REQUIRED();
		}
		else
		    size = 0;
	    }

	    s.write_long(size);
	
	    if (size > 0)
	    {
		s.write_string(stringRef);

		stringRef = null;
	    }
	}
	catch (BAD_PARAM exEnv)
	{
	    // narrow failed, so not a transactional object.
	}

	OTS_ContextFilter.enable(currentThread);
	
	return true;
    }

/*
 * Finished with request, so disassociate this thread from the
 * transaction.
 */

public boolean outReplyPreMarshal (Request r)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextFilter::outReplyPreMarshal ( "+r+" )");
	}

	try
	{
	    Control theControl = OTS.current().suspend();
	    
	    if (theControl != null)
	    {
		OTS.destroyControl(theControl);
		theControl = null;
	    }
	}
	catch (SystemException ex)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ContextFilter::outReplyPreMarshal caught: "+ex);

	    return false;
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ContextFilter::outReplyPreMarshal caught exception: "+e);
	
	    return false;
	}

	return true;
    }

public boolean inRequestPreMarshal (Request r)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextFilter::inRequestPreMarshal ( "+r+" )");
	}
	
	try
	{
	    TransactionalObject ptr = org.omg.CosTransactions.TransactionalObjectHelper.narrow(r.target());

	    if (ptr == null)
		throw new BAD_PARAM();	    

	    ptr = null;
	    
	    String stringRef = null;
	    InputStream s = _OrbixWeb.Request(r).create_input_stream();
	    int size = s.read_long();

	    if (size > 0)
	    {
		try
		{
		    stringRef = s.read_string();
		    org.omg.CORBA.Object obj = ORBInterface.orb().string_to_object(stringRef);
		    Coordinator theCoordinator = org.omg.CosTransactions.CoordinatorHelper.narrow(obj);

		    if (theCoordinator == null)
			throw new BAD_PARAM();
		    
		    Control theControl = OTS_Factory.createProxy(theCoordinator, null);

		    if (theControl != null)
		    {
			try
			{
			    OTS.current().resume(theControl);
			}
			catch (Exception e)
			{
			}

			theControl = null;
		    }
		    
		    obj = null;
		    theCoordinator = null;
		}
		catch (BAD_PARAM exEnv)
		{
		}
		catch (Exception e)
		{
		}

		stringRef = null;
	    }
	    else
	    {
		/*
		 * Only throw an exception if we have no transaction context
		 * and we require one.
		 */
	    
		if (otsNeedTranContext)
		    throw new TRANSACTION_REQUIRED();
	    }
	}
	catch (BAD_PARAM exEnv)
	{
	}

	return true;
    }

public boolean inReplyPreMarshal (Request r)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextFilter::inReplyPreMarshal ( "+r+" )");
	}

	return true;
    }

public void outReplyFailure (Request r, Exception ex)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextFilter::outReplyFailure ( "+r+" )");
	}

	try
	{
	    Control theControl = OTS.current().suspend();
	    
	    if (theControl != null)
	    {
		OTS.destroyControl(theControl);
		theControl = null;
	    }
	}
	catch (SystemException e1)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ContextFilter::outReplyPreMarshal caught: "+e1);
	}
	catch (Exception e2)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ContextFilter::outReplyPreMarshal caught exception: "+e2);
	}
    }
    
private static synchronized boolean isDisabled (Thread t)
    {
	return _disabledList.contains(t);
    }

private static synchronized void disable (Thread t)
    {
	_disabledList.put(t, t);
    }

private static synchronized void enable (Thread t)
    {
	_disabledList.remove(t);
    }
    
private static boolean otsAlwaysPropagate = true;
private static boolean otsNeedTranContext = false;
private static boolean otsHaveChecked = false;
private static Hashtable _disabledList = new Hashtable();
    
    static
    {
	if (!otsHaveChecked)
	{
	    String env = PropertyManager.getProperty(OTS_Environment.OTS_NEED_TRAN_CONTEXT, null);

	    if (env != null)
	    {
		if (env.compareTo("YES") == 0)
		    otsNeedTranContext = true;
	    }

	    env = PropertyManager.getProperty(OTS_Environment.OTS_ALWAYS_PROPAGATE_CONTEXT, null);

	    if (env != null)
	    {
		if (env.compareTo("NO") == 0)
		    otsAlwaysPropagate = false;
	    }
	    
	    otsHaveChecked = true;
	}
    }
    
}
