/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ClientContextInterceptor.java,v 1.1 2000/02/25 14:06:02 nmcl Exp $
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
 * This is too restrictive, and removes the ability to build modular
 * applications. Therefore, by default we will *not* do this, but this
 * can be overridden with an appropriate environment variable.
 */

public class OTS_ClientContextInterceptor implements ClientRequestInterceptor
{

public void preinvoke_premarshal (org.omg.CORBA.Object target,
				  String operation,
				  ServiceContextListHolder sctx,
				  Closure closure)
{
}

public void preinvoke_postmarshal (org.omg.CORBA.Object target,
				   com.inprise.vbroker.CORBA.portable.OutputStream buf,
				   Closure closure)
    {
	Thread currentThread = Thread.currentThread();
	CDROutputStream ostr = (com.inprise.vbroker.orb.CDROutputStream) buf;
	int dataStart = ostr.dataOffset();
	int dataEnd = ostr.offset();
	
	if (OTS_ContextInterceptorManager.isDisabled(currentThread))
	{
	    /*
	     * Must remember to put something into the
	     * message for the server to pull out since it
	     * will not know we have been disabled!
	     */

	    ostr.write_boolean(false);
	}
	else
	{
	    /*
	     * Now disable interceptors to prevent us getting
	     * into an infinite loop!
	     */
	
	    OTS_ContextInterceptorManager.disable(currentThread);
	
	    if (target != null)
	    {
		if (!otsAlwaysPropagate)
		{
		    try
		    {
			TransactionalObject ptr = org.omg.CosTransactions.TransactionalObjectHelper.narrow(target);

			if (ptr == null)
			    throw new BAD_PARAM();
			else
			    ptr = null;
		    }
		    catch (BAD_PARAM e)
		    {
			/*
			 * Not a transactional object so return.
			 * Remember to enable interceptors.
			 */

			OTS_ContextInterceptorManager.enable(currentThread);
		    
			return;
		    }
		}

		try
		{
		    OTS_Current current = OTS.current();
		    Control controlPtr = current.get_control();
		    Coordinator coordPtr = null;

		    if (controlPtr != null)
		    {
			try
			{
			    coordPtr = controlPtr.get_coordinator();
			}
			catch (Unavailable e)
			{
			    coordPtr = null;
			}
		    }

		    if (coordPtr == null)
		    {
			/*
			 * Only throw an exception if we have no transaction context
			 * and we require one.
			 */
	    
			if (otsNeedTranContext)
			{
			    OTS_ContextInterceptorManager.enable(currentThread);
			
			    throw new TRANSACTION_REQUIRED();
			}
			else
			{
			    ostr.write_boolean(false);
			}
		    }
		    else
		    {
			ostr.write_boolean(true);
			
			String refString = ORBInterface.orb().object_to_string(coordPtr);
			
			ostr.write_string(refString);
		    }
		    
		    coordPtr = null;
		    controlPtr = null;
		}
		catch (SystemException e)
		{
		    System.err.println("OTS_ClientContextInterceptor.preinvoke_postmarshal - caught unexpected exception: "+e);
		}
	    }

	    OTS_ContextInterceptorManager.enable(currentThread);
	}
	
	/*
	 * Always write the start offset to the end of our
	 * message. The server will use this to determine
	 * where to start reading from.
	 */
	
	ostr.write_long(dataEnd);
    }

public void postinvoke (org.omg.CORBA.Object target, ServiceContext[] sctx,
			com.inprise.vbroker.CORBA.portable.InputStream istr,
			org.omg.CORBA.Environment env, Closure closure)
    {
    }

public void exception_occurred (org.omg.CORBA.Object target,
				org.omg.CORBA.Environment env,
				Closure closure)
    {
    }

private static boolean otsHaveChecked = false;
private static boolean otsNeedTranContext = OTS_Defaults.needTransactionContext;
private static boolean otsAlwaysPropagate = OTS_Defaults.alwaysPropagateContext;
  
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
