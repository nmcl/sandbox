/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ControlWrapper.javatmpl,v 1.1.2.1.2.1.8.1.6.1.2.3.2.3.2.2.22.1 2001/05/15 11:16:46 nmcl Exp $
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




package com.arjuna.CosTransactions;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Atomic.BasicAction;
import com.arjuna.CosTransactions.tx.XID;
import org.omg.CosTransactions.*;
import org.omg.CORBA.CompletionStatus;

import java.lang.NullPointerException;
import org.omg.CosTransactions.SubtransactionsUnavailable;
import org.omg.CosTransactions.NoTransaction;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CosTransactions.InvalidControl;
import org.omg.CORBA.SystemException;
import org.omg.CORBA.UserException;
import org.omg.CORBA.UNKNOWN;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.BAD_OPERATION;

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
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */



import org.omg.CosTransactions.WrongTransaction;

import org.omg.CORBA.INVALID_TRANSACTION;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CORBA.TRANSACTION_ROLLEDBACK;





/**
 * This class attempts to mask the local/remote control issue.
 * We try to use local controls directly as much as possible and
 * not register them with the ORB until the last minute. This
 * improves performance *significantly*. At present we only do this
 * for top-level transactions, but extending for nested transactions
 * is straightforward.
 *
 * It also acts as a convenience class for ease of use. Therefore, some
 * Coordinator and Terminator methods may be found directly on this
 * class. Because of the way in which the implementation works, however, some
 * of their signatures may be slightly different.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_ControlWrapper.javatmpl,v 1.1.2.1.2.1.8.1.6.1.2.3.2.3.2.2.22.1 2001/05/15 11:16:46 nmcl Exp $
 * @since JTS 2.0.
 */

/*
 * We create and destroy instances of this class regularly simply because
 * otherwise we would never know.
 */

public class OTS_ControlWrapper
{

public OTS_ControlWrapper (Control c)
    {
	this(c, (OTS_Control) null);
    }

public OTS_ControlWrapper (OTS_Control impl)
    {
	_control = null;
	_controlImpl = impl;
	_checkedLocality = true;
	_theUid = impl.get_uid();
    }

public OTS_ControlWrapper (Control c, OTS_Control impl)
    {
	_control = c;
	_controlImpl = impl;
	_checkedLocality = (impl != null);
	_theUid = ((impl == null) ? Uid.nullUid() : impl.get_uid());
    }

public OTS_ControlWrapper (Control c, Uid u)
    {
	_control = c;
	_controlImpl = null;
	_checkedLocality = false;
	_theUid = u;
    }

public final Uid get_uid ()
    {
	return _theUid;
    }
    
public final OTS_ControlWrapper create_subtransaction () throws Unavailable, Inactive, SubtransactionsUnavailable, SystemException
    {
	Coordinator coord = null;

	try
	{
	    coord = ((_control != null) ? _control.get_coordinator() : _controlImpl.get_coordinator());
	}
	catch (SystemException e)
	{
	    coord = null;
	}

	if (coord != null)
	{
	    return new OTS_ControlWrapper(coord.create_subtransaction());
	}
	else
	{
	    if (DebugController.enabled())
	    {
		DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
						   FacilityCode.FAC_OTS, "OTS_ControlWrapper::create_subtransaction - subtransaction parent is inactive.");
	    }


	    throw new INVALID_TRANSACTION (OTS_Exceptions.UNAVAILABLE_COORDINATOR, CompletionStatus.COMPLETED_NO);

	}
    }


public final void commit (boolean report_heuristics) throws Unavailable, HeuristicMixed, HeuristicHazard, SystemException

    {
	try
	{
	    if (_controlImpl != null)
		_controlImpl.getImplHandle().commit(report_heuristics);
	    else
	    {
		Terminator t = null;
	    
		try
		{
		    t = _control.get_terminator();
		}
		catch (SystemException e)
		{
		    throw new Unavailable();
		}

		if (t != null)
		    t.commit(report_heuristics);
		else
		    throw new Unavailable();
	    }
	}
	catch (NullPointerException ex)
	{
	    throw new Unavailable();
	}
    }

