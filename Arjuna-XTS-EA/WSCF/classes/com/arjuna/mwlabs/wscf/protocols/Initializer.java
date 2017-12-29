/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Initializer.java,v 1.4 2003/03/14 14:26:33 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.protocols;

import com.arjuna.mw.wscf.common.CoordinatorXSD;

import com.arjuna.mw.wscf.protocols.ProtocolManager;

import com.arjuna.mw.wsas.exceptions.*;

import com.arjuna.mwlabs.wscf.model.as.coordinator.arjunacore.ArjunaCoreHLS;
import com.arjuna.mwlabs.wscf.model.as.coordinator.jta.JTAHLS;

import com.arjuna.mwlabs.wscf.model.twophase.arjunacore.TwoPhaseHLSImple;
import com.arjuna.mwlabs.wscf.model.jta.arjunajta.JTAHLSImple;

import com.arjuna.mwlabs.wscf.utils.*;

import org.w3c.dom.*;

/**
 * Register all of the default coordination protocols with the system.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: Initializer.java,v 1.4 2003/03/14 14:26:33 nmcl Exp $
 * @since 1.0.
 */

public class Initializer
{

    public Initializer (ProtocolManager manager) throws SystemException
    {
	_manager = null;
	
	try
	{
	    com.arjuna.mw.wsas.utils.Configuration.initialise("/com/arjuna/mw/wscf/wscf.xml");
	}
	catch (java.io.FileNotFoundException ex)
	{
	    //	    ex.printStackTrace();
	}
	catch (Exception ex)
	{
	    //	    throw new ExceptionInInitializerError(ex.toString());
	}
    }
    
    private final void addProtocol (String impl) throws SystemException
    {
	try
	{
	    // add in ArjunaCore support

	    ProtocolLocator pl = new ProtocolLocator(impl);
	    org.w3c.dom.Document doc = pl.getProtocol();
	    
	    if (doc == null)
	    {
		throw new SystemException("Failed to create "+impl);
	    }
	    else
	    {
		_manager.addProtocol(doc, impl);
	    }
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	    
	    throw new SystemException(ex.toString());
	}
    }

    private ProtocolManager _manager;
    
}

