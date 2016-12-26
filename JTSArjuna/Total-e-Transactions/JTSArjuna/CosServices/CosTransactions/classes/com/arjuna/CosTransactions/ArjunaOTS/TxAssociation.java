/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TxAssociation.java,v 1.3.6.1.2.2.24.1.4.2.34.1 2001/06/15 12:11:02 nmcl Exp $
 */

package com.arjuna.CosTransactions.ArjunaOTS;

import com.arjuna.CosTransactions.OTS_ControlWrapper;

import org.omg.CORBA.SystemException;

/**
 * Instances of these classes are informed whenever a
 * transaction is begun/suspended/resumed/ended.
 * The instance can control whether it wants to be informed
 * about all transactions, or just subtransactions.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: TxAssociation.java,v 1.3.6.1.2.2.24.1.4.2.34.1 2001/06/15 12:11:02 nmcl Exp $
 * @since JTS 2.0.
 */

public interface TxAssociation
{
 
/*
 * Since one instance could mark the transaction as rollback
 * we could inform subsequent instances. We don't at present.
 */

public void begin (OTS_ControlWrapper tx) throws SystemException;
public void commit (OTS_ControlWrapper tx) throws SystemException;
public void rollback (OTS_ControlWrapper tx) throws SystemException;
public void suspend (OTS_ControlWrapper tx) throws SystemException;
public void resume (OTS_ControlWrapper tx) throws SystemException;

public String name ();
 
/*
 * We could impose some ordering constraints
 * on instances, such that users have the capability
 * of specifying how they are invoked, e.g., A before
 * B. It would affect performance of adding, but if
 * that does not happen frequently it probably doesn't
 * matter.
 */
 
};
