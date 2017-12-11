/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrationCoordinatorImpl.java
 */

package com.arjuna.wsc.impl.messaging.saaj;

import java.util.Iterator;
import java.util.List;
import java.util.LinkedList;
import javax.xml.soap.*;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.impl.messaging.XMLUtil;
import com.arjuna.wsc.impl.messaging.saaj.MessageListener;

public class RegistrationCoordinatorImpl implements MessageListener, RegistrationCoordinator, java.io.Serializable
{
    public RegistrationCoordinatorImpl(String serviceURI)
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
            System.err.println("RegistrationCoordinatorImpl: " + exception);
        }
        catch (Error error)
        {
            System.err.println("RegistrationCoordinatorImpl: " + error);
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
            System.err.println("RegistrationCoordinatorImpl: " + exception);
        }
        catch (Error error)
        {
            System.err.println("RegistrationCoordinatorImpl: " + error);
        }

        return request;
    }

    public String getServiceURI()
    {
        return _serviceURI;
    }

    public RegisterMessage recvRegister()
        throws MarshalException
    {
        try
        {
            RegisterMessage registerMessage = new RegisterMessage();

            SOAPEnvelope envelope = receiveRequest();
            SOAPBody     body     = envelope.getBody();

            Iterator        bodyElements = body.getChildElements();
            SOAPBodyElement bodyElement  = (SOAPBodyElement) bodyElements.next();

            if (bodyElements.hasNext())
                throw new MarshalException("Unexpected extra body element(s)");

            Name bodyElementName = bodyElement.getElementName();

            if (! (bodyElementName.getLocalName().equals(XMLUtil.REGISTER_TAG) && bodyElementName.getURI().equals(XMLUtil.WSCOOR_NAMESPACE_URI)))
                throw new MarshalException("Not expecting Tag: " + bodyElementName);

            Iterator    parameters = bodyElement.getChildElements();
            SOAPElement parameter1 = (SOAPElement) parameters.next();
            SOAPElement parameter2 = (SOAPElement) parameters.next();
            SOAPElement parameter3 = (SOAPElement) parameters.next();
            SOAPElement parameter4 = (SOAPElement) parameters.next();

            registerMessage.registrationService        = PortReferenceHelper.fromXML(envelope, parameter1);
            registerMessage.requesterReference         = PortReferenceHelper.fromXML(envelope, parameter2);
            registerMessage.protocolIdentifier         = parameter3.getValue();
            registerMessage.participantProtocolService = PortReferenceHelper.fromXML(envelope, parameter4);

            if (parameters.hasNext())
            {
                SOAPElement parameter     = (SOAPElement) parameters.next();
                Name        parameterName = parameter.getElementName();

                if (parameterName.getLocalName().equals(XMLUtil.DIALOG_IDENTIFIER_TAG) && parameterName.getURI().equals(XMLUtil.WSARJCOOR_NAMESPACE_URI))
                    registerMessage.dialogIdentifier = parameter.getValue();
                else
                    throw new MarshalException("Unexpected extra element(s)");
            }
            else
                registerMessage.dialogIdentifier = null;

            return registerMessage;
        }
        catch (SOAPException soapException)
        {
            throw new MarshalException("Marshal failure: " + soapException);
        }
    }

    public void sendRegisterResponse(RegisterResponseMessage registerResponseMessage)
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

            Name            registarResponceName              = envelope.createName(XMLUtil.REGISTER_RESPONSE_TAG, null, XMLUtil.WSCOOR_NAMESPACE_URI);
            SOAPBodyElement registarResponceBodyElement       = body.addBodyElement(registarResponceName);
            SOAPElement     requestReferenceElement           = registarResponceBodyElement.addChildElement(XMLUtil.REQUESTER_REFERENCE_TAG);
            SOAPElement     coordinatorProtocolServiceElement = registarResponceBodyElement.addChildElement(XMLUtil.COORDINATOR_PROTOCOL_SERVICE_TAG);

            PortReferenceHelper.toXML(envelope, requestReferenceElement, registerResponseMessage.requesterReference);
            PortReferenceHelper.toXML(envelope, coordinatorProtocolServiceElement, registerResponseMessage.coordinationProtocolService);
            if (registerResponseMessage.dialogIdentifier != null)
            {
                SOAPElement dialogIdentifierElement = registarResponceBodyElement.addChildElement(XMLUtil.DIALOG_IDENTIFIER_TAG, XMLUtil.WSARJCOOR_PREFIX, XMLUtil.WSARJCOOR_NAMESPACE_URI);

                dialogIdentifierElement.addTextNode(registerResponseMessage.dialogIdentifier);
            }

            SOAPConnection connection = _connectionFactory.createConnection();
            SOAPMessage    responce   = connection.call(message, registerResponseMessage.requesterReference.getAddress());

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

    private SOAPConnectionFactory _connectionFactory = null;
    private String                _serviceURI        = null;
    private List                  _requestList       = new LinkedList();
}
