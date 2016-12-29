/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DebugTxAssociation.java,v 1.6.4.1.4.1 2000/05/22 10:23:53 nmcl Exp $
 */

package com.arjuna.CosTransactions.ArjunaOTS;

import org.omg.CosTransactions.*;
import com.arjuna.CosTransactions.*;

/*
 * Whereas TxAssociation classes are on a per
 * thread basis, insances of this type apply to all threads.
 */

public class DebugTxAssociation implements TxAssociation
{
    
public void begin (Control tx)
    {
	System.err.println("Transaction "+txName(tx)+" begun");
    }

public void begin (OTS_Control tx)
    {
	System.err.println("Transaction "+txName(tx)+" begun");
    }
    
public void commit (Control tx)
    {
	System.err.println("Transaction "+txName(tx)+" commit");
    }

public void commit (OTS_Control tx)
    {
	System.err.println("Transaction "+txName(tx)+" commit");
    }

public void rollback (Control tx)
    {
	System.err.println("Transaction "+txName(tx)+" rollback");
    }

public void rollback (OTS_Control tx)
    {
	System.err.println("Transaction "+txName(tx)+" rollback");
    }
	
public void suspend (Control tx)
    {
	System.err.println("Transaction "+txName(tx)+" suspend");
    }

public void suspend (OTS_Control tx)
    {
	System.err.println("Transaction "+txName(tx)+" suspend");
    }
    
public void resume (Control tx)
    {
	System.err.println("Transaction "+txName(tx)+" resume");
    }

public void resume (OTS_Control tx)
    {
	System.err.println("Transaction "+txName(tx)+" resume");
    }

public String name ()
    {
	return "Debug";
    }

private String txName (OTS_Control tx)
    {
	if (tx != null)
	    return tx.getImplHandle().get_transaction_name();
	else
	    return null;
    }
	    
private String txName (Control tx)
    {
	String name = "null";
	
	try
	{
	    if (tx != null)
	    {
		Coordinator c = tx.get_coordinator();
		
		if (c != null)
		    name = c.get_transaction_name();
	    }
	}
	catch (Exception e)
	{
	}
	
	return name;
    }
    
};
