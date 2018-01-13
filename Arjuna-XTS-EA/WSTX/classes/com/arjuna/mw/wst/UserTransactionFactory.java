/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UserTransactionFactory.java,v 1.7 2003/03/24 10:57:01 nmcl Exp $
 */

package com.arjuna.mw.wst;

import javax.naming.InitialContext;
import javax.naming.NamingException;

public class UserTransactionFactory
{

    public static final String UserTxName = "UserXMLTransaction";

    public static UserTransaction userTransaction ()
    {
	synchronized (UserTxName)
	{
	    if (_userTransaction == null)
	    {
		try
		{
		    InitialContext initialContext = new InitialContext();

		    Object impl =  initialContext.lookup(UserTxName);

		    if (impl instanceof javax.naming.Reference)
		    {
			javax.naming.Reference ref = (javax.naming.Reference) impl;
			Class c = Class.forName(ref.getClassName());
			
			_userTransaction = (UserTransaction) c.newInstance();
		    }
		    else
			_userTransaction = (UserTransaction) impl;
		}
		catch (Exception ex)
		{
		    ex.printStackTrace();
		}
	    }
	}

	return _userTransaction;
    }

    private static UserTransaction _userTransaction = null;

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
