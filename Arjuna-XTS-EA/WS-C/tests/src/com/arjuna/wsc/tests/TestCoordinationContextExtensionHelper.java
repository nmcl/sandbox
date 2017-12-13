/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * TestCoordinationContextExtensionHelper.java
 */

package com.arjuna.wsc.tests;

import java.util.Iterator;
import javax.xml.soap.SOAPException;
import javax.xml.soap.Name;
import javax.xml.soap.SOAPEnvelope;
import javax.xml.soap.SOAPElement;
import com.arjuna.wsc.messaging.MarshalException;
import com.arjuna.wsc.messaging.CoordinationContext;
import com.arjuna.wsc.messaging.saaj.CoordinationContextExtensionHelper;

public class TestCoordinationContextExtensionHelper extends CoordinationContextExtensionHelper
{
    private static final String EXTENSION_VALUE_TAG = "ExtensionValue";
    private static final String TEST_PREFIX         = "test";
    private static final String TEST_NAMESPACE_URI  = "http://example.org/test";

    public void toXML(SOAPEnvelope soapEnvelope, SOAPElement coordinationContextElement, CoordinationContext coordinationContext)
        throws MarshalException
    {
        try
        {
            super.toXML(soapEnvelope, coordinationContextElement, coordinationContext);

            SOAPElement extensionValueElement = coordinationContextElement.addChildElement(EXTENSION_VALUE_TAG, TEST_PREFIX, TEST_NAMESPACE_URI);

            if (coordinationContext instanceof TestCoordinationContext)
                extensionValueElement.addTextNode(((TestCoordinationContext) coordinationContext).extensionValue);
            else
                throw new MarshalException("Marshal failure: incorrect class type \"" + coordinationContext.getClass().getName() + "\"");
        }
        catch (SOAPException soapException)
        {
            throw new MarshalException("Marshal failure: " + soapException);
        }
    }

    public CoordinationContext fromXML(SOAPEnvelope soapEnvelope, SOAPElement testCoordinationContextElement)
        throws MarshalException
    {
        TestCoordinationContext testCoordinationContext         = new TestCoordinationContext();
        Iterator                testCoordinationContextElements = testCoordinationContextElement.getChildElements();

        fromXML(soapEnvelope, testCoordinationContextElements, testCoordinationContext);

        SOAPElement element = (SOAPElement) testCoordinationContextElements.next();

        if (element != null)
        {
            Name elementName = element.getElementName();

            if (elementName.getLocalName().equals(EXTENSION_VALUE_TAG) && elementName.getURI().equals(TEST_NAMESPACE_URI))
                testCoordinationContext.extensionValue = element.getValue();
            else
                throw new MarshalException("Marshal failure: incorrect extension element");
        }
        else
            throw new MarshalException("Marshal failure: missing extension element");

        return testCoordinationContext;
    }
}
