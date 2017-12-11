/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ErrorCodeHelper.java
 */

package com.arjuna.wsc.impl.messaging.saaj;

import javax.xml.namespace.QName;
import javax.xml.soap.SOAPException;
import javax.xml.soap.SOAPEnvelope;
import javax.xml.soap.SOAPElement;
import com.arjuna.wsc.messaging.MarshalException;
import com.arjuna.wsc.messaging.ErrorCode;
import com.arjuna.wsc.impl.messaging.XMLUtil;

public class ErrorCodeHelper
{
    public static void toXML(SOAPEnvelope soapEnvelope, SOAPElement errorCodeElement, ErrorCode errorCode)
        throws MarshalException
    {
        try
        {
/*
 * Correct implement is, but due to a bug in Axis-1.0, ....
 *
 * SOAPElement e = errorCodeElement.addNamespaceDeclaration("error", errorCode.namespace);
 * errorCodeElement.addTextNode("error:" + errorCode.name);
 */

            if (XMLUtil.WSCOOR_NAMESPACE_URI.equals(errorCode.namespace))
                errorCodeElement.addTextNode(XMLUtil.WSCOOR_PREFIX + ":" + errorCode.name);
            else if (XMLUtil.WSARJCOOR_NAMESPACE_URI.equals(errorCode.namespace))
                errorCodeElement.addTextNode(XMLUtil.WSARJCOOR_PREFIX + ":" + errorCode.name);
            else
                throw new MarshalException("Unknown error namespace: " + errorCode.namespace);
        }
        catch (SOAPException soapException)
        {
            throw new MarshalException("Marshal failure: \"" + soapException + "\"");
        }
    }

    public static ErrorCode fromXML(SOAPEnvelope soapEnvelope, SOAPElement errorCodeElement)
        throws MarshalException
    {
        ErrorCode errorCode     = new ErrorCode();
        String    errorCodeText = errorCodeElement.getValue();
        int       endOfPrefix   = errorCodeText.indexOf(':');

        if (endOfPrefix != -1)
        {
/*
 * Correct implement is, but due to a bug in Axis-1.0, ....
 *
 * errorCode.namespace = errorCodeElement.getNamespaceURI();
 */
            String prefix = errorCodeText.substring(0, endOfPrefix);

            if (XMLUtil.WSCOOR_PREFIX.equals(prefix))
                errorCode.namespace = XMLUtil.WSCOOR_NAMESPACE_URI;
            else if (XMLUtil.WSARJCOOR_PREFIX.equals(prefix))
                errorCode.namespace = XMLUtil.WSARJCOOR_NAMESPACE_URI;
            else
                throw new MarshalException("Unknown error prefix: \"" + prefix + "\"");

            errorCode.name = errorCodeText.substring(endOfPrefix + 1);
        }
        else
        {
            errorCode.namespace = errorCodeElement.getElementName().getURI();
            errorCode.name      = errorCodeText;
        }

        return errorCode;
    }
}
