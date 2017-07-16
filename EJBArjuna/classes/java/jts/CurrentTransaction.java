/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

package java.jts;

import java.lang.IllegalStateException;
import java.lang.IllegalArgumentException;
import java.lang.SecurityException;

public interface CurrentTransaction
{

public abstract void begin () throws IllegalStateException;
public abstract void commit () throws TransactionRolledbackException, HeuristicMixedException, HeuristicRollbackException, SecurityException, IllegalStateException;
public abstract TransactionControl getControl ();
public abstract int getStatus ();
public abstract void resume (TransactionControl suspended) throws IllegalArgumentException;
public abstract void rollback () throws IllegalStateException, SecurityException;
public abstract void rollbackOnly () throws IllegalStateException;
public abstract void setTransactionTimeout (int seconds);
public abstract TransactionControl suspend ();

};
