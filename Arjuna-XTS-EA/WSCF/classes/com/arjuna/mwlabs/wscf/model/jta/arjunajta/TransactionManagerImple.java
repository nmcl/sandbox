/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionManagerImple.java,v 1.1 2003/01/07 10:33:53 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.jta.arjunajta;

import javax.transaction.*;

import com.arjuna.mw.wsas.UserActivityFactory;
import com.arjuna.mw.wsas.UserActivity;

import com.arjuna.mw.wsas.activity.ActivityHierarchy;

import javax.transaction.NotSupportedException;
import javax.transaction.RollbackException;
import javax.transaction.HeuristicMixedException;
import javax.transaction.HeuristicRollbackException;
import javax.transaction.HeuristicMixedException;
import javax.transaction.SystemException;
import java.lang.SecurityException;
import java.lang.IllegalStateException;
import java.lang.NullPointerException;

import java.util.Hashtable;

import com.arjuna.mw.wsas.exceptions.InvalidActivityException;

public class TransactionManagerImple extends BaseTransactionImple implements javax.transaction.TransactionManager
{

    public TransactionManagerImple ()
    {
	_coordControl = new CoordinatorControl();
	_transactions = new Hashtable();
    }
    
    public Transaction getTransaction () throws javax.transaction.SystemException
    {
	return _coordControl.transactionManager().getTransaction();
    }
 
    /**
     * @return the suspended transaction.
     */

    public Transaction suspend () throws javax.transaction.SystemException
    {
	try
	{
	    ActivityHierarchy hier = UserActivityFactory.userActivity().suspend();
	    Transaction tx = _coordControl.transaction(hier);

	    _transactions.put(tx, hier);

	    return tx;
	}
	catch (com.arjuna.mw.wsas.exceptions.SystemException ex)
	{
	    throw new javax.transaction.SystemException(ex.toString());
	}
    }

    public void resume (Transaction which) throws InvalidTransactionException, java.lang.IllegalStateException, javax.transaction.SystemException
    {
	try
	{
	    ActivityHierarchy hier = (ActivityHierarchy) _transactions.get(which);

	    if ((which != null) && (hier == null))
		throw new InvalidTransactionException();
	    
	    UserActivityFactory.userActivity().resume(hier);
	}
	catch (InvalidActivityException ex)
	{
	    throw new InvalidTransactionException();
	}
	catch (com.arjuna.mw.wsas.exceptions.SystemException ex)
	{
	    throw new javax.transaction.SystemException(ex.toString());
	}
    }

    private CoordinatorControl _coordControl;
    private Hashtable          _transactions;
    
}
