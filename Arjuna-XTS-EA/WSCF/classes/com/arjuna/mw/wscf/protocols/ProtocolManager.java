/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ProtocolManager.java,v 1.10 2003/03/14 14:26:33 nmcl Exp $
 */

package com.arjuna.mw.wscf.protocols;

import com.arjuna.mw.wscf.common.CoordinatorXSD;
import com.arjuna.mw.wscf.common.Environment;

import com.arjuna.mwlabs.wscf.protocols.Initializer;

import com.arjuna.mw.wscf.utils.CoordinationXML;

import com.arjuna.mwlabs.wscf.utils.ProtocolLocator;

import org.w3c.dom.*;

import com.arjuna.mwlabs.wscf.utils.DocComparitor;

import com.arjuna.mwlabs.wscf.protocols.ProtocolEntry;

import java.util.HashMap;

import com.arjuna.mw.wscf.exceptions.ProtocolAlreadyRegisteredException;
import com.arjuna.mw.wscf.exceptions.ProtocolNotRegisteredException;
import com.arjuna.mw.wscf.exceptions.UnknownProtocolImplementationException;

import java.security.InvalidParameterException;
import java.lang.ExceptionInInitializerError;

import java.net.URL;

import java.util.Vector;
import java.util.Properties;
import java.util.Enumeration;

/**
 * The ProtocolManager is the way in which protocol implementations
 * may be registered with the system.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ProtocolManager.java,v 1.10 2003/03/14 14:26:33 nmcl Exp $
 * @since 1.0.
 */

public class ProtocolManager
{

    /**
     * Add a specific protocol implementation to the factory.
     * More than one implementation may provide the same protocol. They
     * may differ by coordinator-name only.
     *
     * @param org.w3c.dom.Document protocol The XML definition of the protocol.
     * @param String className The name of the class that implements the
     * specified protocol.
     *
     * @exception ProtocolAlreadyRegisteredException Thrown if the exact same
     * protocol definition has already been registered.
     * @exception InvalidParameterException Thrown if either of the parameters
     * is invalid.
     */

    public void addProtocol (org.w3c.dom.Document protocol, Object className) throws ProtocolAlreadyRegisteredException, InvalidParameterException
    {
	if ((protocol == null) || (className == null))
	    throw new InvalidParameterException();
	
	if (present(protocol))
	    throw new ProtocolAlreadyRegisteredException();
	else
	{
	    ProtocolEntry e = new ProtocolEntry(protocol, className);
	    Vector impls = (Vector) _protocols.get(e.protocolType());

	    if (impls == null)
	    {
		impls = new Vector();
	
		_protocols.put(e.protocolType(), impls);
	    }
	    
	    impls.add(e);
	}
    }

    /**
     * Replace a specific protocol implementation in the factory.
     *
     * @param org.w3c.dom.Document protocol The XML definition of the protocol.
     * @param String className The name of the class that implements the
     * specified protocol.
     *
     * @exception ProtocolNotRegisteredException Thrown if the protocol to be
     * removed has not been registered.
     * @exception InvalidParameterException Thrown if either of the parameters
     * is invalid.
     */

    public void replaceProtocol (org.w3c.dom.Document protocol, Object className) throws ProtocolNotRegisteredException, InvalidParameterException
    {
	synchronized (this)
	{
	    CoordinationXML protocolDef = new CoordinationXML(protocol);
	    Vector impls = (Vector) _protocols.get(protocolDef.protocolType());
	    boolean found = false;
	    
	    if (impls != null)
	    {
		for (int i = 0; (i < impls.size()) && !found; i++)
		{
		    ProtocolEntry entry = (ProtocolEntry) impls.elementAt(i);

		    // currently replaces only on type: should be exact match!

		    if (sameProtocolType(entry, protocolDef))
		    {
			impls.remove(entry);
		    
			try
			{
			    addProtocol(protocol, className);
			}
			catch (Exception ex)
			{
			    throw new InvalidParameterException(ex.toString());
			}
			
			found = true;
		    }
		}
	    }

	    if (!found)
		throw new ProtocolNotRegisteredException();
	}
    }
    
    /**
     * @param org.wc3.dom.Document protocol The XML definition of the coordination
     * protocol.
     *
     * @exception ProtocolNotRegisteredException Thrown if the requested
     * coordination protocol has not been registered.
     * @exception InvalidParameterException Thrown if the parameter is invalid.
     *
     * @return The class that implements the specified coordination protocol.
     */

    public Object getProtocolImplementation (org.w3c.dom.Document protocol) throws ProtocolNotRegisteredException, InvalidParameterException
    {
	/*
	 * Only checks the coordination-type via the hashtable. Now we
	 * need to check the coordination-name.
	 */

	CoordinationXML protocolDef = new CoordinationXML(protocol);
	Vector impls = (Vector) _protocols.get(protocolDef.protocolType());

	if (impls != null)
	{
	    for (int i = 0; i < impls.size(); i++)
	    {
		ProtocolEntry entry = (ProtocolEntry) impls.elementAt(i);
		
		if (sameProtocolType(entry, protocolDef))
		{
		    return entry.implementationClass();
		}
	    }
	}

	throw new ProtocolNotRegisteredException("Could not find protocol "+protocolDef.protocolType());
    }

    /**
     * Remove the specified protocol definition from the factory.
     *
     * @param org.w3c.dom.Document protocol The XML definition of the coordination
     * protocol.
     *
     * @exception ProtocolNotRegisteredException Thrown if the protocol to be
     * removed has not been registered.
     * @exception InvalidParamaterException Thrown if the paramater is invalid
     * (e.g., null).
     *
     * @return the protocol implementation removed.
     */

