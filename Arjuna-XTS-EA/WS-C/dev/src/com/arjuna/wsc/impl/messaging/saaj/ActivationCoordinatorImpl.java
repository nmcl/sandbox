/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ActivationCoordinatorImpl.java
 */

package com.arjuna.wsc.impl.messaging.saaj;

import java.util.Iterator;
import java.util.List;
import java.util.LinkedList;
import javax.xml.soap.*;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.messaging.saaj.CoordinationContextHelper;
import com.arjuna.wsc.impl.messaging.XMLUtil;
import com.arjuna.wsc.impl.messaging.saaj.MessageListener;

public class ActivationCoordinatorImpl implements MessageListener, ActivationCoordinator, java.io.Serializable
{
    public ActivationCoordinatorImpl(String serviceURI)
        throws SOAPException
    {
        _connectionFactory = SOAPConnectionFactory.newInstance();
        _serviceURI        = serviceURI;
    }

    public void onMessage(SOAPEnvelope envelope)
    {
        try
        {
            synchronized(_requestList)
            {
                _requestList.add(envelope);
                _requestList.notifyAll();
            }
        }
        catch (Exception exception)
        {
            System.err.println("ActivationCoordinatorImpl: " + exception);
        }
        catch (Error error)
        {
            System.err.println("ActivationCoordinatorImpl: " + error);
        }
    }

    public SOAPEnvelope receiveRequest()
    {
        SOAPEnvelope request = null;

        try
        {
            synchronized(_requestList)
            {
                while (request == null)
                {
                    if (_requestList.isEmpty())
                       _requestList.wait();
                    else
                        request = (SOAPEnvelope) _requestList.remove(0);
                }
            }
        }
        catch (Exception exception)
        {
            System.err.println("ActivationCoordinatorImpl: " + exception);
        }
        catch (Error error)
        {
            System.err.println("ActivationCoordinatorImpl: " + error);
        }

        return request;
    }

    public String getServiceURI()
    {
        return _serviceURI;
    }

    public CreateCoordinationContextMessage recvCreateCoordinationContext()
        throws MarshalException
    {
        try
        {
            CreateCoordinationContextMessage createCoordinationContextMessage = new CreateCoordinationContextMessage();

            SOAPEnvelope envelope = receiveRequest();
            SOAPBody     body     = envelope.getBody();

            Iterator        bodyElements    = body.getChildElements();
            SOAPBodyElement bodyElement     = (SOAPBodyElement) bodyElements.next();

            if (bodyElements.hasNext())
                throw new MarshalException("Unexpected extra body element");

            Name bodyElementName = bodyElement.getElementName();

            if (! (bodyElementName.getLocalName().equals(XMLUtil.CREATE_COORDINATION_CONTEXT_TAG) && bodyElementName.getURI().equals(XMLUtil.WSCOOR_NAMESPACE_URI)))
                throw new MarshalException("Not expecting Tag: {" + bodyElementName.getURI() + "," + bodyElementName.getLocalName() + "}");

            Iterator    parameters    = bodyElement.getChildElements();
            SOAPElement parameter     = null;
            Name        parameterName = null;

            if (parameters.hasNext())
            {
                parameter     = (SOAPElement) parameters.next();
                parameterName = parameter.getElementName();
            }

            if ((parameter != null) && parameterName.getLocalName().equals(XMLUtil.ACTIVATION_SERVICE_TAG) && parameterName.getURI().equals(XMLUtil.WSCOOR_NAMESPACE_URI))
            {
                createCoordinationContextMessage.activationService = PortReferenceHelper.fromXML(envelope,parameter);

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();
                }
                else
                    parameter = null;
            }
            else
                throw new MarshalException("Not expecting Tag: {" + parameterName.getURI() + "," + parameterName.getLocalName() + "}");

            if ((parameter != null) && parameterName.getLocalName().equals(XMLUtil.EXPIRES_TAG) && parameterName.getURI().equals(XMLUtil.WSCOOR_NAMESPACE_URI))
            {
                createCoordinationContextMessage.expires = AttributedDateTimeHelper.fromXML(envelope, parameter);

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();
                }
                else
                    parameter = null;
            }
            else
                createCoordinationContextMessage.expires = null;

