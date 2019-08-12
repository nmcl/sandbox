/*
 * Copyright (C) 1999, 2000,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ContextFilter.java,v 1.3.4.1.4.2.2.1.22.1 2000/11/16 15:34:24 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Context.orbacus;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;
import com.ooc.CORBA.Interceptor;
import java.util.Hashtable;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CosTransactions.Unavailable;

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

/*
 * Because we have process and object filters we have previously
 * registered a process-level thread filter which we assume
 * will be called before any incoming object-level filters, and after any
 * outgoing object-level filters. If the ORBacus implementation
 * changes, we may have to do explicit filter management here
 * (similar to RCP-ORB).
 */

public class OTS_ContextFilter extends Interceptor implements ORBFilter
{

public org.omg.CORBA.portable.OutputStream preRequest (org.omg.CORBA.portable.ObjectImpl obj,
						       String op,
						       org.omg.CORBA.portable.OutputStream buf) throws java.io.IOException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextFilter::preRequest ( "+op+" )");
	}

	if (op.compareTo("_is_a") == 0)
	    return buf;

	Thread currentThread = Thread.currentThread();
	com.ooc.CORBA.OutputStream theBuff = (com.ooc.CORBA.OutputStream) buf;
	String stringRef = null;
	int size = 0;
	boolean problem = false;

	try
	{
	    if (!otsAlwaysPropagate)
	    {
		TransactionalObject ptr = org.omg.CosTransactions.TransactionalObjectHelper.narrow(obj);

		if (ptr == null)
		    throw new BAD_PARAM();
		
		ptr = null;
	    }

	    OTS_Current current = OTS.current();
	    Control theControl = current.get_control();

	    if (theControl != null)
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
	    else
		problem = true;
	}
	catch (BAD_PARAM exEnv)
	{
	    // narrow failed, so not a transactional object.

	    problem = true;
	}
	catch (Exception e)
	{
	    /*
	     * Something bad happened, so leave now!
	     */
	
	    return buf;
	}
    
	if (problem)
	{
	    /*
	     * Only throw an exception if we have no transaction context
	     * and we require one.
	     */
	    
	    if (otsNeedTranContext)
		throw new TRANSACTION_REQUIRED();
	}

	int oldLen = theBuff._OB_buffer().length;
	int sizeOfInt = 4;
	int len = oldLen+sizeOfInt+size+magicBoundary.length()+14;
    
	theBuff._OB_capacity(len);
	theBuff.write_string(magicBoundary);
	theBuff.write_ulong(size);
	
	if (size > 0)
	{
	    theBuff.write_string(stringRef);
	    stringRef = null;
	}

	return theBuff;
    }

public org.omg.CORBA.portable.InputStream postRequest (org.omg.CORBA.portable.ObjectImpl obj,
						       org.omg.CORBA.portable.InputStream buf) throws java.io.IOException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextFilter::postRequest ()");
	}

	return buf;
    }

public org.omg.CORBA.portable.InputStream preDispatch (org.omg.CORBA.DynamicImplementation p, String op,
						       org.omg.CORBA.portable.InputStream buf) throws java.io.IOException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextFilter::preDispatch ( "+op+" )");
	}

	if (op.compareTo("_is_a") == 0)
	    return buf;

	Thread currentThread = Thread.currentThread();
	com.ooc.CORBA.InputStream theBuff = (com.ooc.CORBA.InputStream) buf;
	boolean problem = false;
	org.omg.CORBA.portable.ObjectImpl obj = (org.omg.CORBA.portable.ObjectImpl) p;
	int oldPos = theBuff.pos_;
	
	try
	{
	    /*
	     * If we always propagate the context, then it does not matter
	     * whether we are a TransactionalObject or not - we require something,
	     * even if it is null.
	     */
	
	    if (!otsAlwaysPropagate)
	    {
		TransactionalObject ptr = org.omg.CosTransactions.TransactionalObjectHelper.narrow(obj);

		if (ptr == null)
		    throw new BAD_PARAM();

		ptr = null;
	    }
	}
	catch (BAD_PARAM exEnv)
	{
	    /* narrow failed, so not a transactional object. */

	    problem = true;
	}
	catch (Exception e)
	{
	    /*
	     * Something bad happened, so leave now!
	     */
	
	    return buf;
	}

	if (!problem || otsNeedTranContext)
	{
	    String stringRef = null;
	    String dummyString = null;
	    int size = 0;

	    try
	    {
		String v = new String(theBuff.buf_);
		int i = v.indexOf(magicBoundary);
		int sizeOfInt = 4;

		if (i == -1) // not an Arjuna buffer!
		    return theBuff;
		else
		{
		    theBuff.pos_ = i - sizeOfInt;
		    dummyString = theBuff.read_string();
		}
	    }
	    catch (Exception e)
	    {
		return theBuff;  // not an Arjuna buffer!
	    }

	    dummyString = null;
	    size = theBuff.read_ulong();

	    if (size > 0)
	    {
		try
		{
		    stringRef = theBuff.read_string();
		    org.omg.CORBA.Object optr = ORBInterface.orb().string_to_object(stringRef);
		    Coordinator theCoordinator = org.omg.CosTransactions.CoordinatorHelper.narrow(optr);

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
		    
		    theCoordinator = null;
		}
		catch (BAD_PARAM exEnv)
		{
		}
		catch (Exception e)
		{
		}
		
		stringRef = null;
	    }
	    else
		problem = true;
	}

	if (problem)
	{
	    /*
	     * Only throw an exception if we have no transaction context
	     * and we require one.
	     */
	    
	    if (otsNeedTranContext)
	    {
		theBuff.pos_ = oldPos;		

		throw new TRANSACTION_REQUIRED();
	    }
	}

	theBuff.pos_ = oldPos;
	
	return theBuff;
    }

public org.omg.CORBA.portable.OutputStream postDispatch (org.omg.CORBA.DynamicImplementation p, String op,
							 org.omg.CORBA.portable.OutputStream buf) throws java.io.IOException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ContextFilter::postDispatch ( "+op+" )");
	}

	if (op.compareTo("_is_a") == 0)
	    return buf;

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
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ContextFilter.postDispatch caught: "+ex);
	    return buf;
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ContextFilter.postDispatch caught exception "+e);
	    return buf;
	}

	return buf;
    }

private static boolean otsAlwaysPropagate = false;
private static boolean otsNeedTranContext = false;
private static boolean otsHaveChecked = false;

private static String magicBoundary = "#ARJUNA#";

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
		if (env.compareTo("NO") == 0)
		    otsAlwaysPropagate = false;
	    }
	    
	    otsHaveChecked = true;
	}
    }

};
