/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DomUtil.java,v 1.2 2003/03/04 12:55:56 nmcl Exp $
 */

package com.arjuna.mw.wscf.utils;

import com.arjuna.mw.wscf.common.CoordinatorXSD;

import org.w3c.dom.*;

import javax.xml.parsers.*;

// TODO put back

//import org.apache.xml.serialize.DOMWriterImpl;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: DomUtil.java,v 1.2 2003/03/04 12:55:56 nmcl Exp $
 * @since 1.0.
 */

public class DomUtil
{

    public static final org.w3c.dom.Text getTextNode (org.w3c.dom.Node element, String name)
    {
	org.w3c.dom.Node child = getNode(element, name);
	
	if (child != null)
	    return (org.w3c.dom.Text) child.getFirstChild();
	else
	    return null;
    }

    public static final org.w3c.dom.Node getNode (org.w3c.dom.Node element, String name)
    {
	org.w3c.dom.Node child = null;
	
	if (element != null)
	{
	    org.w3c.dom.NodeList children = element.getChildNodes();
	    
	    for (int i = 0; i < children.getLength(); i++)
	    {
		org.w3c.dom.Node item = children.item(i);
		
		if (item.getNodeName().equals(name))
		{
		    child = item;

		    break;
		}
		else
		{
		    if (item.hasChildNodes())
		    {
			child = getNode(item, name);
			
			if (child != null)
			    break;
		    }
		}
	    }
	}
	
	return child;
    }

    public static final String nodeAsString (org.w3c.dom.Node node)
    {
	// TODO put back

	//	DOMWriterImpl domWriter = new DOMWriterImpl(false);

	//	return domWriter.writeToString(node);

	return node.toString();
    }

    public static final DocumentBuilder getDocumentBuilder ()
    {
	try
	{
	    DocumentBuilderFactory factory = DocumentBuilderFactory.newInstance();
	    
	    return factory.newDocumentBuilder();
	}
	catch (Exception ex)
	{
	    ex.printStackTrace();
	}
	
	return null;
    }

}
