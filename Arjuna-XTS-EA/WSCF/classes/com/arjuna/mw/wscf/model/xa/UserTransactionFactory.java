/*
 * Copyright (C) 2002,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UserTransactionFactory.java,v 1.4 2003/03/14 14:26:32 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.xa;

import com.arjuna.mw.wscf.model.xa.hls.JTAHLS;

import com.arjuna.mw.wscf.common.*;
import com.arjuna.mw.wscf.protocols.*;
import com.arjuna.mw.wscf.utils.*;

import com.arjuna.mwlabs.wscf.model.jta.arjunajta.JTAHLSImple;

import com.arjuna.mwlabs.wscf.utils.*;
import com.arjuna.mwlabs.wscf.protocols.*;

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

import com.arjuna.mw.wscf.exceptions.*;

import java.util.HashMap;

public class UserTransactionFactory
{

    /**
     * @exception ProtocolNotRegisteredException Thrown if the default
     * protocol is not available.
     *
     * @return the CoordinatorManager implementation to use. The default
     * coordination protocol is used (two-phase commit) with its
     * associated implementation.
     */

    public static javax.transaction.UserTransaction userTransaction () throws ProtocolNotRegisteredException, SystemException
    {
	try
	{
	    ProtocolLocator pl = new ProtocolLocator(JTAHLSImple.className());
	    org.w3c.dom.Document doc = pl.getProtocol();
	    
	    if (doc == null)
	    {
		System.err.println("Failed to create "+JTAHLSImple.className());
	    }
	    else
		return userTransaction(doc);
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();

	    throw new SystemException(ex.toString());
	}

	return null;
    }

    /**
     * Obtain a reference to a coordinator that implements the specified
     * protocol.
     *
     * @param org.w3c.dom.Document protocol The XML definition of the type of
     * coordination protocol required.
     *
     * @exception ProtocolNotRegisteredException Thrown if the requested
     * protocol is not available.
     *
     * @return the CoordinatorManager implementation to use.
     */
    
    /*
     * Have the type specified in XML. More data may be specified, which
     * can be passed to the implementation in the same way ObjectName was.
     */

    public static javax.transaction.UserTransaction userTransaction (org.w3c.dom.Document protocol) throws ProtocolNotRegisteredException, SystemException
    {
	try
	{
	    synchronized (_implementations)
	    {
		org.w3c.dom.Text child = DomUtil.getTextNode(protocol, CoordinatorXSD.coordinatorType);
		String protocolType = child.getNodeValue();
		JTAHLS coordHLS = (JTAHLS) _implementations.get(protocolType);
	
		if (coordHLS == null)
		{
		    Object implementation = _protocolManager.getProtocolImplementation(protocol);
		
		    if (implementation instanceof String)
		    {
			Class c = Class.forName((String) implementation);

			coordHLS = (JTAHLS) c.newInstance();
		    }
		    else
			coordHLS = (JTAHLS) implementation;

		    _implementations.put(protocolType, coordHLS);
		}
	    
		return coordHLS.userTransaction();
	    }
	}
	catch (ProtocolNotRegisteredException ex)
	{
	    throw ex;
	}
	catch (Exception ex)
	{
	    throw new SystemException(ex.toString());
	}
    }

    private static ProtocolManager _protocolManager = ProtocolRegistry.sharedManager();
    private static HashMap         _implementations = new HashMap();
    
}
