/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DebugTxAssociation.java,v 1.6.4.1.4.1.2.2.24.1.4.2.34.1 2001/06/15 12:11:02 nmcl Exp $
 */

package com.arjuna.CosTransactions.ArjunaOTS;

import com.arjuna.CosTransactions.OTS_ControlWrapper;

import org.omg.CORBA.SystemException;

/**
 * Whereas TxAssociation classes are on a per
 * thread basis, insances of this type apply to all threads.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: DebugTxAssociation.java,v 1.6.4.1.4.1.2.2.24.1.4.2.34.1 2001/06/15 12:11:02 nmcl Exp $
 * @since JTS 2.0.
 */

public class DebugTxAssociation implements TxAssociation
{
    
public void begin (OTS_ControlWrapper tx) throws SystemException
    {
	String name = ((tx == null) ? "none" : tx.get_transaction_name());
	
	System.err.println("Transaction "+name+" begun");
    }

public void commit (OTS_ControlWrapper tx) throws SystemException
    {
	String name = ((tx == null) ? "none" : tx.get_transaction_name());
	
	System.err.println("Transaction "+name+" commit");
    }

public void rollback (OTS_ControlWrapper tx) throws SystemException
    {
	String name = ((tx == null) ? "none" : tx.get_transaction_name());

	System.err.println("Transaction "+name+" rollback");
    }

public void suspend (OTS_ControlWrapper tx) throws SystemException
    {
	String name = ((tx == null) ? "none" : tx.get_transaction_name());

	System.err.println("Transaction "+name+" suspend");
    }

public void resume (OTS_ControlWrapper tx) throws SystemException
    {
	String name = ((tx == null) ? "none" : tx.get_transaction_name());

	System.err.println("Transaction "+name+" resume");
    }

public String name ()
    {
	return "Debug";
    }

};
