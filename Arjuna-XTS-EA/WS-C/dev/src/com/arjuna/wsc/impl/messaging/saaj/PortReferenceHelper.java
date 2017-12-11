/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * PortReferenceHelper.java
 */

package com.arjuna.wsc.impl.messaging.saaj;

import java.util.Iterator;
import javax.xml.soap.*;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.impl.messaging.XMLUtil;

public class PortReferenceHelper
{
    public static void toXML(SOAPEnvelope soapEnvelope, SOAPElement portReferenceElement, PortReference portReference)
        throws MarshalException
    {
        try
        {
            SOAPElement addressElement = portReferenceElement.addChildElement(XMLUtil.ADDRESS_TAG, XMLUtil.WSU_PREFIX, XMLUtil.WSU_NAMESPACE_URI);
            addressElement.addTextNode(portReference.getAddress());

            Iterator extensions = portReference.getExtensions();
            while (extensions.hasNext())
            {
                PortReference.Extension extension = (PortReference.Extension) extensions.next();

                SOAPElement extensionElement = portReferenceElement.addChildElement(extension.getTag(), XMLUtil.WSARJCOOR_PREFIX, XMLUtil.WSARJCOOR_NAMESPACE_URI);
                extensionElement.addTextNode(extension.getValue());
            }
        }
        catch (SOAPException soapException)
        {
            throw new MarshalException("Marshal failure: " + soapException);
        }
    }

    public static PortReference fromXML(SOAPEnvelope soapEnvelope, SOAPElement portReferenceElement)
        throws MarshalException
    {
        PortReference portReference = new PortReference();

        Iterator    elements       = portReferenceElement.getChildElements();
        SOAPElement addressElement = (SOAPElement) elements.next();

        portReference.setAddress(addressElement.getValue());

        while (elements.hasNext())
        {
            SOAPElement extensionElement = (SOAPElement) elements.next();

            portReference.addExtension(extensionElement.getElementName().getLocalName(), extensionElement.getValue());
        }

        return portReference;
    }
}
