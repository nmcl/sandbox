/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ContextFactoryMapperImple.java,v 1.10 2003/04/04 15:00:06 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc;

import com.arjuna.mwlabs.wsc.model.twophase.ContextFactoryImple;

import com.arjuna.wsc.ContextFactoryMapper;
import com.arjuna.wsc.ContextFactory;

import com.arjuna.mw.wscf.protocols.*;

import com.arjuna.mw.wscf.common.CoordinatorXSD;

import com.arjuna.mw.wscf.utils.CoordinationXML;

import javax.naming.InitialContext;

import javax.xml.parsers.*;

import org.w3c.dom.*;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ContextFactoryMapperImple.java,v 1.10 2003/04/04 15:00:06 nmcl Exp $
 * @since 1.0.
 */

public class ContextFactoryMapperImple implements ContextFactoryMapper, java.io.Serializable
{

    public ContextFactoryMapperImple (String serviceURI) throws Exception
    {
	_protocols = ProtocolRegistry.createManager();

	InitialContext initialContext = new InitialContext();
	
	initialContext.bind(localName(), this);

	_serviceURI = serviceURI;
    }

    public ContextFactoryMapperImple (String jndiName, String serviceURI) throws Exception
    {
	_protocols = ProtocolRegistry.createManager();
	
	InitialContext initialContext = new InitialContext();
	
	initialContext.bind(jndiName, this);
	initialContext.bind(localName(), this);
	
	_serviceURI = serviceURI;
    }

    public String getServiceURI ()
    {
	return _serviceURI;
    }
    
    public void addContextFactory (String coordinationTypeURI, ContextFactory contextFactory)
    {
	try
	{
	    _protocols.addProtocol(convert(coordinationTypeURI), contextFactory);

	    contextFactory.install(_serviceURI, coordinationTypeURI);
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
    }
    
    public void removeContextFactory (String coordinationTypeURI)
    {
	try
	{
	    ContextFactory contextFactory = (ContextFactory) _protocols.removeProtocol(convert(coordinationTypeURI));

	    contextFactory.uninstall(_serviceURI, coordinationTypeURI);
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
    }
    
    /*
     * The factory implementation must initialise the activity service
     * with the correct HLS.
     */

    public ContextFactory getContextFactory (String coordinationTypeURI)
    {
	try
	{
	    org.w3c.dom.Document doc = convert(coordinationTypeURI);
	    Object type = _protocols.getProtocolImplementation(convert(coordinationTypeURI));
	    
	    if (type instanceof String)
	    {
		Class c = Class.forName((String) type);
		
		ContextFactory factory = (ContextFactory) c.newInstance();

		_protocols.replaceProtocol(doc, factory);
		
		return factory;
	    }
	    else
	    {
		return (ContextFactory) type;
	    }
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}

	return null;
    }

    public void setSubordinateContextFactoryMapper (ContextFactoryMapper subordinateContextFactoryMapper)
    {
    }

    public ContextFactoryMapper getSubordinateContextFactoryMapper ()
    {
	return null;
    }

    public void setDefaultContextFactory (ContextFactory defaultContextFactory)
    {
    }

    public ContextFactory getDefaultContextFactory ()
    {
	return null;
    }
    
    private org.w3c.dom.Document convert (String coordinationTypeURI) throws Exception
    {
	DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
	DocumentBuilder builder = factory.newDocumentBuilder();
	org.w3c.dom.Document doc = builder.newDocument();
	org.w3c.dom.Element rootElement = doc.createElement(_wscRootElement);
	org.w3c.dom.Element elem = doc.createElement(CoordinatorXSD.coordinatorType);

	elem.appendChild(doc.createTextNode(coordinationTypeURI));
	
	rootElement.appendChild(elem);
	
	doc.appendChild(rootElement);
	
	return doc;
    }

    private void writeObject(java.io.ObjectOutputStream objectOutputStream )
        throws java.io.IOException
    {
    }

    private void readObject(java.io.ObjectInputStream objectInputStream)
        throws java.io.IOException, ClassNotFoundException
    {
    }

    public static final String localName ()
    {
	//	return "com/arjuna/mwlabs/wsc/contextfactorymapper";
	return "comarjunamwlabswsccontextfactorymapper";
    }
    
    private ProtocolManager _protocols;
    private String          _serviceURI;
    
    private static final String _wscRootElement = "WS-C";
    
}

