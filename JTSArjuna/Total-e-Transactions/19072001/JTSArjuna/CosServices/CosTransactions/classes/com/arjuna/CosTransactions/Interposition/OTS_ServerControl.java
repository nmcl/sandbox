/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerControl.javatmpl,v 1.12.6.2.2.2.8.4.10.1.2.1.2.2.2.1 2001/02/14 10:47:19 nmcl Exp $
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
































































package com.arjuna.CosTransactions.Interposition;

import com.arjuna.OrbCommon.*;
import com.arjuna.CosTransactions.OTS_Control;
import com.arjuna.CosTransactions.OTS_Transaction;
import com.arjuna.ArjunaOTS.*;
import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.ArjunaCommon.Template.*;
import org.omg.CosTransactions.*;
import java.util.*;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;

/**
 * As with OTS_Control, the transaction is maintained until the
 * control object is deleted.
 *
 * This class maintains a handle on the current server-side transaction,
 * and also references to the original transaction it is "mirroring". This
 * allows us to have a single place to hold both sets of information which
 * is accessible to interposed resources, synchronizations, and transactions.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_ServerControl.javatmpl,v 1.12.6.2.2.2.8.4.10.1.2.1.2.2.2.1 2001/02/14 10:47:19 nmcl Exp $
 * @since JTS 1.0.
 */

public class OTS_ServerControl extends OTS_Control
{

public OTS_ServerControl ( Uid actUid, Control parentCon, OTS_Transaction parentTran,
			   Coordinator realCoord, Terminator realTerm )
    {
	super();

	_realCoordinator = realCoord;
	_realTerminator = realTerm;

	_parentControl = parentCon;

	/*
	 * allControls only contains local controls.
	 * Have we seen this transaction before, i.e., is it
	 * a locally created transaction that we are re-importing?
	 */

	OTS_Control cont = ((OTS_Control.allControls != null) ? (OTS_Control) OTS_Control.allControls.get(actUid) : null);

	/*
	 * Temporary fix until I have time to rewrite the architecture to
	 * prevent registering interposed hierarchies (and hence creating
	 * proxy transactions/controls) for local hierarchies!
	 */

	if (cont != null)
	{
	    _isWrapper = true;
	    _transactionHandle = cont.getImplHandle();

	    Coordinator coord = null;
	    Terminator term = null;
	    
	    try
	    {
		coord = cont.get_coordinator();
		term = cont.get_terminator();
	    }
	    catch (Exception e)
	    {
		e.printStackTrace();

		try
		{
		    if (coord != null)
			coord.rollback_only();
		}
		catch (Exception ex)
		{
		}
	    }
	    
	    super.duplicateTransactionHandle(coord, term);
	}
	else
	{
	    _transactionHandle = new OTS_ServerTransaction(actUid, _parentControl, parentTran);
	    _isWrapper = false;

	    super.createTransactionHandle();

	    /*
	     * Pass a pointer to the control to the transaction so it knows
	     * what the control is. We use this for transaction comparison and
	     * thread-to-context management.
	     */

	    _transactionHandle.setControlHandle(this);
	}

	_theUid = _transactionHandle.get_uid();

	addControl();
    }

public void finalize () throws Throwable
    {
	_realCoordinator = null;
	_realTerminator = null;

	super.finalize();
    }

public final boolean isWrapper ()
    {
	return _isWrapper;
    }

public Coordinator originalCoordinator ()
    {
	return _realCoordinator;
    }

public Terminator originalTerminator ()
    {
	return _realTerminator;
    }
    
public synchronized void destroy () throws ActiveTransaction, ActiveThreads, BadControl, Destroyed, SystemException
    {
	if (super._destroyed)
	    throw new Destroyed();
	
	/*
	 * We are about to delete ourself (!) so make sure we don't
	 * using anything on the stack after this point. This includes
	 * returning variables.
	 */

	try
	{
	    if (_isWrapper)
		_transactionHandle = null;

	    super.destroy();
	}
	catch (BAD_PARAM e)
	{
	    // already destroyed
	}
	catch (Destroyed de)
	{
	    // already destroyed
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerControl could not destroy object: "+e);
	}
    }

public OTS_ServerControl (OTS_ServerTransaction stx)
    {
	super();

	_realCoordinator = null;
	_realTerminator = null;
	_isWrapper = false;
	
	_transactionHandle = stx;
	_theUid = stx.get_uid();
	
	createTransactionHandle();
	
	addControl();
    }

public String toString ()
    {
	return "OTS_ServerControl < "+get_uid()+" >";
    }

public final boolean forgetHeuristics ()
    {
	return _transactionHandle.forgetHeuristics();
    }
    
protected boolean addControl ()
    {
	synchronized (OTS_ServerControl.allServerControls)
        {
	    OTS_ServerControl.allServerControls.put(get_uid(), this);
	}
	
	return true;
    }

protected boolean removeControl ()
    {
	synchronized (OTS_ServerControl.allServerControls)
        {
	    OTS_ServerControl.allServerControls.remove(get_uid());
	}

	return true;
    }

    /*
     * Make private, with public accessor.
     */

public static Hashtable allServerControls = new Hashtable();
    
private Coordinator _realCoordinator;
private Terminator  _realTerminator;
private boolean     _isWrapper;
    
};
