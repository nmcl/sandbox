/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TransactionManagerFactory.java,v 1.7 2003/03/24 10:57:01 nmcl Exp $
 */

package com.arjuna.mw.wst;

import javax.naming.InitialContext;
import javax.naming.NamingException;

public class TransactionManagerFactory
{

    public static final String TxManagerName = "XMLTransactionManager";

    public static TransactionManager transactionManager ()
    {
	synchronized (TxManagerName)
	{
	    if (_transactionManager == null)
	    {
		try
		{
		    InitialContext initialContext = new InitialContext();

		    Object impl =  initialContext.lookup(TxManagerName);

		    if (impl instanceof javax.naming.Reference)
		    {
			javax.naming.Reference ref = (javax.naming.Reference) impl;
			Class c = Class.forName(ref.getClassName());
			
			_transactionManager = (TransactionManager) c.newInstance();
		    }
		    else
			_transactionManager = (TransactionManager) impl;
		}
		catch (Exception ex)
		{
		    ex.printStackTrace();
		}
	    }
	}

	return _transactionManager;
    }

    private static TransactionManager _transactionManager;

    static
    {
	boolean rebind = false;
	
	try
	{
	    com.arjuna.mwlabs.wst.util.Binder.bind();
	}
	catch (NamingException ex)
	{
	    rebind = true;
	}
	catch (Exception ex)
	{
	}

	if (rebind)
	{
	    try
	    {
		com.arjuna.mwlabs.wst.util.Binder.rebind();
	    }
	    catch (Exception ex)
	    {
	    }
	}

	try
	{
	    com.arjuna.mw.wsas.utils.Configuration.initialise("/config/wstx.xml");
	}
	catch (java.io.FileNotFoundException ex)
	{
	}
	catch (Exception ex)
	{
	    throw new ExceptionInInitializerError(ex.toString());
	}
    }
    
}
