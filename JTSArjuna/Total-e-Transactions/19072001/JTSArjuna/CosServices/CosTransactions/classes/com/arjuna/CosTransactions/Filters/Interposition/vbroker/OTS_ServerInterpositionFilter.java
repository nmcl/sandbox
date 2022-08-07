/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_ServerInterpositionFilter.java,v 1.1.2.2.2.1 2000/11/21 12:03:49 nmcl Exp $
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

public class OTS_ServerInterpositionFilter implements ServerInterceptor
{
    
public OTS_ServerInterpositionFilter ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ServerInterpositionFilter created.");
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

public InputStream receive_request (RequestHeader hdr, org.omg.CORBA.ObjectHolder target,
				    org.omg.CORBA.portable.InputStream buf, Closure closure)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ServerInterpositionFilter.receive_request called for "+hdr.operation);
	}

	if (hdr.operation.compareTo("_is_a") == 0)
	{
	    return null;
	}

	Thread currentThread = Thread.currentThread();
	GiopInputStream istr = (GiopInputStream) buf;
	int dataStart = istr.dataOffset();
	int dataEnd = istr.offset();

	if (OTS_InterpositionFilterManager.isDisabled(currentThread))
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
	
	    OTS_InterpositionFilterManager.disable(currentThread);
	
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

		    OTS_InterpositionFilterManager.enable(currentThread);
		    istr.offset(dataEnd);
		    
		    return null;
		}
		
		tranObject = null;
	    }

	    istr.offset(dataEnd-OTS_ServerInterpositionFilter.dataOffset);
	
	    try
	    {
		istr.offset(istr.read_long());

		PropagationContext ctx = unpackPropagationContext(istr, true);

		if (ctx != null)
		{
		    if (DebugController.enabled())
		    {
			DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
							     (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ServerInterpositionFilter.receive_request got context.");
		    }

		    Control theControl = OTS.get_factory().recreate(ctx);

		    if (theControl != null)
		    {
			OTS_Current current = OTS.current();

			if (DebugController.enabled())
			{
			    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
								 (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ServerInterpositionFilter.receive_request - resuming control for "+hdr.operation);
			}

			current.resume(theControl);

			ctx = null;
			theControl = null;
		    }
		    else
		    {
			ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerInterpositionFilter received transaction context but could not deal with it!");
			
			throw new UNKNOWN();
		    }
		}
		else
		{
		    /*
		     * Only throw an exception if we have no transaction context
		     * and we require one.
		     */
	    
		    if (otsNeedTranContext)
		    {
			OTS_InterpositionFilterManager.enable(currentThread);
			istr.offset(dataEnd);
			
			throw new TRANSACTION_REQUIRED();
		    }
		}
	    }
	    catch (TRANSACTION_ROLLEDBACK ex)
	    {
		istr.offset(dataEnd);
		
		OTS_InterpositionFilterManager.enable(currentThread);

		throw ex;
	    }
	    catch (SystemException e1)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerInterpositionFilter.receive_request - caught unexpected exception: "+e1);

		istr.offset(dataEnd);
		
		OTS_InterpositionFilterManager.enable(currentThread);

		throw e1;
	    }
	    catch (InvalidControl e2)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerInterpositionFilter.receive_request - caught InvalidControl: "+e2);

		istr.offset(dataEnd);
		
		OTS_InterpositionFilterManager.enable(currentThread);

		throw new UNKNOWN(e2.toString());
	    }
	    catch (Exception e3)
	    {
		istr.offset(dataEnd);
		
		OTS_InterpositionFilterManager.enable(currentThread);

		throw new UNKNOWN(e3.toString());		
	    }

	    istr.offset(dataEnd);
	    
	    OTS_InterpositionFilterManager.enable(currentThread);
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
	if (reqHdr.operation.compareTo("_is_a") == 0)
	    return null;
	
	Control c = OTS.current().suspend();

	c = null;

	return null;
    }

public void send_reply_failed (RequestHeader reqHdr, ReplyHeader replyHdr, org.omg.CORBA.Object target, org.omg.CORBA.Environment env, Closure closure)
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
	if (reqHdr.operation.compareTo("_is_a") != 0)
	{
	    Control c = OTS.current().suspend();

	    c = null;
	}
    }

private PropagationContext unpackPropagationContext (GiopInputStream istr)
    {
	return unpackPropagationContext(istr, true);
    }

private PropagationContext unpackPropagationContext (GiopInputStream istr, boolean fullUnpack)
    {
	boolean haveContext = istr.read_boolean();
	PropagationContext ctx = null;

	if (haveContext && fullUnpack)
	{
	    try
	    {
		ctx = PropagationContextHelper.read(istr);
	    }
	    catch (SystemException e1)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerInterpositionFilter.unpackPropagationContext caught system exception: "+e1);

		throw new BAD_PARAM(OTS_Exceptions.BAD_TRANSACTION_CONTEXT, CompletionStatus.COMPLETED_NO);
	    }
	    catch (Exception e2)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerInterpositionFilter.unpackPropagationContext caught exception: "+e2);
		ctx = null;

		throw new UNKNOWN(e2.toString());
	    }
	}

	return ctx;
    }
	
private boolean otsHaveChecked = false;
private boolean otsNeedTranContext = OTS_Defaults.needTransactionContext;
private boolean otsAlwaysPropagate = OTS_Defaults.alwaysPropagateContext;

private static final int dataOffset = 4;
    
};
