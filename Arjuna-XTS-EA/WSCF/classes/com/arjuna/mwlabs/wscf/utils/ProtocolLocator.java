/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ProtocolLocator.java,v 1.6 2003/03/14 14:26:34 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.utils;

import com.arjuna.mwlabs.wscf.exceptions.ProtocolLocationException;

import com.arjuna.mw.wscf.utils.DomUtil;

import org.w3c.dom.*;

import javax.xml.parsers.*;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.IOException;

import java.net.URL;

/**
 * Locates and loads a specified XML definition of a coordination protocol.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ProtocolLocator.java,v 1.6 2003/03/14 14:26:34 nmcl Exp $
 * @since 1.0.
 */

public class ProtocolLocator
{

    public ProtocolLocator (String theType)
    {
	_className = theType.replace(_packageSeparator, _resourceSeparator);
	_xmlName = _className+".xml";
    }

    public final org.w3c.dom.Document getProtocol () throws FileNotFoundException, ProtocolLocationException
    {
	try
	{
	    URL url = ProtocolLocator.class.getResource(_resourceSeparator+_xmlName);

	    if (url == null)
	    {
		throw new FileNotFoundException(_xmlName+" not found.");
	    }
	    else
	    {
		DocumentBuilder builder = DomUtil.getDocumentBuilder();
		org.w3c.dom.Document doc = builder.parse(url.toExternalForm());
	    
		if (doc == null)
		{
		    throw new FileNotFoundException("Failed to create "+_xmlName+" doc!");
		}
		else
		    return doc;
	    }
	}
	catch (FileNotFoundException ex)
	{
	    throw ex;
	}
	catch (IOException ex)
	{
	    throw new FileNotFoundException(ex.toString());
	}
	catch (Exception ex)
	{
	    throw new ProtocolLocationException(ex.toString());
	}
    }

    private String _className;
    private String _xmlName;
    
    private final static char _packageSeparator = '.';
    private final static char _resourceSeparator = '/';
    
}

