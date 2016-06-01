/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TxActionManager.java,v 1.1 2001/07/23 13:44:52 nmcl Exp $
 */

package com.arjuna.Activity.Actions;

import com.arjuna.CosTransactions.OTS;
import com.arjuna.CosTransactions.OTS_Current;
import org.omg.CosActivity.*;
import org.omg.CORBA.*;
import java.io.PrintWriter;

import org.omg.CosActivity.ActionError;
import org.omg.CosActivity.AlreadyDestroyed;
import org.omg.CORBA.SystemException;

public class TxActionManager
{

public Outcome process_signal (Action act, Signal sig) throws ActionError, SystemException
    {
	Outcome o = null;
	
	try
	{
	    OTS.current().begin();
	
	    o = act.process_signal(sig);
	}
	catch (ActionError e1)
	{
	    doRollback();

	    throw e1;
	}
	catch (SystemException e2)
	{
	    doRollback();

	    throw e2;
	}
	catch (Exception e3)
	{
	    doRollback();

	    throw new BAD_OPERATION(e3.toString());
	}

	doCommit();
	
	return o;
    }

public void destroy (Action act) throws AlreadyDestroyed, SystemException
    {
	try
	{
	    OTS.current().begin();

	    act.destroy();
	}
	catch (AlreadyDestroyed e1)
	{
	    doRollback();

	    throw e1;
	}
	catch (SystemException e2)
	{
	    doRollback();

	    throw e2;
	}
	catch (Exception e3)
	{
	    doRollback();

	    throw new BAD_OPERATION();
	}

	doCommit();
    }

private void doCommit () throws SystemException
    {
	try
	{
	    OTS.current().rollback();
	}
	catch (SystemException e1)
	{
	    throw e1;
	}
	catch (Exception e2)
	{
	    throw new BAD_OPERATION(e2.toString());
	}
    }
	
private void doRollback ()
    {
	try
	{
	    OTS.current().rollback();
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}
    }
    
};
