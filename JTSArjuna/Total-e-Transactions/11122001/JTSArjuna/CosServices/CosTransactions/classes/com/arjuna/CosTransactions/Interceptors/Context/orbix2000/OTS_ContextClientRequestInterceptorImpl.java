/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ContextClientRequestInterceptorImpl.java,v 1.1.2.4.72.1.12.1.4.1 2002/05/27 10:43:16 nmcl Exp $
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
import java.util.Hashtable;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CosTransactions.Unavailable;

/**
 * PortableInterceptor::ClientRequestInterceptor implementation which adds a 
 * service context carrying the transaction context.
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

class OTS_ContextClientRequestInterceptorImpl extends LocalObject implements ClientRequestInterceptor
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

public OTS_ContextClientRequestInterceptorImpl (int localSlot, Codec codec)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextClientRequestInterceptorImpl ( "+localSlot+" )");
	}

	_localSlot = localSlot;
	_codec = codec;
    }

public String name ()
    {
	return "OTS_Context";
    }

public void send_request (ClientRequestInfo request_info) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextClientRequestInterceptorImpl::send_request ( "+request_info+" )");
	}

	if (systemCall(request_info))
	    return;
	
	try
	{
	    if (!OTS_ContextClientRequestInterceptorImpl.otsAlwaysPropagate)
	    {
		TransactionalObject ptr = org.omg.CosTransactions.TransactionalObjectHelper.narrow(request_info.target());
	    
		if (ptr == null)
		    throw new BAD_PARAM();
	    }

	    try
	    {
		/*
		 * We get back an Any, which contains a key which we must
		 * now use to get the actual transaction context. This
		 * saves use having to pack and unpack the context every
		 * time it changes, even if we don't then make a remote
		 * invocation.
		 */

		org.omg.CORBA.Any localData = request_info.get_slot(_localSlot);
		String threadId = null;
		boolean problem = false;
		String stringRef = null;

		/*
		 * If we are using co-location optimisations, then
		 * filters may not have been used to set up the
		 * thread-to-context association. So, if the PI slot
		 * is null, check whether the current thread has a context
		 * on it already. If so, use it.
		 */

		if (localData.type().kind().value() != TCKind._tk_null)
		{
		    if ((threadId = localData.extract_string()) == null)
			throw new UNKNOWN("Invalid PI tx parameter!");
		}
		else
		    threadId = Thread.currentThread().getName();

		if (threadId != null)
		{
		    OTS_ControlWrapper theControl = OTS.current().contextManager().current(threadId);

		    if (theControl != null)
		    {
			try
			{
			    Coordinator theCoordinator = theControl.get_coordinator();

			    if (theCoordinator != null)
			    {
				stringRef = ORBInterface.orb().object_to_string(theCoordinator);
			    }
			    else
			    {
				problem = true;
			    }
			}
			catch (Unavailable e)
			{
			    problem = true;
			}
			
			theControl = null;
		    }
		    else
			problem = true;
		}
		else
		    problem = true;

		if (problem)
		{
		    /*
		     * Only throw an exception if we have no
		     * transaction context and we require one.
		     */
			
		    if (otsNeedTranContext)
			throw new TRANSACTION_REQUIRED();
		    else
			stringRef = null;
		}

		if (stringRef != null)
		{
		    org.omg.CORBA.Any data = ORBInterface.orb().create_any();

		    data.insert_string(stringRef);
			
		    byte[] octets = _codec.encode_value(data);
		    ServiceContext service_context = new ServiceContext(OTS.serviceId, octets);

		    request_info.add_request_service_context(service_context, true);
		}
	    }
	    catch (SystemException e)
	    {
		throw e;
	    }
	    catch (Exception ex)
	    {
		ErrorStream.warning().println("OTS_ContextClientRequestInterceptorImpl: "+ex.toString());
		
		ex.printStackTrace();

		throw new BAD_OPERATION();
	    }
	}
	catch (BAD_PARAM ex)
	{
	    // narrow failed, so not a transactional object.
	}
    }

public void send_poll (ClientRequestInfo request_info) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextClientRequestInterceptorImpl::send_poll ( "+request_info+" )");
	}
    }

public void receive_reply (ClientRequestInfo request_info) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextClientRequestInterceptorImpl::receive_reply ( "+request_info+" )");
	}
    }

public void receive_exception (ClientRequestInfo request_info) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextClientRequestInterceptorImpl::receive_exception ( "+request_info+" )");
	}

	// mark transaction as rollback only if a system exception
    }

public void receive_other (ClientRequestInfo request_info) throws SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextClientRequestInterceptorImpl::receive_other ( "+request_info+" )");
	}
    }

private final boolean systemCall (ClientRequestInfo request_info)
    {
	if (request_info.operation().equals("_is_a"))
	    return true;
	else
	    return false;
    }
    
private int   _localSlot;
private Codec _codec;

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

