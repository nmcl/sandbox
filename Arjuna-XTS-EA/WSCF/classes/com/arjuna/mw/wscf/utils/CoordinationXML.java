/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CoordinationXML.java,v 1.5 2003/01/22 11:42:32 nmcl Exp $
 */

package com.arjuna.mw.wscf.utils;

import com.arjuna.mw.wscf.common.CoordinatorXSD;

import org.w3c.dom.*;

/**
 * This class is used to parse the coordination protocol XML definition
 * and return specific information contained within.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: CoordinationXML.java,v 1.5 2003/01/22 11:42:32 nmcl Exp $
 * @since 1.0.
 */

public class CoordinationXML
{

    public CoordinationXML (org.w3c.dom.Document protocol)
    {
	_protocol = protocol;
	_rootElement = _protocol.getDocumentElement();

	org.w3c.dom.Text name = DomUtil.getTextNode(_rootElement, CoordinatorXSD.coordinatorType);

	if (name != null)
	    _protocolType = name.getNodeValue();
	else
	    _protocolType = null;

	/*
	 * Optional part of schema.
	 */

	name = DomUtil.getTextNode(_rootElement, CoordinatorXSD.coordinatorName);
	
	if (name != null)
	    _protocolName = name.getNodeValue();
	else
	    _protocolName = null;
    }

    /**
     * @return the type of the protocol.
     */

    public final String protocolType ()
    {
	return _protocolType;
    }

    /**
     * @return the name of the protocol, or null if it was not defined in the
     * protocol definition.
     */

    public final String protocolName ()
    {
	return _protocolName;
    }

    public boolean equals (Object obj)
    {
	if (obj instanceof CoordinationXML)
	{
	    CoordinationXML p = (CoordinationXML) obj;
	    
	    if (_protocolType != null)
	    {
		if (p.protocolType() != null)
		{
		    if (_protocolType.equals(p.protocolType()))
		    {
			if (_protocolName != null)
			{
			    if (p.protocolName() != null)
				return _protocolName.equals(p.protocolName());
			}
		    }
		}
	    }
	}
	
	return false;
    }
    
    private org.w3c.dom.Document _protocol;
    private org.w3c.dom.Element  _rootElement;
    private String               _protocolType;
    private String               _protocolName;
    
}
