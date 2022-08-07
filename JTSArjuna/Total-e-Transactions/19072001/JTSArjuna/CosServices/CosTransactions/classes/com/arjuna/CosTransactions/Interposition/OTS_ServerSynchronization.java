/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerSynchronization.javatmpl,v 1.1.8.2.20.1.2.1 2000/10/20 16:17:50 nmcl Exp $
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

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.OrbCommon.OAInterface;
import com.arjuna.CosTransactions.OTS_Exceptions;
import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.ArjunaCore.Atomic.PrepareOutcome;
import org.omg.CosTransactions.*;
import org.omg.CORBA.CompletionStatus;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_OPERATION;

class OTS_ServerSynchronization extends  org.omg.CosTransactions . SynchronizationPOA 
{

public OTS_ServerSynchronization (OTS_ServerTransaction topLevel)
    {
	OAInterface.objectIsReady(this);
	
	_theTransaction = topLevel;
	_theSynchronization =  org.omg.CosTransactions.SynchronizationHelper.narrow(OAInterface.corbaReference( this )) ;
	
    }

public void finalize ()
    {
	_theTransaction = null;
	_theSynchronization = null;
    }
 
public final Synchronization getSynchronization ()
    {
	return _theSynchronization;
    }
 
public void destroy ()
    {
	try
	{
	    OAInterface.shutdownObject(this);
	}
	catch (Exception e)
	{
	    ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerSynchronization::destroy - failed!");
	}
    }

public void before_completion () throws SystemException
    {
	if (_theTransaction == null)
	    throw new BAD_OPERATION(OTS_Exceptions.NO_TRANSACTION, CompletionStatus.COMPLETED_NO);
	else
	{
	    _theTransaction.doBeforeCompletion();
	}
    }

public void after_completion (org.omg.CosTransactions.Status status) throws SystemException
    {
	if (_theTransaction == null)
	{
	    destroy();
	    
	    throw new BAD_OPERATION(OTS_Exceptions.NO_TRANSACTION, CompletionStatus.COMPLETED_NO);
	}
	else
	{
	    /*
	     * Check that the given status is the same as our status. It should be!
	     */

	    org.omg.CosTransactions.Status myStatus = org.omg.CosTransactions.Status.StatusUnknown;
    
	    try
	    {
		myStatus = _theTransaction.get_status();
	    }
	    catch (Exception e)
	    {
		myStatus = org.omg.CosTransactions.Status.StatusUnknown;
	    }

	    if (myStatus != status)
	    {
		ErrorStream.stream(ErrorStream.WARNING).println("OTS_ServerSynchronization.after_completion - different status: < "+myStatus+", "+status+" >");

		/*
		 * There's nothing much we can do, since the transaction
		 * should have completed. The best we can hope for it to try
		 * to rollback our portion of the transaction, but this may
		 * result in heuristics (which may not be reported to the coordinator,
		 * since exceptions from after_completion can be ignored in the spec.)
		 */

		if (myStatus == org.omg.CosTransactions.Status.StatusActive)
		{
		    try
		    {
			_theTransaction.rollback();
		    }
		    catch (Exception e)
		    {
		    }

		    /*
		     * Get the local status to pass to our local synchronizations.
		     */
		
		    try
		    {
			status = _theTransaction.get_status();
		    }
		    catch (Exception e)
		    {
			status = org.omg.CosTransactions.Status.StatusUnknown;
		    }
		}
	    }
	
	    _theTransaction.doAfterCompletion(status);
	}

	/*
	 * Now dispose of self.
	 */

	destroy();
    }

private OTS_ServerTransaction _theTransaction;
private Synchronization _theSynchronization;
 
};
