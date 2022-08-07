/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ExplicitInterposition.javatmpl,v 1.2.6.2.2.2.2.1.6.1.6.1.2.2.4.1.2.2 2001/02/14 12:10:29 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.CosTransactions;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Atomic.BasicAction;
import com.arjuna.ArjunaCore.Thread.ThreadActionData;
import org.omg.CosTransactions.*;
import java.io.PrintStream;
import org.omg.CORBA.CompletionStatus;

import org.omg.CosTransactions.InvalidControl;
import org.omg.CosTransactions.Inactive;
import org.omg.CosTransactions.Unavailable;
import com.arjuna.ArjunaOTS.InterpositionFailed;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.SystemException;

/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranExceptions.h,v 1.1.2.1.28.1.14.1 2001/03/20 16:24:46 nmcl Exp $
 */





/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranNames.h,v 1.1.42.1 2001/03/20 16:24:47 nmcl Exp $
 */




/*
 * Simple renaming macros for portability.
 *
 * Slightly different from C++ version in that we don't need the
 * typedefs. However, we do need some mapping for OTS exceptions which
 * should now be system exceptions.
 */

/*
 * CosTransaction module exceptions
 */






/*
 * We need to check this. WRONG_TRANSACTION is supposed to
 * be a system exception (in the latest draft of the OTS),
 * but two ORBs have it as a user exception.
 */





/**/





/*
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */



import org.omg.CosTransactions.WrongTransaction;

import org.omg.CORBA.INVALID_TRANSACTION;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CORBA.TRANSACTION_ROLLEDBACK;





/**
 * This class is responsible for doing interposition in the case where
 * implicit context propagation is not possible.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_ExplicitInterposition.javatmpl,v 1.2.6.2.2.2.2.1.6.1.6.1.2.2.4.1.2.2 2001/02/14 12:10:29 nmcl Exp $
 * @since JTS 1.0.
 */

public class OTS_ExplicitInterposition
{

    /**
     * Create a new instance and remember any current transaction that
     * may be associated with the invoking thread so that it can be
     * restored once interposition has finished.
     */

public OTS_ExplicitInterposition ()
    {
	this(true);
    }

    /**
     * Create a new instance. Depending upon the value of the boolean
     * parameter, remember any current transaction that
     * may be associated with the invoking thread so that it can be
     * restored once interposition has finished.
     */

public OTS_ExplicitInterposition (boolean remember)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_ExplicitInterposition::OTS_ExplicitInterposition ( "+remember+" )");
	}

	_remember = remember;
	_inUse = false;
	_oldControl = null;
    }

    /**
     * Create a new instance and interpose with the specified transaction.
     * Depending upon the value of the boolean
     * parameter, remember any current transaction that
     * may be associated with the invoking thread so that it can be
     * restored once interposition has finished.
     */


public OTS_ExplicitInterposition (Control action, boolean remember) throws InterpositionFailed, SystemException

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_ExplicitInterposition::OTS_ExplicitInterposition ( Control action, "+remember+" )");
	}

	_remember = remember;
	_inUse = false;
	_oldControl = null;

	registerTransaction(action);
    }

    /**
     * Create a new instance and interpose with the specified transaction.
     * Depending upon the value of the boolean
     * parameter, remember any current transaction that
     * may be associated with the invoking thread so that it can be
     * restored once interposition has finished.
     */


public OTS_ExplicitInterposition (PropagationContext ctx, boolean remember) throws InterpositionFailed, SystemException

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_ExplicitInterposition::OTS_ExplicitInterposition ( PropagationContext ctx, "+remember+" )");
	}

	_remember = remember;
	_inUse = false;
	_oldControl = null;

	registerTransaction(ctx);
    }
    
public void finalize ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.DESTRUCTORS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_ExplicitInterposition.finalize ()");
	}

	if (_inUse)
	{
	    try
	    {
		unregisterTransaction();
	    }
	    catch (Exception e)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ExplicitInterposition.finalize - could not unregister from transaction.");
	    }
	}
    }

    /**
     * Perform interposition with the specified transaction.
     */


