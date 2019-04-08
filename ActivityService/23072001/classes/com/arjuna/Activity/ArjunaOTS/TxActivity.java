/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TxActivity.java,v 1.1 2001/07/23 13:44:54 nmcl Exp $
 */

package com.arjuna.Activity.ArjunaOTS;

import com.arjuna.Activity.*;
import com.arjuna.Activity.ActivityCoordinator.*;
import com.arjuna.CosTransactions.ArjunaOTS.TxAssociation;
import com.arjuna.CosTransactions.OTS_ControlWrapper;
import org.omg.CosActivity.*;
import org.omg.CosTransactions.Control;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_OPERATION;

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

private void modifyActivity (OTS_ControlWrapper tx,
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
			if (add)
			    aw.addTransaction(tx);
			else
			    aw.removeTransaction(tx);
		    }
		}
	    }
	}
	catch (Exception e)
	{
	    throw new BAD_OPERATION(e.toString());
	}
    }
    
};
