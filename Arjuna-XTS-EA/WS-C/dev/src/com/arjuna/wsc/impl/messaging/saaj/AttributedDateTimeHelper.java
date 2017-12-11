/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * AttributedDateTimeHelper.java
 */

package com.arjuna.wsc.impl.messaging.saaj;

import javax.xml.soap.Name;
import javax.xml.soap.SOAPException;
import javax.xml.soap.SOAPEnvelope;
import javax.xml.soap.SOAPElement;
import com.arjuna.wsc.messaging.MarshalException;
import com.arjuna.wsc.messaging.AttributedDateTime;
import com.arjuna.wsc.impl.messaging.XMLUtil;

public class AttributedDateTimeHelper
{
    public static void toXML(SOAPEnvelope soapEnvelope, SOAPElement attributedDateTimeElement, AttributedDateTime attributedDateTime)
        throws MarshalException
    {
        try
        {
            Name attributeName = soapEnvelope.createName(XMLUtil.VALUETYPE_ATTR, XMLUtil.WSCOOR_PREFIX, XMLUtil.WSCOOR_NAMESPACE_URI);

            if (attributedDateTime.valueType != null)
                attributedDateTimeElement.addAttribute(attributeName, attributedDateTime.valueType);
            attributedDateTimeElement.addTextNode(attributedDateTime.dataTime);
        }
        catch (SOAPException soapException)
        {
            throw new MarshalException("Marshal failure: " + soapException);
        }
    }

    public static AttributedDateTime fromXML(SOAPEnvelope soapEnvelope, SOAPElement attributedDateTimeElement)
        throws MarshalException
    {
        AttributedDateTime attributedDateTime = new AttributedDateTime();

        try
        {
            Name attributeName = soapEnvelope.createName(XMLUtil.VALUETYPE_ATTR, XMLUtil.WSCOOR_PREFIX, XMLUtil.WSCOOR_NAMESPACE_URI);

            attributedDateTime.valueType = attributedDateTimeElement.getAttributeValue(attributeName);
            attributedDateTime.dataTime  = attributedDateTimeElement.getValue();
        }
        catch (SOAPException soapException)
        {
            throw new MarshalException("Marshal failure: " + soapException);
        }

        return attributedDateTime;
    }
}
