/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ContextServerRequestInterceptorImpl.java,v 1.1.2.4.4.2.56.1.42.1.4.1 2002/05/27 10:43:16 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interceptors.Context.orbix2000;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;
import org.omg.CORBA.*;
import org.omg.PortableInterceptor.*; 
import org.omg.PortableInterceptor.ORBInitInfoPackage.*; 
import org.omg.IOP.*;
import org.omg.IOP.CodecFactoryPackage.*;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.NO_IMPLEMENT;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CosTransactions.Unavailable;

/**
 * PortableInterceptor::ServerRequestInterceptor implementation which checks 
 * that a transaction context was received.
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
 * This is too restrictive, and removed the ability to build modular
 * applications. Therefore, by default we will *not* do this, but this
 * can be overridden with an appropriate environment variable. (Could do
 * this through a separate base class.)
 */

class OTS_ContextServerRequestInterceptorImpl extends LocalObject implements ServerRequestInterceptor
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

public OTS_ContextServerRequestInterceptorImpl (int dataSlot, Codec codec)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextServerRequestInterceptorImpl ( "+dataSlot+" )");
	}

	_dataSlot = dataSlot;
	_codec = codec;
    }

public String name ()
    {
	return "OTS_Context";
    }

public void receive_request_service_contexts (ServerRequestInfo request_info) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextServerRequestInterceptorImpl::receive_request_service_contexts ( "+request_info.operation()+" )");
	}

	try
	{
	    try
	    {
		if (!OTS_ContextServerRequestInterceptorImpl.otsAlwaysPropagate)
		{
		    if (!request_info.target_is_a(org.omg.CosTransactions.TransactionalObjectHelper.id()))
			throw new BAD_PARAM();
		}
	    }
	    catch (Exception ex)
	    {
		// just in case the object isn't in the IR.
	    }

	    try
	    {
		ServiceContext serviceContext = null;
		
		try
		{
		    serviceContext = request_info.get_request_service_context(OTS.serviceId);
		}
		catch (BAD_PARAM bp)
		{
		    // no context, so nothing shipped!

		    serviceContext = null;
		}

		if (serviceContext != null)
		{
		    org.omg.CORBA.Any receivedData = _codec.decode_value(serviceContext.context_data, ORBInterface.orb().get_primitive_tc(TCKind.tk_string));

		    /*
		     * Set the slot information for the "current" thread. When
		     * the real invocation thread actually needs to get its
		     * transaction context it must check this slot (if it does
		     * not have a transaction context already) and then do
		     * a resume.
		     */

		    request_info.set_slot(_dataSlot, receivedData);
		}
		else
		{
		    /*
		     * Only throw an exception if we have no transaction
		     * context and we require one.
		     */
	    
		    if (otsNeedTranContext)
			throw new TRANSACTION_REQUIRED();
		}
	    }
	    catch (TRANSACTION_REQUIRED ex)
	    {
		ex.printStackTrace();
		
		throw ex;
	    }
	    catch (Exception e)
	    {
		e.printStackTrace();
	    }
	}
	catch (BAD_PARAM ex)
	{
	}
	catch (Exception exp)
	{
	    exp.printStackTrace();
	}
    }

public void receive_request (ServerRequestInfo request_info) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextServerRequestInterceptorImpl.receive_request ( "+request_info.operation()+" )");
	}
    }

    /**
     * Finished with request, so disassociate this thread from the
     * transaction.
     */

public void send_reply (ServerRequestInfo request_info) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextServerRequestInterceptorImpl::send_reply ( "+request_info.operation()+" )");
	}

	try
	{
	    suspendContext(request_info);
	}
	catch (SystemException ex)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ContextServerRequestInterceptorImpl::send_reply caught: "+ex);

	    throw ex;
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ContextServerRequestInterceptorImpl::send_reply caught exception: "+e);

	    throw new BAD_OPERATION();
	}
    }

public void send_exception (ServerRequestInfo request_info) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextServerRequestInterceptorImpl::send_exception ( "+request_info.operation()+" )");
	}

	try
	{
	    suspendContext(request_info);
	}
	catch (SystemException e1)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ContextServerRequestInterceptorImpl::send_exception caught: "+e1);

	    throw e1;
	}
	catch (Exception e2)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ContextServerRequestInterceptorImpl::send_exception caught exception: "+e2);

	    throw new BAD_OPERATION();
	}
    }

public void send_other (ServerRequestInfo request_info) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextServerRequestInterceptorImpl.send_other ( "+request_info.operation()+" )");
	}

	try
	{
	    suspendContext(request_info);
	}
	catch (SystemException ex)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ContextServerRequestInterceptorImpl::send_other caught: "+ex);

	    throw ex;
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ContextServerRequestInterceptorImpl::send_other caught exception: "+e);

	    throw new BAD_OPERATION();
	}
    }

    /*
     * If there is a thread id associated with PICurrent then it will
     * have been placed there by a server-side thread which executed in
     * the application object and needed to associate an imported
     * transaction with itself. In which case we need to do the
     * equivalent of a suspend to remove the thread from Current and
     * from the current transaction.
     */

private void suspendContext (ServerRequestInfo request_info) throws SystemException, InvalidSlot
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PRIVATE,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextServerRequestInterceptorImpl.suspendContext ( "+request_info.operation()+" )");
	}

	org.omg.CORBA.Any data = request_info.get_slot(_dataSlot);

	if ((data != null) && (data.type().kind().value() != TCKind._tk_null))
	{
	    String threadId = null;
	    
	    try
	    {
		if ((threadId = data.extract_string()) != null)
		{
		    OTS_ControlWrapper ctx = OTS.current().contextManager().popAction(threadId);

		    OTS.current().contextManager().purgeActions(threadId);
		    
		    if (ctx != null)
		    {
			try
			{
			    OTS.destroyControl(ctx.getControl());
			    ctx = null;
			}
			catch (Exception e)
			{
			    ErrorStream.warning().println("OTS_ContextServerRequestInterceptorImpl.suspendContext caught: "+e);
			    
			    throw new UNKNOWN();
			}
		    }
		}
	    }
	    catch (BAD_OPERATION be)
	    {
		// not a string, so still a pgctx
	    }

	    request_info.set_slot(_dataSlot, null);
	}
    }
    
private Codec _codec;
private int   _dataSlot;

private static boolean otsNeedTranContext = OTS_Defaults.needTransactionContext;
private static boolean otsAlwaysPropagate = OTS_Defaults.alwaysPropagateContext;
private static boolean otsHaveChecked = false;
    
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
		if (env.compareTo("YES") == 0)
		    otsAlwaysPropagate = true;
	    }
	    
	    otsHaveChecked = true;
	}
    }

}

