/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * CoordinationContextHelper.java
 */

package com.arjuna.wsc.messaging.saaj;

import java.util.Map;
import java.util.HashMap;
import java.util.Iterator;
import javax.xml.soap.*;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.impl.messaging.XMLUtil;

public class CoordinationContextHelper
{
    public static void toXML(SOAPEnvelope soapEnvelope, SOAPElement coordinationContextElement, CoordinationContext coordinationContext)
        throws MarshalException
    {
        CoordinationContextExtensionHelper coordinationContextExtensionHelper = (CoordinationContextExtensionHelper) _coordinationContextExtensionHelperMap.get(coordinationContext.coordinationType);

        if (coordinationContextExtensionHelper != null)
            coordinationContextExtensionHelper.toXML(soapEnvelope, coordinationContextElement, coordinationContext);
        else
            _defaultCoordinationContextExtensionHelper.toXML(soapEnvelope, coordinationContextElement, coordinationContext);
    }

    public static CoordinationContext fromXML(SOAPEnvelope soapEnvelope, SOAPElement coordinationContextElement)
        throws MarshalException
    {
        Iterator                           coordinationContextElements        = coordinationContextElement.getChildElements();
        CoordinationContextExtensionHelper coordinationContextExtensionHelper = null;

        while (coordinationContextElements.hasNext() && (coordinationContextExtensionHelper == null))
        {
            SOAPElement element     = (SOAPElement) coordinationContextElements.next();
            Name        elementName = element.getElementName();

            if (elementName.getLocalName().equals(XMLUtil.COORDINATION_TYPE_TAG) && elementName.getURI().equals(XMLUtil.WSCOOR_NAMESPACE_URI))
                coordinationContextExtensionHelper = (CoordinationContextExtensionHelper) _coordinationContextExtensionHelperMap.get(element.getValue());
        }

        if (coordinationContextExtensionHelper != null)
            return coordinationContextExtensionHelper.fromXML(soapEnvelope, coordinationContextElement);
        else
            return _defaultCoordinationContextExtensionHelper.fromXML(soapEnvelope, coordinationContextElement);
    }

    public static void addCoordinationContextExtensionHelper(String coordinationType, CoordinationContextExtensionHelper coordinationContextExtensionHelper)
        throws MarshalException
    {
        _coordinationContextExtensionHelperMap.put(coordinationType, coordinationContextExtensionHelper);
    }

    public static void removeCoordinationContextExtensionHelper(String coordinationType)
    {
        _coordinationContextExtensionHelperMap.remove(coordinationType);
    }

    private static CoordinationContextExtensionHelper _defaultCoordinationContextExtensionHelper = new CoordinationContextExtensionHelper();
    private static Map                                _coordinationContextExtensionHelperMap     = new HashMap();
}
