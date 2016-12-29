/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_InterpositionFilter.java,v 1.1.2.1.4.1.4.4 2000/06/01 12:43:59 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Interposition.orbix;

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

/*
 * Only add and remove transactional context if the
 * destination object is inherited from TransactionalObject.
 *
 * When we have finished implementing fully checked transactions
 * we will use the filter to prevent remote access to the factory
 * and the terminator.
 */

/*
 * We could send the transaction context back from the server to
 * the client and use this to lazily register new interposed
 * transactions. We don't do this currently.
 */

/*
 * If we're not within a transaction when we call a TransactionalObject
 * the spec. implies that we should raise a TransactionExpected exception.
 * This is too restrictive, and removed the ability to build modular
 * applications. Therefore, by default we will *not* do this, but this
 * can be overridden with an appropriate environment variable. (Could do
 * this through a separate base class.)
 */

public class OTS_InterpositionFilter extends Filter implements ORBFilter
    {

/*
 * This is a process-level filter in Orbix, so is deliberately
 * different to the others, which are object-level. Basically, all
 * calls go through this filter, even those to the orbixd! So we
 * can't just put a transaction context on to each call (or try
 * to narrow, since that will go through the IR, which will be a
 * remote call, which will go through this filter ...)
 */

public boolean outRequestPreMarshal (Request r)
    {
	OutputStream s = _OrbixWeb.Request(r).create_output_stream();
	Thread currentThread = Thread.currentThread();

	/*
	 * Must remember to put something into the
	 * message for the server to pull out since it
	 * will not know we have been disabled!
	 */
	
	if (OTS_InterpositionFilter.isDisabled(currentThread))
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

	    OTS_InterpositionFilter.disable(currentThread);
	}

	try
	{
	    TransactionalObject ptr = org.omg.CosTransactions.TransactionalObjectHelper.narrow(r.target());

	    if (ptr == null)
		throw new BAD_PARAM();

	    OTS_Current current = OTS.current();
	    Control theControl = current.get_control();
	    boolean problem = false;

	    if (theControl != null)
	    {
		Coordinator theCoordinator = null;
		
		try
		{
		    theCoordinator = theControl.get_coordinator();
		}
		catch (Unavailable e)
		{
		    theCoordinator = null;
		}

		if (theCoordinator != null)
		{
		    try
		    {
			PropagationContext ctx = theCoordinator.get_txcontext();
			
			packPropagationContext(r, ctx);

			ctx = null;
		    }
		    catch (Unavailable ex)
		    {
			/*
			 * We may have to make calls during commit (e.g., after_completion)
			 * which are valid, but which will get Unavailable.
			 */
		    
			// so transmit nothing!
		    
			packPropagationContext(r, null);
		    }

		    theCoordinator = null;
		}
		else
		    problem = true;

		theControl = null;
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
		    OTS_InterpositionFilter.enable(currentThread);
		    
		    throw new TRANSACTION_REQUIRED();
		}
		else
		    packPropagationContext(r, null);
	    }
	}
	catch (BAD_PARAM exEnv)
	{
	    // narrow failed, so not a transactional object.
	}

	OTS_InterpositionFilter.enable(currentThread);

	return true;
    }

/*
 * Finished with request, so disassociate this thread from the
 * transaction.
 */

public boolean outReplyPreMarshal (Request r)
    {
	/*
	 * We could send the propagation context back to the client. Any reason?
	 * Yes, so that we can do low-cost abort and registration.
	 *
	 * //    PropagationContext* ctx = theCoordinator->get_txcontext();
	 */
	
	try
	{
	    Control theControl = OTS.current().suspend();
    
	    /*
	     * Although these are proxies, we cannot delete them yet.
	     * Wait until the transaction terminates.
	     */

	    theControl = null;
	}
	catch (SystemException ex)
	{
	    System.err.println("WARNING - OTS_InterpositionFilter::outReplyPreMarshal caught: "+ex);
	    
	    return false;
	}
	catch (Exception e)
	{
	    System.err.println("WARNING - OTS_InterpositionFilter::outReplyPreMarshal caught exception: "+e);
	    
	    return false;
	}
    
	return true;
    }

public boolean inRequestPreMarshal (Request r)
    {
	boolean problem = false;
	
	try
	{
	    TransactionalObject ptr = org.omg.CosTransactions.TransactionalObjectHelper.narrow(r.target());

	    if (ptr == null)
		throw new BAD_PARAM();

	    ptr = null;
	}
	catch (BAD_PARAM exEnv)
	{
	    /* narrow failed, so not a transactional object. */

	    problem = true;
	}
	
	if (!problem)
	{
	    /*
	     * OK, we are transactional and expect some context information, even
	     * if it zero.
	     */
	    
	    PropagationContext ctx = unpackPropagationContext(r);

	    if (ctx != null)
	    {
		TransactionFactory theFactory = OTS.get_factory();
		Control theControl = theFactory.recreate(ctx);

		if (theControl != null)
		{
		    try
		    {
			OTS.current().resume(theControl);
		    }
		    catch (SystemException e)
		    {
			throw e;
		    }
		    catch (UserException e)
		    {
			throw new UNKNOWN();
		    }
		}

		theControl = null;		
		ctx = null;
	    }
	    else
		problem = true;
	}
	
	if (problem)
	{
	    /*
	     * Only throw an exception if we have no transaction context
	     * and we require one.
	     */
	    
	    if (otsNeedTranContext)
		throw new TRANSACTION_REQUIRED();
	}

	return true;
    }

public boolean inReplyPreMarshal (Request r)
    {
	return true;
    }

public void outReplyFailure (Request r, Exception ex)
    {
	try
	{
	    Control theControl = OTS.current().suspend();
    
	    /*
	     * Although these are proxies, we cannot delete them yet.
	     * Wait until the transaction terminates.
	     */

	    theControl = null;
	}
	catch (SystemException e1)
	{
	    System.err.println("WARNING - OTS_InterpositionFilter::outReplyFailure caught: "+e1);
	}
	catch (Exception e2)
	{
	    System.err.println("WARNING - OTS_InterpositionFilter::outReplyFailure caught exception: "+e2);
	}
    }

private void packPropagationContext (Request r, PropagationContext ctx)
    {
	OutputStream s = _OrbixWeb.Request(r).create_output_stream();

	if (ctx == null)
	    s.write_long(0);
	else
	{
	    s.write_long(1);

	    PropagationContextHelper.write(s, ctx);
	}
    }

private PropagationContext unpackPropagationContext (Request r)
    {
	InputStream s = _OrbixWeb.Request(r).create_input_stream();
	int size = s.read_long();

	if (size != 0)
	    return PropagationContextHelper.read(s);
	else
	    return null;
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
	    String env = System.getProperty(OTS_Environment.OTS_NEED_TRAN_CONTEXT, null);

	    if (env != null)
	    {
		if (env.compareTo("YES") == 0)
		    otsNeedTranContext = true;
	    }

	    env = System.getProperty(OTS_Environment.OTS_ALWAYS_PROPAGATE_CONTEXT, null);

	    if (env != null)
	    {
		if (env.compareTo("NO") == 0)
		    otsAlwaysPropagate = false;
	    }
	    
	    otsHaveChecked = true;
	}
	
}