    public Object removeProtocol (org.w3c.dom.Document protocol) throws ProtocolNotRegisteredException, InvalidParameterException
    {
	CoordinationXML protocolDef = new CoordinationXML(protocol);
	Vector impls = (Vector) _protocols.get(protocolDef.protocolType());
	
	if (impls != null)
	{
	    for (int i = 0; i < impls.size(); i++)
	    {
		ProtocolEntry entry = (ProtocolEntry) impls.elementAt(i);

		if (sameProtocol(entry, protocolDef))
		{
		    impls.remove(entry);
		    
		    return entry.implementationClass();
		}
	    }	    
	}

	throw new ProtocolNotRegisteredException();
    }

    /*
     * Do we need this?
     */
    
    /**
     * Remove the specified coordination protocol implementation class from
     * the factory.
     *
     * @param String className The class to remove.
     *
     * @exception UnknownProtocolImplementationException Thrown if the
     * specified implementation class has not been registered.
     * @exception InvalidParameterException Thrown if the parameter is invalid.
     *
     * @return the protocol implementation removed.
     */

    public Object removeProtocolImplementation (Object className) throws UnknownProtocolImplementationException, InvalidParameterException
    {
	throw new UnknownProtocolImplementationException();
    }

    /**
     * Is the document present exactly as it appears, i.e., even the name
     * is the same?
     *
     * @param org.w3c.dom.Document protocol The XML definition of the coordination
     * protocol.
     *
     * @exception InvalidParameterException Thrown if the parameter is invalid
     * (e.g., null).
     *
     * @return <code>true</code> if there is an exact match for the specified
     * protocol, <code>false</code> otherwise.
     */

    public final boolean present (org.w3c.dom.Document protocol) throws InvalidParameterException
    {
	/*
	 * Only checks the coordination-type via the hashtable. Now we
	 * need to check the coordination-name.
	 */

	boolean found = false;

	CoordinationXML protocolDef = new CoordinationXML(protocol);
	Vector impls = (Vector) _protocols.get(protocolDef.protocolType());
	
	if (impls != null)
	{
	    for (int i = 0; (i < impls.size()) && !found; i++)
	    {
		ProtocolEntry entry = (ProtocolEntry) impls.elementAt(i);

		if (sameProtocolType(entry, protocolDef))
		    found = true;
	    }
	}

	return found;
    }

    /**
     * Is the document present but with a different name?
     *
     * @param org.w3c.dom.Document protocol The XML definition of the coordination
     * protocol.
     *
     * @exception InvalidParameterException Thrown if the parameter is invalid
     * (e.g., null).
     *
     * @return <code>true</code> if there is at match for the specified
     * protocol, <code>false</code> otherwise.
     */

    public final boolean presentWithDifferentName (org.w3c.dom.Document protocol) throws InvalidParameterException
    {
	/*
	 * Check the registered protocols and if the types are the
	 * same but differ by name (essentially who implemented them)
	 * then they are not identical.
	 */

	boolean found = false;
	CoordinationXML protocolDef = new CoordinationXML(protocol);
	Vector impls = (Vector) _protocols.get(protocolDef.protocolType());
	
	if (impls != null)
	{
	    if (protocolDef.protocolName() != null)
	    {
		for (int i = 0; (i < impls.size()) && !found; i++)
		{
		    ProtocolEntry entry = (ProtocolEntry) impls.elementAt(i);

		    if (sameProtocol(entry, protocolDef))
			found = true;
		}
	    }
	    else
		found = true;  // any will do
	}

	return found;
    }

    protected ProtocolManager ()
    {
	initialise();
    }

    private final boolean sameProtocolType (ProtocolEntry pe, CoordinationXML cx)
    {
	if (pe.protocolType() != null)
	{
	    if (cx.protocolType() != null)
	    {
		return cx.protocolType().equals(pe.protocolType());
	    }
	}
	
	return false;
    }

    private final boolean sameProtocol (ProtocolEntry pe, CoordinationXML cx)
    {
	if (sameProtocolType(pe, cx))
	{
	    if (pe.protocolName() != null)
	    {
		if (cx.protocolName() != null)
		    return cx.protocolName().equals(pe.protocolName());
	    }
	}
	
	return false;
    }
	    
    private synchronized final void initialise ()
    {
	if (_initialised)
	    return;
	else
	    _initialised = true;

	// TODO sort out configuration!

	try
	{
	    Initializer init = new Initializer(this);
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}

	Properties props = System.getProperties();
	Enumeration names = props.propertyNames();

	while (names.hasMoreElements())
	{
	    String name = (String) names.nextElement();

	    if (name.startsWith(Environment.PROTOCOL_IMPLEMENTATION))
	    {
		String className = props.getProperty(name);
		
		try
		{
		    ProtocolLocator pl = new ProtocolLocator(className);
		    org.w3c.dom.Document doc = pl.getProtocol();
	    
		    if (doc == null)
		    {
			// TODO

			throw new ExceptionInInitializerError("Failed to "+className+" doc!");
		    }
		    else
		    {
			addProtocol(doc, className);
		    }
		}
		catch (Exception ex)
		{
		    ex.printStackTrace();
	    
		    throw new ExceptionInInitializerError(ex.toString());
		}
	    }
	}
    }
    
    private HashMap _protocols = new HashMap();
    private boolean _initialised = false;
    
}

