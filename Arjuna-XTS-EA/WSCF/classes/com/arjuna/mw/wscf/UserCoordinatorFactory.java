/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UserCoordinatorFactory.java,v 1.9 2003/03/24 10:45:50 nmcl Exp $
 */

package com.arjuna.mw.wscf;

import com.arjuna.mw.wscf.common.CoordinatorXSD;

import com.arjuna.mwlabs.wscf.utils.ProtocolLocator;

import com.arjuna.mwlabs.wscf.UserCoordinatorImple;

import com.arjuna.mw.wscf.protocols.*;
import com.arjuna.mw.wscf.utils.*;

import com.arjuna.mw.wscf.exceptions.ProtocolNotRegisteredException;

import com.arjuna.mw.wsas.exceptions.SystemException;

import java.util.HashMap;

import java.net.URL;

import java.io.FileNotFoundException;

/**
 * The factory which returns the UserCoordinator implementation to use.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: UserCoordinatorFactory.java,v 1.9 2003/03/24 10:45:50 nmcl Exp $
 * @since 1.0.
 */

public class UserCoordinatorFactory
{

    /**
     * @exception ProtocolNotRegisteredException Thrown if the default
     * protocol is not available.
     *
     * @return the UserCoordinator implementation to use. The default
     * coordination protocol is used (two-phase commit) with its
     * associated implementation.
     */

    public static UserCoordinator userCoordinator () throws ProtocolNotRegisteredException, SystemException
    {
	try
	{
	    ProtocolLocator pl = new ProtocolLocator(com.arjuna.mwlabs.wscf.model.as.coordinator.arjunacore.ArjunaCoreHLS.class.getName());
	    org.w3c.dom.Document doc = pl.getProtocol();
	    
	    if (doc == null)
	    {
		System.err.println("Failed to create arjunacore doc!");
	    }
	    else
		return userCoordinator(doc);
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
     * @return the UserCoordinator implementation to use.
     */
    
    /*
     * Have the type specified in XML. More data may be specified, which
     * can be passed to the implementation in the same way ObjectName was.
     */

    public static UserCoordinator userCoordinator (org.w3c.dom.Document protocol) throws ProtocolNotRegisteredException, SystemException
    {
	try
	{
	    synchronized (_implementations)
	    {
		org.w3c.dom.Text child = DomUtil.getTextNode(protocol, CoordinatorXSD.coordinatorType);
		String protocolType = child.getNodeValue();
		UserCoordinatorImple coord = (UserCoordinatorImple) _implementations.get(protocolType);
	
		if (coord == null)
		{
		    Object implementation = _protocolManager.getProtocolImplementation(protocol);
		
		    coord = new UserCoordinatorImple(implementation);

		    _implementations.put(protocolType, coord);
		}

		return coord;
	    }
	}
	catch (ProtocolNotRegisteredException ex)
	{
	    throw ex;
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	    
	    throw new SystemException(ex.toString());
	}
    }

    private static ProtocolManager _protocolManager = ProtocolRegistry.sharedManager();
    private static HashMap         _implementations = new HashMap();

    static
    {
	try
	{
	    com.arjuna.mw.wsas.utils.Configuration.initialise("/config/wscf.xml");
	}
	catch (FileNotFoundException ex)
	{
	}
	catch (Exception ex)
	{
	    throw new ExceptionInInitializerError(ex.toString());
	}
    }
    
}

