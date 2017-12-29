/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ProtocolEntry.java,v 1.4 2003/01/17 15:39:34 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.protocols;

import com.arjuna.mw.wscf.common.CoordinatorXSD;

import com.arjuna.mw.wscf.utils.DomUtil;

import org.w3c.dom.*;

/**
 * When an XML coordination protocol definition is loaded, an instance of this
 * class is created to manage it within the registry.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ProtocolEntry.java,v 1.4 2003/01/17 15:39:34 nmcl Exp $
 * @since 1.0.
 */

public class ProtocolEntry
{

    public ProtocolEntry (org.w3c.dom.Document doc)
    {
	_doc = doc;
	_implementation = null;

	org.w3c.dom.Element rootElement = _doc.getDocumentElement();
		    
	_protocolType = DomUtil.getTextNode(rootElement, CoordinatorXSD.coordinatorType).getNodeValue();

	/*
	 * Optional part of schema.
	 */

	org.w3c.dom.Text name = DomUtil.getTextNode(rootElement, CoordinatorXSD.coordinatorName);
	
	if (name != null)
	    _protocolName = name.getNodeValue();
	else
	    _protocolName = null;
    }

    public ProtocolEntry (org.w3c.dom.Document doc, Object implementation)
    {
	_doc = doc;
	_implementation = implementation;

	org.w3c.dom.Element rootElement = _doc.getDocumentElement();
		    
	_protocolType = DomUtil.getTextNode(rootElement, CoordinatorXSD.coordinatorType).getNodeValue();

	org.w3c.dom.Text name = DomUtil.getTextNode(rootElement, CoordinatorXSD.coordinatorName);
	
	if (name != null)
	    _protocolName = name.getNodeValue();
	else
	    _protocolName = null;	
    }
    
    public final org.w3c.dom.Document document ()
    {
	return _doc;
    }
    
    public final Object implementationClass ()
    {
	return _implementation;
    }

    public final String protocolType ()
    {
	return _protocolType;
    }
    
    public final String protocolName ()
    {
	return _protocolName;
    }
    
    public boolean equals (Object obj)
    {
	if (obj == null)
	    return false;
	else
	{
	    if (obj == this)
		return true;
	    else
	    {
		if (obj instanceof ProtocolEntry)
		{
		    ProtocolEntry entry = (ProtocolEntry) obj;
		    org.w3c.dom.Element rootElement = entry.document().getDocumentElement();
		    String ptype = DomUtil.getTextNode(rootElement, CoordinatorXSD.coordinatorType).getNodeValue();

		    return _protocolType.equals(ptype);
		}
	    }
	    
	    return false;
	}
    }

    public int hashCode ()
    {
	return _protocolType.hashCode();
    }

    public String toString ()
    {
	return DomUtil.nodeAsString(_doc);
    }
    
    private org.w3c.dom.Document _doc;
    private Object               _implementation;
    private String               _protocolType;
    private String               _protocolName;
    
}

