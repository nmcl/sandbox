/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TxActivity.java,v 1.2 2002/04/22 14:04:17 nmcl Exp $
 */

package com.arjuna.Activity.ArjunaOTS;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.Activity.*;
import com.arjuna.Activity.ActivityCoordinator.*;
import com.arjuna.CosTransactions.ArjunaOTS.TxAssociation;
import com.arjuna.CosTransactions.OTS_ControlWrapper;
import org.omg.CosActivity.*;
import org.omg.CosTransactions.Control;
import org.omg.CosTransactions.Coordinator;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_OPERATION;
import org.omg.CORBA.BAD_PARAM;

/*
 * This is specific to the Arjuna transaction service and is a 
 * way of getting transactions to register themselves with activities
 * when they are created, and removed when they are terminated. If
 * the Arjuna OTS isn't being used then something else will have to be
 * substituted for this.
 */

public class TxActivity implements TxAssociation
{
    /*
     * Should throw system exceptions - am using an older version of
     * JTSArjuna.
     */

public void begin (OTS_ControlWrapper tx) throws SystemException
    {
	if (tx != null)
	{	
	    modifyActivity(tx, true);
	}
    }
    
public void commit (OTS_ControlWrapper tx) throws SystemException
    {
	if (tx != null)
	{
	    modifyActivity(tx, false);
	}
    }

public void rollback (OTS_ControlWrapper tx) throws SystemException
    {
	if (tx != null)
	{
	    modifyActivity(tx, false);
	}
    }
	
public void suspend (OTS_ControlWrapper tx) throws SystemException
    {
    }
    
public void resume (OTS_ControlWrapper tx) throws SystemException
    {
    }

public String name ()
    {
	return "TxActivity";
    }

private final void modifyActivity (OTS_ControlWrapper tx,
				   boolean add) throws SystemException
    {
	try
        {
	    org.omg.CosActivity.Current current = CosActivity.get_current();

	    if (current != null)
	    {
		ActivityCoordinator act = current.get_coordinator();
		
		if (act != null)
		{
		    ActivityCoordinatorImple aw = ActivityCoordinatorImple.getActivityCoordinatorImple(act);

		    if (aw == null)
		    {
			throw new BAD_OPERATION("No such activity coordinator!");
		    }
		    else
		    {
			try
			{
			    if (add)
				aw.addTransaction(tx);
			    else
				aw.removeTransaction(tx);
			}
			catch (BAD_PARAM ex)
			{
			    // transaction wraps activities - cancel them

			    cancelActivities(aw, tx);

			    throw ex;
			}
		    }
		}
	    }
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	    
	    throw new BAD_OPERATION(e.toString());
	}
    }
    
private final void cancelActivities (ActivityCoordinatorImple aw, OTS_ControlWrapper tx) throws SystemException
    {
	Coordinator currCoord = null;
	
	try
	{
	    currCoord = tx.get_coordinator();
	}
	catch (Exception ex)
	{
	    throw new BAD_OPERATION(ex.toString());
	}
	
	try
	{
	    boolean finished = false;
	    
	    while (!finished)
	    {
		Control enclosingTransaction = aw.getEnclosingTransaction();
	    
		if (enclosingTransaction != null)
		{
		    Coordinator coord = enclosingTransaction.get_coordinator();
	
		    if (coord.is_same_transaction(currCoord))
			finished = true;
		    else
		    {
			coord.rollback_only();

			aw.complete_activity(null, CompletionStatus.CompletionStatusFailOnly);
		    }

		    aw = aw.getParentCoordinator();
		}

		if (aw == null)
		{
		    ErrorStream.warning().println("TxActivity - could not terminate to parent!");
		    
		    finished = true;
		}
	    }
	}
	catch (Exception ex)
	{
	    throw new BAD_OPERATION(ex.toString());
	}
    }
    
};
