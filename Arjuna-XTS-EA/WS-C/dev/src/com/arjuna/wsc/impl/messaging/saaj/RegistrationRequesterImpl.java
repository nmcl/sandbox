/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrationRequesterImpl.java
 */

package com.arjuna.wsc.impl.messaging.saaj;

import java.util.Iterator;
import java.util.List;
import java.util.LinkedList;
import java.util.Map;
import java.util.HashMap;
import javax.xml.soap.*;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.impl.messaging.XMLUtil;
import com.arjuna.wsc.impl.messaging.saaj.MessageListener;

public class RegistrationRequesterImpl implements MessageListener, RegistrationRequester, java.io.Serializable
{
    public RegistrationRequesterImpl(String serviceURI)
        throws SOAPException
    {
        _connectionFactory = SOAPConnectionFactory.newInstance();
        _serviceURI = serviceURI;
    }

    public void onMessage(SOAPEnvelope envelope)
    {
        try
        {
            SOAPBody        body             = envelope.getBody();
            SOAPBodyElement bodyElement      = (SOAPBodyElement) body.getChildElements().next();
            Iterator        elements         = bodyElement.getChildElements();
            String          dialogIdentifier = null;

            while (elements.hasNext() && (dialogIdentifier == null))
            {
                SOAPElement element     = (SOAPElement) elements.next();
                Name        elementName = element.getElementName();

                if (elementName.getLocalName().equals(XMLUtil.DIALOG_IDENTIFIER_TAG) && elementName.getURI().equals(XMLUtil.WSARJCOOR_NAMESPACE_URI))
                    dialogIdentifier = element.getValue();
            }

            if (dialogIdentifier == null)
            {
                synchronized(_noDialogIdentifierResponceList)
                {
                    _noDialogIdentifierResponceList.add(envelope);
                    _noDialogIdentifierResponceList.notifyAll();
                }
            }
            else
            {
                synchronized(_dialogIdentifierResponceMap)
                {
                    List responceList = (List) _dialogIdentifierResponceMap.get(dialogIdentifier);

                    if (responceList == null)
                    {
                        responceList = new LinkedList();
                        _dialogIdentifierResponceMap.put(dialogIdentifier, responceList);
                    }

                    responceList.add(envelope);
                    _dialogIdentifierResponceMap.notifyAll();
                }
            }
        }
        catch (Exception exception)
        {
            System.err.println("RegistrationRequesterImpl: " + exception);
        }
        catch (Error error)
        {
            System.err.println("RegistrationRequesterImpl: " + error);
        }
    }

    public SOAPEnvelope receiveResponce(String dialogIdentifier)
    {
        SOAPEnvelope responce = null;

        try
        {
            if (dialogIdentifier == null)
            {
                synchronized(_noDialogIdentifierResponceList)
                {
                    do
                    {
                        if (_noDialogIdentifierResponceList.isEmpty())
                            _noDialogIdentifierResponceList.wait();
                        else
                            responce = (SOAPEnvelope) _noDialogIdentifierResponceList.remove(0);
                    }
                    while (responce == null);
                }
            }
            else
            {
                synchronized(_dialogIdentifierResponceMap)
                {
                    List responceList = null;

                    do
                    {
                        responceList = (List) _dialogIdentifierResponceMap.get(dialogIdentifier);

                        if ((responceList == null) || responceList.isEmpty())
                            _dialogIdentifierResponceMap.wait();
                        else
                            responce = (SOAPEnvelope) responceList.remove(0);
                    }
                    while (responce == null);

                    if (responceList.isEmpty())
                        _dialogIdentifierResponceMap.remove(dialogIdentifier);
                }
            }
        }
        catch (Exception exception)
        {
            System.err.println("RegistrationRequesterImpl: " + exception);
        }
        catch (Error error)
        {
            System.err.println("RegistrationRequesterImpl: " + error);
        }

        return responce;
    }

    public String getServiceURI()
    {
        return _serviceURI;
    }

    public void sendRegister(RegisterMessage registerMessage)
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

            Name            registarName                      = envelope.createName(XMLUtil.REGISTER_TAG, null, XMLUtil.WSCOOR_NAMESPACE_URI);
            SOAPBodyElement registarBodyElement               = body.addBodyElement(registarName);
            SOAPElement     registrationServiceElement        = registarBodyElement.addChildElement(XMLUtil.REGISTRATION_SERVICE_TAG);
            SOAPElement     requesterReferenceElement         = registarBodyElement.addChildElement(XMLUtil.REQUESTER_REFERENCE_TAG);
            SOAPElement     protocolIdentifierElement         = registarBodyElement.addChildElement(XMLUtil.PROTOCOL_IDENTIFIER_TAG);
            SOAPElement     participantProtocolServiceElement = registarBodyElement.addChildElement(XMLUtil.PARTICIPANT_PROTOCOL_SERVICE_TAG);