public final void rollback () throws Unavailable, NoTransaction, SystemException
    {
	try
	{
	    if (_controlImpl != null)
		_controlImpl.getImplHandle().rollback();
	    else
	    {
		Terminator t = null;
	    
		try
		{
		    t = _control.get_terminator();
		}
		catch (SystemException e)
		{
		    throw new Unavailable();
		}

		if (t != null)
		    t.rollback();
		else
		    throw new Unavailable();
	    }
	}
	catch (NullPointerException ex)
	{
	    throw new Unavailable();
	}
    }

public final void rollback_only () throws Unavailable, NoTransaction, Inactive, SystemException
    {
	try
	{
	    if (_controlImpl != null)
		_controlImpl.getImplHandle().rollback_only();
	    else
	    {
		Coordinator c = null;
	    
		try
		{
		    c = _control.get_coordinator();
		}
		catch (SystemException e)
		{
		    throw new Unavailable();
		}

		if (c != null)
		    c.rollback_only();
		else
		    throw new Unavailable();	    	    
	    }
	}
	catch (NullPointerException ex)
	{
	    throw new Unavailable();
	}
    }

public final org.omg.CosTransactions.RecoveryCoordinator register_resource (Resource r) throws Inactive, SystemException
    {
	try
	{
	    if (_controlImpl != null)
		return _controlImpl.getImplHandle().register_resource(r);
	    else
	    {
		try
		{
		    Coordinator coord = _control.get_coordinator();
		
		    return coord.register_resource(r);
		}
		catch (Unavailable e2)
		{
		    throw new Inactive();
		}
		catch (SystemException e3)
		{
		    throw new UNKNOWN();
		}
	    }
	}
	catch (NullPointerException e1)
	{
	    // no transaction

	    throw new Inactive();
	}
    }

public final void register_synchronization (Synchronization sync) throws Inactive, SynchronizationUnavailable, SystemException
    {
	try
	{
	    if (_controlImpl != null)
		_controlImpl.getImplHandle().register_synchronization(sync);
	    else
	    {
		try
		{
		    Coordinator coord = _control.get_coordinator();
		
		    coord.register_synchronization(sync);
		}
		catch (Unavailable e2)
		{
		    throw new Inactive();
		}
		catch (Exception e3)
		{
		    throw new UNKNOWN();
		}
	    }
	}
	catch (NullPointerException e1)
	{
	    // not available

	    throw new Inactive();
	}
    }

public final org.omg.CosTransactions.Status get_status () throws SystemException
    {
	try
	{
	    if (_controlImpl != null)
		return _controlImpl.getImplHandle().get_status();
	    else
	    {
		Coordinator c = null;

		try
		{
		    c = _control.get_coordinator();
		}
		catch (Unavailable e)
		{
		    c = null;
		}

		if (c != null)
		{
		    try
		    {
			return c.get_status();
		    }
		    catch (Exception e)
		    {
			return org.omg.CosTransactions.Status.StatusUnknown;
		    }
		}
		else
		    return org.omg.CosTransactions.Status.StatusNoTransaction;
	    }
	}
	catch (NullPointerException ex)
	{
	    return org.omg.CosTransactions.Status.StatusNoTransaction;
	}
    }

public final XID get_xid (boolean branch) throws SystemException
    {
	if (_controlImpl != null)
	    return OTS_Utility.getXid(_controlImpl.get_uid(), branch);
	else
	{
	    try
	    {
		return OTS_Utility.getXid(_control, branch);
	    }
	    catch (Exception e)
	    {
		throw new BAD_OPERATION();
	    }
	}
    }
    
public final String get_transaction_name () throws SystemException	
    {
	try
	{
	    if (_controlImpl != null)
		return _controlImpl.getImplHandle().get_transaction_name();
	    else
	    {
		try
		{
		    return _control.get_coordinator().get_transaction_name();
		}
		catch (Unavailable e1)
		{
		    return null;
		}
		catch (SystemException e2)
		{
		    throw e2;
		}
	    }
	}
	catch (NullPointerException e3)
	{
	    return null;
	}
    }

