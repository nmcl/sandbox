/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_InterpositionFilter.java,v 1.1.6.1 2000/05/22 10:23:54 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Interposition.jbroker;

import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;
import org.omg.CORBA.*;
import org.omg.CORBA.portable.*;
import com.objectEra.jbroker.api.transaction.*;
import java.util.Hashtable;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.TRANSACTION_REQUIRED;

/*
 * JBroker implements CosTSInteroperation and friends, which means that
 * we have no control over whether we propagate a context when not
 * derived from TransactionalObject - the ORB only calls us when the
 * object being invoked is derived from TransactionalObject!
 */

/*
 * We could send the transaction context back from the server to
 * the client and use this to lazily register new interposed
 * transactions. We don't do this currently.
 */

public class OTS_InterpositionFilter implements TransactionService
    {

public OTS_InterpositionFilter ()
    {
    }

public void initialize (org.omg.CORBA.ORB orb, com.objectEra.jbroker.api.transaction.ThreadContext thrdContext, java.util.Properties props)
    {
	String env = System.getProperty(OTS_Environment.OTS_NEED_TRAN_CONTEXT, null);

	if (env != null)
	{
	    if (env.compareTo("YES") == 0)
		otsNeedTranContext = true;
	}
    }

public void sendingRequest (int requestId, OutputStream os)
    {
	try
	{
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
			
			packPropagationContext(os, ctx);

			ctx = null;
		    }
		    catch (Unavailable ex)
		    {
			/*
			 * We may have to make calls during commit (e.g., after_completion)
			 * which are valid, but which will get Unavailable.
			 */
		    
			// so transmit nothing!
		    
			packPropagationContext(os, null);
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
		    throw new TRANSACTION_REQUIRED();
		else
		    packPropagationContext(os, null);
	    }
	}
	catch (BAD_PARAM exEnv)
	{
	    // narrow failed, so not a transactional object.
	}
    }

/*
 * Finished with request, so disassociate this thread from the
 * transaction.
 */

public void sendingReply(int requestId, OutputStream contextIS)
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
	    System.err.println("WARNING - OTS_InterpositionFilter::sendingReply caught: "+ex);
	    throw ex;
	}
	catch (Exception e)
	{
	    System.err.println("WARNING - OTS_InterpositionFilter::sendingReply caught exception: "+e);
	    throw new UNKNOWN();
	}
    }

    /*
     * Must be TransactionalObject to get here!
     */

public void receivedRequest(int requestId, InputStream s)
    {
	boolean problem = false;
	PropagationContext ctx = unpackPropagationContext(s);

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
	
	if (problem)
	{
	    /*
	     * Only throw an exception if we have no transaction context
	     * and we require one.
	     */
	    
	    if (otsNeedTranContext)
		throw new TRANSACTION_REQUIRED();
	}
    }

public void receivedReply(int requestId, InputStream contextIS, Exception ex)
    {
    }

private void packPropagationContext (OutputStream os, PropagationContext ctx)
    {
	if (ctx == null)
	    os.write_long(0);
	else
	{
	    os.write_long(1);

	    PropagationContextHelper.write(os, ctx);
	}
    }

private PropagationContext unpackPropagationContext (InputStream s)
    {
	int size = s.read_long();

	if (size != 0)
	    return PropagationContextHelper.read(s);
	else
	    return null;
    }
	
private static boolean otsNeedTranContext = false;

}


