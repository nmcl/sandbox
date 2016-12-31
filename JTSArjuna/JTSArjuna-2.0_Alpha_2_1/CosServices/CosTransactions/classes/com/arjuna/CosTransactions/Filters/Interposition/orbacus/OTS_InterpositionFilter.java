/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_InterpositionFilter.java,v 1.1.6.1 2000/05/22 10:23:54 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Interposition.orbacus;

import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;
import org.omg.CosTransactions.*;
import com.ooc.CORBA.*;
import org.omg.CORBA.*;
import org.omg.CORBA.portable.*;
import java.util.Hashtable;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.TRANSACTION_REQUIRED;

/*
 * Only add and remove transactional context if the
 * destination object is inherited from TransactionalObject.
 *
 * When we have finished implementing fully checked transactions
 * we will use the filter to prevent remote access to the factory
 * and the terminator.
 */

/*
 * We could send the transaction context back from the server to
 * the client and use this to lazily register new interposed
 * transactions. We don't do this currently.
 */

/*
 * If we're not within a transaction when we call a TransactionalObject
 * the spec. implies that we should raise a TransactionExpected exception.
 * This is too restrictive, and removed the ability to build modular
 * applications. Therefore, by default we will *not* do this, but this
 * can be overridden with an appropriate environment variable. (Could do
 * this through a separate base class.)
 */

public class OTS_InterpositionFilter extends Interceptor implements ORBFilter
{
    
public org.omg.CORBA.portable.OutputStream preRequest (org.omg.CORBA.portable.ObjectImpl obj,
						       String op,
						       org.omg.CORBA.portable.OutputStream buf) throws java.io.IOException
    {
	if (op.compareTo("_is_a") == 0)
	    return buf;

	boolean problem = false;
	com.ooc.CORBA.OutputStream theBuff = (com.ooc.CORBA.OutputStream) buf;
	
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

	    OTS_Current current = OTS.current();
	    Control theControl = current.get_control();

	    if (theControl != null)
	    {
		Coordinator theCoordinator = theControl.get_coordinator();
		
		if (theCoordinator != null)
		{
		    try
		    {
			PropagationContext ctx = theCoordinator.get_txcontext();
			packPropagationContext(theBuff, ctx);

			ctx = null;
		    }
		    catch (Unavailable ex)
		    {
			/*
			 * We may have to make calls during commit (e.g., after_completion) which
			 * are valid, but which will get Inactive.
			 */
			
			// so transmit nothing!
		    
			packPropagationContext(theBuff, null);
		    }

		    theCoordinator = null;
		}
		else
		    problem = true;

		theControl = null;
	    }
	    else
		problem = true;
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

	if (problem)
	{
	    /*
	     * Only throw an exception if we have no transaction context
	     * and we require one.
	     */
	    
	    if (otsNeedTranContext)
		throw new TRANSACTION_REQUIRED();		
	    else
		packPropagationContext(theBuff, null);
	}

	return theBuff;
    }

public org.omg.CORBA.portable.InputStream postRequest (org.omg.CORBA.portable.ObjectImpl obj,
						       org.omg.CORBA.portable.InputStream buf) throws java.io.IOException
    {
	return buf;
    }

public org.omg.CORBA.portable.InputStream preDispatch (org.omg.CORBA.DynamicImplementation p, String op,
						       org.omg.CORBA.portable.InputStream buf) throws java.io.IOException
    {
	if (op.compareTo("_is_a") == 0)
	    return buf;

	Control theControl = null;
	boolean problem = false;
	com.ooc.CORBA.InputStream theBuff = (com.ooc.CORBA.InputStream) buf;
	org.omg.CORBA.portable.ObjectImpl obj = (org.omg.CORBA.portable.ObjectImpl) p;

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
	    /*
	     * OK, we are transactional and expect some context information, even
	     * if it zero.
	     */
	    
	    PropagationContext ctx = unpackPropagationContext(theBuff);
	    
	    if (ctx != null)
	    {
		TransactionFactory theFactory = OTS.get_factory();

		try
	        {
		    theControl = theFactory.recreate(ctx);
		}
		catch (Exception e)
		{
		    theControl = null;
		}

		if (theControl == null)  // can control legitimately be null?
		{
		    System.err.println("FATAL - OTS_InterpositionFilter received transaction context but could not deal with it!");
		    System.exit(0);
		}

		ctx = null;
	    }
	    else
	    {
		problem = true;
	    }
	}
	else
	    return theBuff;  // not an Arjuna buffer

	if (problem)
	{
	    /*
	     * Only throw an exception if we have no transaction context
	     * and we require one.
	     */
	    
	    if (otsNeedTranContext)
		throw new TRANSACTION_REQUIRED();
	}

	try
	{
	    OTS_Current current = OTS.current();

	    current.resume(theControl);
	    theControl = null;
	}
	catch (Exception e)
	{
	    return buf;
	}

	return theBuff;
    }

public org.omg.CORBA.portable.OutputStream postDispatch (org.omg.CORBA.DynamicImplementation p, String op,
							 org.omg.CORBA.portable.OutputStream buf) throws java.io.IOException
    {
	if (op.compareTo("_is_a") == 0)
	    return buf;

	try
	{
	    Control theControl = OTS.current().suspend();

	    /*
	     * Although these are proxies, we cannot delete them yet.
	     * Wait until the transaction terminates.
	     */

	    theControl = null;
	}
	catch (SystemException ex)
	{
	    System.err.println("WARNING - OTS_InterpositionFilter.postDispatch caught: "+ex);
	    return buf;
	}
	catch (Exception e)
	{
	    System.err.println("WARNING - OTS_InterpositionFilter.postDispatch caught exception.");
	    return buf;
	}

	return buf;
    }

private void packPropagationContext (com.ooc.CORBA.OutputStream buf,
				     PropagationContext ctx)
    {
	int oldLen = buf._OB_buffer().length;
	int haveCtx = 0;
	int sizeOfInt = 4;
	int len = oldLen+sizeOfInt+magicBoundary.length()+20;

	if (ctx != null)
	{
	    haveCtx = 1;
	}

	buf.write_string(magicBoundary);
	buf.write_ulong(haveCtx);
	
	if (haveCtx == 1)
	{
	    PropagationContextHelper.write(buf, ctx);
	}
    }

private PropagationContext unpackPropagationContext (com.ooc.CORBA.InputStream buf)
    {
	int haveCtx = 0;
	int oldPos = buf.pos_;
	String dummyString = null;
	PropagationContext ctx = null;
	String v = new String(buf.buf_);
	int i = v.indexOf(magicBoundary);
	int sizeOfInt = 4;
	
	if (i == -1) // not an Arjuna buffer!
	    return null;
	else
	{
	    buf.pos_ = i - sizeOfInt;
	    dummyString = buf.read_string();
	}

	dummyString = null;
	haveCtx = buf.read_ulong();

	if (haveCtx == 1)
	{
	    ctx = PropagationContextHelper.read(buf);
	}

	buf.pos_ = oldPos;
	
	return ctx;
    }

private static boolean otsAlwaysPropagate = true;
private static boolean otsNeedTranContext = false;
private static boolean otsHaveChecked = false;
private static Hashtable _disabledList = new Hashtable();
private static String magicBoundary = "#ARJUNA#";
	
	static
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
	
};