public final Control get_control () throws org.omg.CosTransactions.Unavailable, SystemException
    {
	try
	{
	    if (_controlImpl != null)
		return _controlImpl.getControl();
	    else
		return _control;
	}
	catch (NullPointerException e)
	{
	    return null;
	}
    }
    
public final Coordinator get_coordinator () throws SystemException, org.omg.CosTransactions.Unavailable
    {
	try
	{
	    if (_controlImpl != null)
		return _controlImpl.get_coordinator();
	    else
		return _control.get_coordinator();
	}
	catch (NullPointerException e)
	{
	    throw new org.omg.CosTransactions.Unavailable();
	}
    }
    
public final Terminator get_terminator () throws SystemException, org.omg.CosTransactions.Unavailable
    {
	try
	{
	    if (_controlImpl != null)
		return _controlImpl.get_terminator();
	    else
		return _control.get_terminator();
	}
	catch (NullPointerException e)
	{
	    throw new org.omg.CosTransactions.Unavailable();
	}
    }

public final int hash_transaction () throws SystemException
    {
	try
	{
	    if (_controlImpl != null)
		return _controlImpl.getImplHandle().hash_transaction();
	    else
	    {
		try
		{
		    Coordinator coord = _control.get_coordinator();
		
		    return coord.hash_transaction();
		}
		catch (SystemException ex)
		{
		    throw ex;
		}
	    }
	}
	catch (Exception e)
	{
	    return -1;
	}
    }

    /**
     * Overrides Object.equals
     */

public final boolean equals (Object e)
    {
	if (e instanceof OTS_ControlWrapper)
	{
	    OTS_ControlWrapper c = (OTS_ControlWrapper) e;
	    
	    if (c.isLocal() && isLocal())
		return c.getImple().equals(_controlImpl);
	    else
	    {
		/*
		 * One of them is not local, so we have to revert to
		 * indirect comparison.
		 */

		Coordinator coord = null;
		
		try
		{
		    coord = _control.get_coordinator();
		}
		catch (Exception e1)
		{
		    return false;
		}
		
		Coordinator myCoord = null;
		    
		try
		{
		    myCoord = get_coordinator();
		}
		catch (Exception e2)
		{
		    return false;
		}

		try
		{
		    return coord.is_same_transaction(myCoord);
		}
		catch (Exception e3)
		{
		}		
	    }
	}
	
	return false;
    }

    /**
     * Override Object.toString.
     *
     * @since JTS 2.1.1.
     */

public String toString ()
    {
	try
	{
	    return get_transaction_name();
	}
	catch (Exception e)
	{
	    return "Invalid";
	}
    }

    /**
     * Override Object.hashCode. We always return a positive value.
     */

public int hashCode ()
    {
	try
	{
	    return hash_transaction();
	}
	catch (Exception e)
	{
	    return -1;
	}
    }
	
public final Control getControl ()
    {
	return _control;
    }
    
public final OTS_Control getImple ()
    {
	return _controlImpl;
    }
    
public final boolean isLocal ()
    {
	return ((_controlImpl == null) ? false : true);
    }

    /**
     * Determine whether or not we are a local control.
     * Only do this once since locality is not likely to
     * change!
     */

public final void determineLocality ()
    {
	if (!_checkedLocality)
	{
	    _controlImpl = OTS_Helper.localControl(_control);

	    /*
	     * Could be a proxy for a remote control, in which case
	     * we say we are remote.
	     */

	    if ((_controlImpl != null) && (_controlImpl.getImplHandle() == null))
	    {
		_controlImpl = null;
		_theUid = _controlImpl.get_uid();
	    }
	    
	    _checkedLocality = true;
	}
    }

private Control     _control;
private OTS_Control _controlImpl;
private boolean     _checkedLocality;
private Uid         _theUid;
    
}
