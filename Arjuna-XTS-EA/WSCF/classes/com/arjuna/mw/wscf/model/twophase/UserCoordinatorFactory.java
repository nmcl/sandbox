/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UserCoordinatorFactory.java,v 1.4 2003/03/14 14:26:32 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.twophase;

import com.arjuna.mw.wscf.model.twophase.hls.TwoPhaseHLS;
import com.arjuna.mw.wscf.model.twophase.api.UserCoordinator;

import com.arjuna.mwlabs.wscf.protocols.Initializer;

import com.arjuna.mw.wsas.exceptions.SystemException;

import com.arjuna.mwlabs.wscf.utils.ProtocolLocator;

import com.arjuna.mw.wscf.common.CoordinatorXSD;

import com.arjuna.mw.wscf.protocols.*;
import com.arjuna.mw.wscf.utils.*;

import com.arjuna.mw.wscf.exceptions.ProtocolNotRegisteredException;

import java.util.HashMap;

import com.arjuna.mwlabs.wscf.model.twophase.arjunacore.TwoPhaseHLSImple;

/**
 * The factory which returns the UserCoordinator implementation to use.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: UserCoordinatorFactory.java,v 1.4 2003/03/14 14:26:32 nmcl Exp $
 * @since 1.0.
 */

public class UserCoordinatorFactory
{

    /**
     * @exception ProtocolNotRegisteredException Thrown if the default
     * protocol is not available.
     *
     * @return the CoordinatorManager implementation to use. The default
     * coordination protocol is used (two-phase commit) with its
     * associated implementation.
     */

    public static UserCoordinator userCoordinator () throws ProtocolNotRegisteredException, SystemException
    {
	try
	{
	    ProtocolLocator pl = new ProtocolLocator(TwoPhaseHLSImple.className());
	    org.w3c.dom.Document doc = pl.getProtocol();
	    
	    if (doc == null)
	    {
		System.err.println("Failed to create "+TwoPhaseHLSImple.className());
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
     * @return the CoordinatorManager implementation to use.
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
		TwoPhaseHLS coordHLS = (TwoPhaseHLS) _implementations.get(protocolType);
	
		if (coordHLS == null)
		{
		    Object implementation = _protocolManager.getProtocolImplementation(protocol);
		
		    if (implementation instanceof String)
		    {
			Class c = Class.forName((String) implementation);

			coordHLS = (TwoPhaseHLS) c.newInstance();
		    }
		    else
			coordHLS = (TwoPhaseHLS) implementation;

		    _implementations.put(protocolType, coordHLS);
		}
	    
		return coordHLS.userCoordinator();
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

