/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * CoordinationContextExtensionHelper.java
 */

package com.arjuna.wsc.messaging.saaj;

import java.util.Iterator;
import javax.xml.soap.SOAPException;
import javax.xml.soap.Name;
import javax.xml.soap.SOAPEnvelope;
import javax.xml.soap.SOAPElement;
import com.arjuna.wsc.messaging.MarshalException;
import com.arjuna.wsc.messaging.CoordinationContext;
import com.arjuna.wsc.impl.messaging.XMLUtil;
import com.arjuna.wsc.impl.messaging.saaj.AttributedDateTimeHelper;
import com.arjuna.wsc.impl.messaging.saaj.PortReferenceHelper;

public class CoordinationContextExtensionHelper
{
    public void toXML(SOAPEnvelope soapEnvelope, SOAPElement coordinationContextElement, CoordinationContext coordinationContext)
        throws MarshalException
    {
        try
        {
            if (coordinationContext.expires != null)
            {
                SOAPElement expiresElement = coordinationContextElement.addChildElement(XMLUtil.EXPIRES_TAG, XMLUtil.WSU_PREFIX, XMLUtil.WSU_NAMESPACE_URI);

                AttributedDateTimeHelper.toXML(soapEnvelope, expiresElement, coordinationContext.expires);
            }

            SOAPElement identifierElement         = coordinationContextElement.addChildElement(XMLUtil.IDENTIFIER_TAG, XMLUtil.WSU_PREFIX, XMLUtil.WSU_NAMESPACE_URI);
            SOAPElement coordinationTypeElement   = coordinationContextElement.addChildElement(XMLUtil.COORDINATION_TYPE_TAG);
            SOAPElement registationServiceElement = coordinationContextElement.addChildElement(XMLUtil.REGISTRATION_SERVICE_TAG);

            identifierElement.addTextNode(coordinationContext.identifier);
            coordinationTypeElement.addTextNode(coordinationContext.coordinationType);
            PortReferenceHelper.toXML(soapEnvelope, registationServiceElement, coordinationContext.registrationService);
        }
        catch (SOAPException soapException)
        {
            throw new MarshalException("Marshal failure: " + soapException);
        }
    }

    public CoordinationContext fromXML(SOAPEnvelope soapEnvelope, SOAPElement coordinationContextElement)
        throws MarshalException
    {
        CoordinationContext coordinationContext         = new CoordinationContext();
        Iterator            coordinationContextElements = coordinationContextElement.getChildElements();

        fromXML(soapEnvelope, coordinationContextElements, coordinationContext);

        return coordinationContext;
    }

    protected final void fromXML(SOAPEnvelope soapEnvelope, Iterator coordinationContextElements, CoordinationContext coordinationContext)
        throws MarshalException
    {
        SOAPElement coordinationContextElement     = (SOAPElement) coordinationContextElements.next();
        Name        coordinationContextElementName = coordinationContextElement.getElementName();

        if (coordinationContextElementName.getLocalName().equals(XMLUtil.EXPIRES_TAG) && coordinationContextElementName.getURI().equals(XMLUtil.WSU_NAMESPACE_URI))
        {
            coordinationContext.expires = AttributedDateTimeHelper.fromXML(soapEnvelope, coordinationContextElement);
            coordinationContextElement  = (SOAPElement) coordinationContextElements.next();
        }
        coordinationContext.identifier          = coordinationContextElement.getValue();
        coordinationContextElement              = (SOAPElement) coordinationContextElements.next();
        coordinationContext.coordinationType    = coordinationContextElement.getValue();
        coordinationContextElement              = (SOAPElement) coordinationContextElements.next();
        coordinationContext.registrationService = PortReferenceHelper.fromXML(soapEnvelope, coordinationContextElement);
    }
}
