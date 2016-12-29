/*
 * Copyright (C) 2000,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ContextFilter.java,v 1.3.4.1 2000/04/25 08:36:07 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Context.jbroker;

import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;
import org.omg.CORBA.*;
import org.omg.CORBA.portable.*;
import com.objectEra.jbroker.api.transaction.*;
import java.util.*;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CosTransactions.Unavailable;

/*
 * JBroker implements CosTSInteroperation and friends, which means that
 * we have no control over whether we propagate a context when not
 * derived from TransactionalObject - the ORB only calls us when the
 * object being invoked is derived from TransactionalObject!
 */

public class OTS_ContextFilter implements TransactionService
{

/*
 * Only the transaction creator can terminate the transaction. So don't
 * propagate the terminator.
 */

public OTS_ContextFilter ()
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
    
/*
 * Propagate/expect context if:
 *
 * (i) we are a TransactionalObject.
 *
 * Throw an exception at the client/server side if there is no context if:
 *
 * (i) we define otsNeedTranContext to FALSE.
 */

public void sendingRequest (int requestId, OutputStream os)
    {
	try
	{
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
		    throw new TRANSACTION_REQUIRED();
		else
		    size = 0;
	    }

	    os.write_long(size);

	    System.out.println("adding "+size);
	    
	    if (size > 0)
	    {
		os.write_string(stringRef);

		stringRef = null;
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
	    System.err.println("WARNING - OTS_ContextFilter::sendingReply caught: "+ex);
	    throw ex;
	}
	catch (Exception e)
	{
	    System.err.println("WARNING - OTS_ContextFilter::sendingReply caught exception: "+e);
	
	    throw new UNKNOWN();
	}
    }

    /*
     * Must be TransactionalObject to get here!
     */

public void receivedRequest(int requestId, InputStream s)
    {
	String stringRef = null;
	int size = s.read_long();
	boolean problem = false;
	
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

		ORBInterface.orb().connect(theControl);
		
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
		problem = true;
	    }
	    catch (Exception e)
	    {
		problem = true;
	    }
	    
	    stringRef = null;
	}

	/*
	 * Only throw an exception if we have no transaction context
	 * and we require one.
	 */
	 
	if (problem)
	{
	    if (otsNeedTranContext)
		throw new TRANSACTION_REQUIRED();
	}
    }

public void receivedReply(int requestId, InputStream contextIS, Exception ex)
    {
    }
    
private static boolean otsNeedTranContext = false;
private static Hashtable transactions = new Hashtable();  // to be used for checked transactions
    
}
