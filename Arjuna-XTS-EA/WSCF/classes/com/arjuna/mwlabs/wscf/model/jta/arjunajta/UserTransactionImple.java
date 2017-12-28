/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UserTransactionImple.java,v 1.1 2003/01/07 10:33:53 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.model.jta.arjunajta;

import com.hp.mw.ts.jta.UserTransaction;

import javax.transaction.*;

import javax.transaction.NotSupportedException;
import javax.transaction.RollbackException;
import javax.transaction.HeuristicMixedException;
import javax.transaction.HeuristicRollbackException;
import javax.transaction.HeuristicMixedException;
import javax.transaction.SystemException;
import java.lang.SecurityException;
import java.lang.IllegalStateException;
import java.lang.NullPointerException;

public class UserTransactionImple extends BaseTransactionImple implements javax.transaction.UserTransaction
{
    
    public UserTransactionImple ()
    {
    }
    
}
