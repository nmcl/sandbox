/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_ServerContextFilter.java,v 1.1.2.2.2.1 2000/11/21 12:03:48 nmcl Exp $
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
import org.omg.CORBA.TRANSACTION_ROLLEDBACK;
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

public class OTS_ServerContextFilter implements ServerInterceptor
{
    
public OTS_ServerContextFilter ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ServerContextFilter created.");
	}
	
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

public IOR locate (int req_id, byte[] object_key, Closure closure)
    {
        return null;
    }

public void locate_succeeded (int req_id, Closure closure)
    {
    }

public void locate_forwarded (int req_id, IORHolder forward_ior, Closure closure)
    {
    }

public IOR locate_failed (int req_id, byte[] object_key, Closure closure)
    {
        return null;
    }

    /*
     * Filters in VisiBroker are fairly braindead - they use a single buffer
     * for the entire message and always position it at the start of *all*
     * data. So, we must somehow find our own way to the data for this filter,
     * otherwise we could start using the data meant for the actual object!
     */
    
public InputStream receive_request (RequestHeader hdr, org.omg.CORBA.ObjectHolder target,
				    org.omg.CORBA.portable.InputStream buf, Closure closure)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ServerContextFilter.receive_request called for "+hdr.operation);
	}

	Thread currentThread = Thread.currentThread();
	GiopInputStream istr = (GiopInputStream) buf;
	int dataStart = istr.dataOffset();
	int dataEnd = istr.offset();

	istr.offset(dataEnd-OTS_ServerContextFilter.dataOffset);

	if (OTS_ContextFilterManager.isDisabled(currentThread))
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
	
	    OTS_ContextFilterManager.disable(currentThread);
	
	    if (!otsAlwaysPropagate)
	    {
		TransactionalObject tranObject = null;
	    
		try
		{
		    tranObject = org.omg.CosTransactions.TransactionalObjectHelper.narrow(target.value);

		    if (tranObject == null)
			throw new BAD_PARAM();
		}
		catch (BAD_PARAM e)
		{
		    /*
		     * Not a transactional object so return.
		     * Remember to enable interceptors.
		     */

		    OTS_ContextFilterManager.enable(currentThread);
		    istr.offset(dataEnd);
		    
		    return null;
		}
		
		tranObject = null;
	    }
	
	    try
	    {
		istr.offset(istr.read_long());
		
		boolean haveContext = istr.read_boolean();

		if (haveContext)
		{
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							   (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ServerContextFilter.receive_request got context.");
		    }

		    String refString = istr.read_string();
		    
		    org.omg.CORBA.Object obj = ORBInterface.orb().string_to_object(refString);
		    Coordinator theCoordinator = org.omg.CosTransactions.CoordinatorHelper.narrow(obj);
		    Control theControl = OTS_Factory.createProxy(theCoordinator, null);
		    OTS_Current current = OTS.current();

		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							   (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ServerContextFilter.receive_request - resuming control.");
		    }

		    current.resume(theControl);

		    theControl = null;
		    theCoordinator = null;
		}
		else
		{
		    /*
		     * Only throw an exception if we have no transaction context
		     * and we require one.
		     */
	    
		    if (otsNeedTranContext)
		    {
			OTS_ContextFilterManager.enable(currentThread);
			istr.offset(dataEnd);
			
			throw new TRANSACTION_REQUIRED();
		    }
		}
	    }
	    catch (TRANSACTION_ROLLEDBACK ex)
	    {
		istr.offset(dataEnd);
	    
		OTS_ContextFilterManager.enable(currentThread);

		throw ex;
	    }
	    catch (SystemException e1)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerContextFilter.receive_request - caught unexpected exception: "+e1);

		istr.offset(dataEnd);
	    
		OTS_ContextFilterManager.enable(currentThread);

		throw e1;
	    }
	    catch (InvalidControl e2)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerContextFilter.receive_request - caught InvalidControl: "+e2);

		istr.offset(dataEnd);
	    
		OTS_ContextFilterManager.enable(currentThread);

		throw new UNKNOWN(e2.toString());
	    }

	    istr.offset(dataEnd);
	    
	    OTS_ContextFilterManager.enable(currentThread);
	}

	return null;
    }

public void prepare_reply (RequestHeader hdr, ReplyHeaderHolder reply,
			   org.omg.CORBA.Object target, Closure closure)
    {
    }

public org.omg.CORBA.portable.OutputStream send_reply (RequestHeader reqHdr, ReplyHeader hdr,
						       org.omg.CORBA.Object target,
						       org.omg.CORBA.portable.OutputStream buf,
						       org.omg.CORBA.Environment env,
						       Closure closure)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ServerContextFilter.send_reply for "+reqHdr.operation);
	}
	
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
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerContextFilter.send_reply caught: "+e);
	}
	
	return null;
    }

public void send_reply_failed (RequestHeader reqHdr, ReplyHeader replyHdr,
			       org.omg.CORBA.Object target, org.omg.CORBA.Environment env, Closure closure)
    {
    }

public void request_completed (RequestHeader reqHdr, org.omg.CORBA.Object target, Closure closure)
    {
    }

public void shutdown (com.visigenic.vbroker.interceptor.ServerInterceptorPackage.ShutdownReason reason)
    {
    }

public void exception_occurred (RequestHeader reqHdr, org.omg.CORBA.Environment env, Closure closure)
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
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerContextFilter.exception_occurred caught: "+e);
	}
    }

private boolean otsHaveChecked = false;
private boolean otsNeedTranContext = OTS_Defaults.needTransactionContext;
private boolean otsAlwaysPropagate = OTS_Defaults.alwaysPropagateContext;

private static final int dataOffset = 4;
    
};
