/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Tyne and Wear,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ClientContextFilter.java,v 1.1.2.2.2.1 2000/11/21 12:03:48 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Context.vbroker;

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
 * If we're not within a transaction when we call a TransactionalObject
 * the spec. implies that we should raise a TransactionExpected exception.
 * This is too restrictive, and removed the ability to build modular
 * applications. Therefore, by default we will *not* do this, but this
 * can be overridden with an appropriate environment variable. (Could do
 * this through a separate base class.)
 */

public class OTS_ClientContextFilter implements ClientInterceptor
{
    
public OTS_ClientContextFilter (org.omg.CORBA.Object obj)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ClientContextFilter created.");
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
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ClientContextFilter.send_request called for "+hdr.operation);

	    // for debugging purposes only.
	
	    closure.object = hdr.operation;
	}

	Thread currentThread = Thread.currentThread();
	GiopOutputStream ostr = (GiopOutputStream) buf;
	int dataStart = ostr.dataOffset();
	int dataEnd = ostr.offset();
	
	if (OTS_ContextFilterManager.isDisabled(currentThread))
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
	
	    OTS_ContextFilterManager.disable(currentThread);
	
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

			OTS_ContextFilterManager.enable(currentThread);
		    
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
			    OTS_ContextFilterManager.enable(currentThread);
			
			    throw new TRANSACTION_REQUIRED();
			}
			else
			{
			    ostr.write_boolean(false);

			    if (DebugController.enabled())
			    {
				DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ClientContextFilter.send_request - have no context to send.");
			    }
			}
		    }
		    else
		    {
			if (DebugController.enabled())
			{
			    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ClientContextFilter.send_request - have context to send.");
			}

			ostr.write_boolean(true);
			
			String refString = ORBInterface.orb().object_to_string(coordPtr);
			
			ostr.write_string(refString);
		    }
		    
		    coordPtr = null;
		    controlPtr = null;
		}
		catch (SystemException e)
		{
		    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ClientContextFilter.send_request - caught unexpected exception: "+e);
		}
	    }

	    OTS_ContextFilterManager.enable(currentThread);
	}

	/*
	 * Always write the start offset to the end of our
	 * message. The server will use this to determine
	 * where to start reading from.
	 */
	
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
	if (DebugController.enabled())
	{
	    String operation = ((closure.object != null) ? (String) closure.object : "Unknown");

	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ClientContextFilter.receive_reply for "+operation);
	}
	
	return null;
    }

public void receive_reply_failed (int req_id, org.omg.CORBA.Environment env, Closure closure)
    {
    }

public void exception_occurred (int req_id, org.omg.CORBA.Environment env, Closure closure)
    {
    }

private boolean otsHaveChecked = false;
private boolean otsNeedTranContext = OTS_Defaults.needTransactionContext;
private boolean otsAlwaysPropagate = OTS_Defaults.alwaysPropagateContext;
private org.omg.CORBA.Object target = null;
    
};