            PortReferenceHelper.toXML(envelope, registrationServiceElement, registerMessage.registrationService);
            PortReferenceHelper.toXML(envelope, requesterReferenceElement, registerMessage.requesterReference);
            protocolIdentifierElement.addTextNode(registerMessage.protocolIdentifier);
            PortReferenceHelper.toXML(envelope, participantProtocolServiceElement, registerMessage.participantProtocolService);
            if (registerMessage.dialogIdentifier != null)
            {
                SOAPElement dialogIdentifierElement = registarBodyElement.addChildElement(XMLUtil.DIALOG_IDENTIFIER_TAG, XMLUtil.WSARJCOOR_PREFIX, XMLUtil.WSARJCOOR_NAMESPACE_URI);

                dialogIdentifierElement.addTextNode(registerMessage.dialogIdentifier);
            }

            SOAPConnection connection = _connectionFactory.createConnection();
            SOAPMessage    responce   = connection.call(message, registerMessage.registrationService.getAddress());

            connection.close();
        }
        catch (SOAPException soapException)
        {
            throw new MarshalException("Marshal failure: " + soapException);
        }
    }

    public RegisterResponseMessage recvRegisterResponse(String dialogIdentifier)
        throws MarshalException, ErrorMessageException
    {
        try
        {
            SOAPEnvelope envelope = receiveResponce(dialogIdentifier);
            SOAPBody     body     = envelope.getBody();

            Iterator        bodyElements = body.getChildElements();
            SOAPBodyElement bodyElement  = (SOAPBodyElement) bodyElements.next();

            if (bodyElements.hasNext())
                throw new MarshalException("Unexpected extra body element");

            Name bodyElementName = bodyElement.getElementName();

            if (bodyElementName.getLocalName().equals(XMLUtil.REGISTER_RESPONSE_TAG) && bodyElementName.getURI().equals(XMLUtil.WSCOOR_NAMESPACE_URI))
            {
                RegisterResponseMessage registerResponseMessage = new RegisterResponseMessage();

                Iterator    parameters = bodyElement.getChildElements();
                SOAPElement parameter1 = (SOAPElement) parameters.next();
                SOAPElement parameter2 = (SOAPElement) parameters.next();

                registerResponseMessage.requesterReference          = PortReferenceHelper.fromXML(envelope, parameter1);
                registerResponseMessage.coordinationProtocolService = PortReferenceHelper.fromXML(envelope, parameter2);

                if (parameters.hasNext())
                {
                    SOAPElement parameter     = (SOAPElement) parameters.next();
                    Name        parameterName = parameter.getElementName();

                    if (parameterName.getLocalName().equals(XMLUtil.DIALOG_IDENTIFIER_TAG) && parameterName.getURI().equals(XMLUtil.WSARJCOOR_NAMESPACE_URI))
                        registerResponseMessage.dialogIdentifier = parameter.getValue();
                    else
                        throw new MarshalException("Unexpected extra element(s)");
                }
                else
                    registerResponseMessage.dialogIdentifier = null;

                return registerResponseMessage;
            }
            else if (bodyElementName.getLocalName().equals(XMLUtil.ERROR_TAG) && bodyElementName.getURI().equals(XMLUtil.WSCOOR_NAMESPACE_URI))
            {
                ErrorMessage errorMessage = new ErrorMessage();

                Iterator    parameters = bodyElement.getChildElements();
                SOAPElement parameter1 = (SOAPElement) parameters.next();
                SOAPElement parameter2 = (SOAPElement) parameters.next();

                errorMessage.targetProtocolService = PortReferenceHelper.fromXML(envelope, parameter1);
                errorMessage.errorCode             = ErrorCodeHelper.fromXML(envelope, parameter2);

                if (parameters.hasNext())
                {
                    SOAPElement parameter     = (SOAPElement) parameters.next();
                    Name        parameterName = parameter.getElementName();

                    if (parameterName.getLocalName().equals(XMLUtil.DIALOG_IDENTIFIER_TAG) && parameterName.getURI().equals(XMLUtil.WSARJCOOR_NAMESPACE_URI))
                        errorMessage.dialogIdentifier = parameter.getValue();
                    else
                        throw new MarshalException("Unexpected extra element(s)");
                }
                else
                    errorMessage.dialogIdentifier = null;

                throw new ErrorMessageException(errorMessage);
            }
            else
                throw new MarshalException("Not expecting Tag: " + bodyElementName);
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

    private SOAPConnectionFactory _connectionFactory              = null;
    private String                _serviceURI                     = null;
    private List                  _noDialogIdentifierResponceList = new LinkedList();
    private Map                   _dialogIdentifierResponceMap    = new HashMap();
}