            if ((parameter != null) && parameterName.getLocalName().equals(XMLUtil.CURRENT_CONTEXT_TAG) && parameterName.getURI().equals(XMLUtil.WSCOOR_NAMESPACE_URI))
            {
                createCoordinationContextMessage.currentContext = CoordinationContextHelper.fromXML(envelope, parameter);

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();
                }
                else
                    parameter = null;
            }
            else
                createCoordinationContextMessage.currentContext = null;

            if ((parameter != null) && parameterName.getLocalName().equals(XMLUtil.REQUESTER_REFERENCE_TAG) && parameterName.getURI().equals(XMLUtil.WSCOOR_NAMESPACE_URI))
            {
                createCoordinationContextMessage.requesterReference = PortReferenceHelper.fromXML(envelope, parameter);

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();
                }
                else
                    parameter = null;
            }
            else
                throw new MarshalException("Not expecting Tag: {" + parameterName.getURI() + "," + parameterName.getLocalName() + "}");

            if ((parameter != null) && parameterName.getLocalName().equals(XMLUtil.COORDINATION_TYPE_TAG) && parameterName.getURI().equals(XMLUtil.WSCOOR_NAMESPACE_URI))
            {
                createCoordinationContextMessage.coordinationType = parameter.getValue();

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();
                }
                else
                    parameter = null;
            }
            else
                throw new MarshalException("Not expecting Tag: {" + parameterName.getURI() + "," + parameterName.getLocalName() + "}");

            if ((parameter != null) && parameterName.getLocalName().equals(XMLUtil.DIALOG_IDENTIFIER_TAG) && parameterName.getURI().equals(XMLUtil.WSARJCOOR_NAMESPACE_URI))
            {
                createCoordinationContextMessage.dialogIdentifier = parameter.getValue();

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();
                }
                else
                    parameter = null;
            }
            else
                createCoordinationContextMessage.dialogIdentifier = null;

            if (parameter != null)
                throw new MarshalException("Not expecting extra Tag: {" + parameterName.getURI() + "," + parameterName.getLocalName() + "}");

            return createCoordinationContextMessage;
        }
        catch (SOAPException soapException)
        {
            throw new MarshalException("Marshal failure: " + soapException);
        }
    }

    public void sendCreateCoordinationContextResponse(CreateCoordinationContextResponseMessage createCoordinationContextResponseMessage)
        throws MarshalException
    {
        try
        {
            MessageFactory messageFactory = MessageFactory.newInstance();
            SOAPMessage    message        = messageFactory.createMessage();
            SOAPPart       part           = message.getSOAPPart();
            SOAPEnvelope   envelope       = part.getEnvelope();
            SOAPBody       body           = envelope.getBody();

            envelope.getHeader().detachNode();

            Name            createCoordinationContextResponceName        = envelope.createName(XMLUtil.CREATE_COORDINATION_CONTEXT_RESPONSE_TAG, null, XMLUtil.WSCOOR_NAMESPACE_URI);
            SOAPBodyElement createCoordinationContextResponceBodyElement = body.addBodyElement(createCoordinationContextResponceName);
            SOAPElement     requestReferenceElement                      = createCoordinationContextResponceBodyElement.addChildElement(XMLUtil.REQUESTER_REFERENCE_TAG);

            PortReferenceHelper.toXML(envelope, requestReferenceElement, createCoordinationContextResponseMessage.requesterReference);
            if (createCoordinationContextResponseMessage.coordinationContext != null)
            {
                SOAPElement coordinationContextElement = createCoordinationContextResponceBodyElement.addChildElement(XMLUtil.COORDINATION_CONTEXT_TAG);

                CoordinationContextHelper.toXML(envelope, coordinationContextElement, createCoordinationContextResponseMessage.coordinationContext);
            }
            if (createCoordinationContextResponseMessage.dialogIdentifier != null)
            {
                SOAPElement dialogIdentifierElement = createCoordinationContextResponceBodyElement.addChildElement(XMLUtil.DIALOG_IDENTIFIER_TAG, XMLUtil.WSARJCOOR_PREFIX, XMLUtil.WSARJCOOR_NAMESPACE_URI);

                dialogIdentifierElement.addTextNode(createCoordinationContextResponseMessage.dialogIdentifier);
            }

            SOAPConnection connection = _connectionFactory.createConnection();
            SOAPMessage    responce   = connection.call(message, createCoordinationContextResponseMessage.requesterReference.getAddress());

            connection.close();
        }
        catch (SOAPException soapException)
        {
            throw new MarshalException("Marshal failure: " + soapException);
        }
    }

    public void sendError(ErrorMessage errorMessage)
        throws MarshalException
    {
        try
        {
            MessageFactory messageFactory = MessageFactory.newInstance();
            SOAPMessage    message        = messageFactory.createMessage();
            SOAPPart       part           = message.getSOAPPart();
            SOAPEnvelope   envelope       = part.getEnvelope();
            SOAPBody       body           = envelope.getBody();

            envelope.getHeader().detachNode();

            Name            errorName                    = envelope.createName(XMLUtil.ERROR_TAG, null, XMLUtil.WSCOOR_NAMESPACE_URI);
            SOAPBodyElement errorBodyElement             = body.addBodyElement(errorName);
            SOAPElement     targetProtocolServiceElement = errorBodyElement.addChildElement(XMLUtil.TARGET_PROTOCOL_SERVICE_TAG);
            SOAPElement     errorCodeElement             = errorBodyElement.addChildElement(XMLUtil.ERROR_CODE_TAG);

            PortReferenceHelper.toXML(envelope, targetProtocolServiceElement, errorMessage.targetProtocolService);
            ErrorCodeHelper.toXML(envelope, errorCodeElement, errorMessage.errorCode);
            if (errorMessage.dialogIdentifier != null)
            {
                SOAPElement dialogIdentifierElement = errorBodyElement.addChildElement(XMLUtil.DIALOG_IDENTIFIER_TAG, XMLUtil.WSARJCOOR_PREFIX, XMLUtil.WSARJCOOR_NAMESPACE_URI);

                dialogIdentifierElement.addTextNode(errorMessage.dialogIdentifier);
            }

            SOAPConnection connection = _connectionFactory.createConnection();
            SOAPMessage    responce   = connection.call(message, errorMessage.targetProtocolService.getAddress());

            connection.close();
        }
        catch (SOAPException soapException)
        {
            throw new MarshalException("Marshal failure: " + soapException);
        }
    }

    private void writeObject(java.io.ObjectOutputStream objectOutputStream )
        throws java.io.IOException
    {
    }

    private void readObject(java.io.ObjectInputStream objectInputStream)
        throws java.io.IOException, ClassNotFoundException
    {
    }

    private SOAPConnectionFactory     _connectionFactory     = null;
    private String                    _serviceURI            = null;
    private List                      _requestList           = new LinkedList();
}