public final synchronized void registerTransaction (Control action) throws InterpositionFailed, SystemException

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_ExplicitInterposition::registerTransaction ( Control action )");
	}

	if (_inUse)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ExplicitInterposition.registerTransaction called multiple times.");

	    throw new InterpositionFailed();
	}
	
	boolean registerNull = false;

	if (action != null)
	{
	    try
	    {
		Coordinator coord = action.get_coordinator();
		
		if (coord != null)
		{
		    PropagationContext ctx = coord.get_txcontext();
		    
		    if (ctx != null)
		    {
			try
			{
			    registerTransaction(ctx);
			    ctx = null;
			}
			catch (Exception e)
			{
			    ErrorStream.warning().println(e);
			    
			    ctx = null;

			    throw new InterpositionFailed();
			}
		    }
		    else
			registerNull = true;

		    coord = null;
		}
		else
		    registerNull = true;
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println(e);

		throw new InterpositionFailed();
	    }
	}
	else
	{
	    _inUse = true;
	    registerNull = true;
	}

	if (registerNull)
	{
	    try
	    {
		OTS.current().resume((Control) null);
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println(e);

		throw new InterpositionFailed();
	    }
	}
    }

    /**
     * We need this explicit method because we cannot rely upon
     * the object "destructor" being called by the time the
     * method which instantiated the interposition class ends!
     */
    
public synchronized void unregisterTransaction () throws INVALID_TRANSACTION , SystemException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						 FacilityCode.FAC_OTS, "OTS_ExplicitInterposition::unregisterTransaction ()");
	}

	if (!_inUse)
	    throw new INVALID_TRANSACTION (OTS_Exceptions.INVALID_ACTION, CompletionStatus.COMPLETED_NO);
	else
	{
	    try
	    {
		Control control = OTS.current().suspend();

		control = null;

		if (_remember && (_oldControl != null))
		{
		    try
		    {
			OTS.current().resume(_oldControl);
			_oldControl = null;
		    }
		    catch (Exception exp)
		    {
			ErrorStream.warning().println(exp);
			
			throw new UNKNOWN(exp.toString());
		    }
		}
	    }
	    catch (UNKNOWN exp)
	    {
		throw exp;
	    }
	    catch (SystemException ex)
	    {
		ErrorStream.warning().println(ex);

		_inUse = false;

		throw ex;
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println(e);

		_inUse = false;

		throw new UNKNOWN(e.toString());
	    }

	    _inUse = false;
	}
    }


private final synchronized void registerTransaction (PropagationContext ctx) throws InterpositionFailed, SystemException

    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PRIVATE,
						 FacilityCode.FAC_OTS, "OTS_ExplicitInterposition::registerTransaction ( PropagationContext ctx )");
	}

	if (_inUse)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ExplicitInterposition.registerTransaction called multiple times.");

	    throw new InterpositionFailed();
	}

	if ((ctx == null) || (ctx.currentTransaction.coord == null)) // invalid
	    throw new INVALID_TRANSACTION (OTS_Exceptions.INVALID_ACTION, CompletionStatus.COMPLETED_NO);

	_inUse = true;
	
	OTS_Factory _localFactory = OTS.factory();

	try
	{
	    OTS_Control cont = _localFactory.recreateLocal(ctx);
	    OTS_Current current = OTS.current();
		
	    /*
	     * If this thread is associated with any transactions, then
	     * they will be lost in favour of this new hierarchy, unless
	     * we have remembered them explicitly.
	     */

	    if (_remember)
	    {
		try
		{
		    _oldControl = current.suspend();
		}
		catch (Exception e)
		{
		    ErrorStream.warning().println(e);

		    throw new InterpositionFailed();
		}
	    }
	
	    current.resume(cont.getControl());

	    cont = null;
	}
	catch (InterpositionFailed ex)
	{
	    throw ex;
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ExplicitInterposition.registerTransaction(PropagationContext) caught exception: "+e);

	    e.printStackTrace();
	    
	    throw new InterpositionFailed();
	}
    }

private boolean _remember;
private boolean _inUse;
private Control _oldControl;
    
};
