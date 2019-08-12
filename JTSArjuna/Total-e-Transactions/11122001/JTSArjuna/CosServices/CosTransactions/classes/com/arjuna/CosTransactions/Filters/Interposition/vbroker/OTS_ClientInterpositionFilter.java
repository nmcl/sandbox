/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ClientInterpositionFilter.java,v 1.1.2.2.2.1 2000/11/21 12:03:49 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Interposition.vbroker;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.visigenic.vbroker.interceptor.*;
import com.visigenic.vbroker.IOP.*;
import com.visigenic.vbroker.GIOP.*;
import com.visigenic.vbroker.orb.*;
import org.omg.CORBA.portable.*;
import org.omg.CORBA.BOA;
import org.omg.CORBA.CompletionStatus;

import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;

/**
 * IMPORTANT: the stream returned from the interceptor in a send 
 * or receive call must have the dataOffset field set to the beginning
 * of the data body (following all headers) and the offset field set 
 * to the end of the message.  This is absolutely necessary so that 
 * the ORB can tell if you have increased the length of your message.
 * Set these using offset() and dataOffset() settors.
 */

/**
 * Only add and remove transactional context if the
 * destination object is inherited from TransactionalObject.
 *
 * When we have finished implementing fully checked transactions
 * we will use the filter to prevent remote access to the factory
 * and the terminator.
 */

/**
 * We could send the transaction context back from the server to
 * the client and use this to lazily register new interposed
 * transactions. We don't do this currently.
 */

/**
 * If we're not within a transaction when we call a TransactionalObject
 * the spec. implies that we should raise a TransactionExpected exception.
 * This is too restrictive, and removed the ability to build modular
 * applications. Therefore, by default we will *not* do this, but this
 * can be overridden with an appropriate environment variable. (Could do
 * this through a separate base class.)
 */

public class OTS_ClientInterpositionFilter implements ClientInterceptor
{
    
public OTS_ClientInterpositionFilter (org.omg.CORBA.Object obj)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ClientInterpositionFilter created.");
	}

	target = obj;

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

public void prepare_request (RequestHeaderHolder hdr, Closure closure)
    {
    }
    
public org.omg.CORBA.portable.OutputStream send_request (RequestHeader hdr,
							 org.omg.CORBA.portable.OutputStream buf,
							 Closure closure)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ClientInterpositionFilter.send_request called for "+hdr.operation);
	}

	Thread currentThread = Thread.currentThread();
	GiopOutputStream ostr = (GiopOutputStream) buf;
	int dataStart = ostr.dataOffset();
	int dataEnd = ostr.offset();

	if (OTS_InterpositionFilterManager.isDisabled(currentThread))
	{
	    /*
	     * Must remember to put something into the
	     * message for the server to pull out since it
	     * will not know we have been disabled!
	     */

	    packPropagationContext(ostr, null);
	}
	else
	{
	    /*
	     * Now disable interceptors to prevent us getting
	     * into an infinite loop!
	     */
	
	    OTS_InterpositionFilterManager.disable(currentThread);
	
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

			OTS_InterpositionFilterManager.enable(currentThread);
		    
			return null;
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
			    OTS_InterpositionFilterManager.enable(currentThread);
			
			    throw new TRANSACTION_REQUIRED();
			}
			else
			{
			    packPropagationContext(ostr, null);
			
			    if (DebugController.enabled())
			    {
				DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
								     (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ClientInterpositionFilter.send_request - have no context to send.");
			    }
			}
		    }
		    else
		    {
			if (DebugController.enabled())
			{
			    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
								 (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ClientInterpositionFilter.send_request - have context to send.");
			}

			PropagationContext ctx = coordPtr.get_txcontext();

			packPropagationContext(ostr, ctx);

			ctx = null;
		    }
		    
		    coordPtr = null;
		    controlPtr = null;
		}
		catch (TRANSACTION_REQUIRED e)
		{
		    throw e;
		}
		catch (SystemException e)
		{
		    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ClientInterpositionFilter.send_request - caught unexpected exception: "+e);
		}
		catch (Unavailable e)
		{
		    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ClientInterpositionFilter.send_request - caught Unavailable exception: "+e);
		    
		    // so transmit nothing!
		    
		    packPropagationContext(ostr, null);
		}
	    }

	    OTS_InterpositionFilterManager.enable(currentThread);
	}

	ostr.write_long(dataEnd);
	
	return null;
    }

public void send_request_failed (RequestHeader hdr, org.omg.CORBA.Environment env,
				 Closure closure)
    {
    }

public void send_request_succeeded (RequestHeader hdr, Closure closure)
    {
    }

public org.omg.CORBA.portable.InputStream receive_reply (ReplyHeader hdr,
							 org.omg.CORBA.portable.InputStream buf,
							 org.omg.CORBA.Environment env,
							 Closure closure)
    {
	return null;
    }

public void receive_reply_failed (int req_id, org.omg.CORBA.Environment env, Closure closure)
    {
    }

public void exception_occurred (int req_id, org.omg.CORBA.Environment env, Closure closure)
    {
    }

private boolean packPropagationContext (GiopOutputStream ostr, PropagationContext ctx)
    {
	if (ctx == null)
	{
	    ostr.write_boolean(false);
	}
	else
	{
	    ostr.write_boolean(true);
	    
	    try
	    {
		PropagationContextHelper.write(ostr, ctx);
	    }
	    catch (Exception e)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ClientInterpositionFilter.packPropagationContext caught exception: "+e);

		return false;
	    }
	}

	return true;
    }
    
private boolean otsHaveChecked = false;
private boolean otsNeedTranContext = OTS_Defaults.needTransactionContext;
private boolean otsAlwaysPropagate = OTS_Defaults.alwaysPropagateContext;
private org.omg.CORBA.Object target = null;
    
};
