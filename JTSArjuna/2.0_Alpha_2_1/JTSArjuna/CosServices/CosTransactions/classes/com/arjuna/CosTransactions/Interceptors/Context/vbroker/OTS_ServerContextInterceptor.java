/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerContextInterceptor.java,v 1.4 2000/03/06 11:47:42 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interceptors.Context.vbroker;

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.inprise.vbroker.interceptor.*;
import com.inprise.vbroker.IOP.*;
import com.inprise.vbroker.orb.*;
import org.omg.CORBA.portable.*;
import org.omg.CORBA.CompletionStatus;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.TRANSACTION_REQUIRED;

/*
 * IMPORTANT: the stream returned from the interceptor in a send 
 * or receive call must have the dataOffset field set to the beginning
 * of the data body (following all headers) and the offset field set 
 * to the end of the message.  This is absolutely necessary so that 
 * the ORB can tell if you have increased the length of your message.
 * Set these using offset() and dataOffset() settors.
 */

public class OTS_ServerContextInterceptor implements ServerRequestInterceptor
{
    /*
     * Interceptors in VisiBroker are fairly braindead - they use a single
     * buffer for the entire message and always position it at the start of
     * *all* data. So, we must somehow find our own way to the data for this
     * filter, otherwise we could start using the data meant for the actual
     * object!
     */

public void preinvoke (org.omg.CORBA.Object target, String operation,
		       ServiceContext[] sctx,
		       com.inprise.vbroker.CORBA.portable.InputStream buf,
		       Closure closure)
    {
	/*
	 * There appears to be a bug in Visibroker whereby the client-side
	 * invocation of _is_a does not go through the interceptor! So, we
	 * can't look for null contexts being tagged on!
	 */

	if (operation.compareTo("_is_a") == 0)
	    return;
	
	Thread currentThread = Thread.currentThread();
	CDRInputStream istr = (com.inprise.vbroker.orb.CDRInputStream) buf;
	int dataStart = istr.dataOffset();
	int dataEnd = istr.offset();
	
	istr.offset(dataEnd-OTS_ServerContextInterceptor.dataOffset);

	if (OTS_ContextInterceptorManager.isDisabled(currentThread))
	{
	    /*
	     * Must remember to "pull" something from the
	     * message even if we have been disabled - the
	     * client will not know that!
	     */

	    istr.offset(dataEnd);
	}
	else
	{
	    /*
	     * Now disable interceptors to prevent us getting
	     * into an infinite loop!
	     */
	
	    OTS_ContextInterceptorManager.disable(currentThread);
	
	    if (!otsAlwaysPropagate)
	    {
		TransactionalObject tranObject = null;
	    
		try
		{
		    tranObject = org.omg.CosTransactions.TransactionalObjectHelper.narrow(target);

		    if (tranObject == null)
			throw new BAD_PARAM();
		}
		catch (BAD_PARAM e)
		{
		    /*
		     * Not a transactional object so return.
		     * Remember to enable interceptors.
		     */

		    OTS_ContextInterceptorManager.enable(currentThread);
		    istr.offset(dataEnd);

		    return;
		}
		
		tranObject = null;
	    }
	
	    try
	    {
		istr.offset(istr.read_long());

		boolean haveContext = istr.read_boolean();

		if (haveContext)
		{
		    String refString = istr.read_string();
		    
		    org.omg.CORBA.Object obj = ORBInterface.orb().string_to_object(refString);
		    Coordinator theCoordinator = org.omg.CosTransactions.CoordinatorHelper.narrow(obj);
		    Control theControl = OTS_Factory.createProxy(theCoordinator, null);
		    OTS_Current current = OTS.current();

		    current.resume(theControl);

		    theControl = null;
		    theCoordinator = null;
		}
		else
		{
		    /*
		     * Only throw an exception if we have no transaction
		     * context and we require one.
		     */
	    
		    if (otsNeedTranContext)
		    {
			OTS_ContextInterceptorManager.enable(currentThread);
			istr.offset(dataEnd);

			throw new TRANSACTION_REQUIRED();
		    }
		}
	    }
	    catch (InvalidControl e1)
	    {
		System.err.println("OTS_ServerContextInterceptor.preinvoke - caught InvalidControl: "+e1);
	    }
	    catch (SystemException e2)
	    {
		System.err.println("OTS_ServerContextInterceptor.preinvoke - caught unexpected exception: "+e2);
	    }

	    istr.offset(dataEnd);

	    OTS_ContextInterceptorManager.enable(currentThread);
	}
    }

public void postinvoke_premarshal (org.omg.CORBA.Object target,
				   ServiceContextListHolder sctx,
				   org.omg.CORBA.Environment env,
				   Closure closure)
    {
    }

public void postinvoke_postmarshal (org.omg.CORBA.Object target,
				    com.inprise.vbroker.CORBA.portable.OutputStream buf,
				    Closure closure)
    {
	try
	{
	    Control c = OTS.current().suspend();
	
	    if (c != null)
	    {
		OTS.destroyControl(c);
		c = null;
	    }
	}
	catch (Exception e)
	{
	    System.err.println("OTS_ServerContextInterceptor.postinvoke_postmarshal caught: "+e);
	}
    }

public void exception_occurred (org.omg.CORBA.Object target,
				org.omg.CORBA.Environment env,
				Closure closure)
    {
    }

private static boolean otsHaveChecked = false;
private static boolean otsNeedTranContext = OTS_Defaults.needTransactionContext;
private static boolean otsAlwaysPropagate = OTS_Defaults.alwaysPropagateContext;

private static final int dataOffset = 4;

    static
    {
	if (!otsHaveChecked)
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
    
};
