/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * PhaseZeroCoordinatorImpl.java
 */

package com.arjuna.wst.impl.messaging.saaj;

import java.util.Iterator;
import java.util.List;
import java.util.LinkedList;
import java.util.Map;
import java.util.HashMap;
import javax.xml.soap.*;
import com.arjuna.wsc.impl.messaging.saaj.PortReferenceHelper;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.impl.messaging.saaj.MessageListener;

public class PhaseZeroCoordinatorImpl implements MessageListener, PhaseZeroCoordinator, java.io.Serializable
{
    public PhaseZeroCoordinatorImpl(String serviceURI)
        throws SOAPException
    {
        _connectionFactory = SOAPConnectionFactory.newInstance();
        _serviceURI        = serviceURI;
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

                if (elementName.getLocalName().equals(XMLUtil.DIALOG_IDENTIFIER_TAG) && elementName.getURI().equals(XMLUtil.WSARJTX_NAMESPACE_URI))
                    dialogIdentifier = element.getValue();
            }

            if (dialogIdentifier == null)
            {
                synchronized(_messageListsSyncObject)
                {
                    _noDialogIdentifierMessageList.add(envelope);
                    _messageListsSyncObject.notifyAll();
                }
            }
            else
            {
                synchronized(_messageListsSyncObject)
                {
                    List messageList = (List) _dialogIdentifierMessageMap.get(dialogIdentifier);

                    if (messageList == null)
                    {
                        messageList = new LinkedList();
                        _dialogIdentifierMessageMap.put(dialogIdentifier, messageList);
                    }

                    messageList.add(envelope);
                    _messageListsSyncObject.notifyAll();
                }
            }
        }
        catch (Exception exception)
        {
            System.err.println("PhaseZeroCoordinatorImpl: " + exception);
        }
        catch (Error error)
        {
            System.err.println("PhaseZeroCoordinatorImpl: " + error);
        }
    }

    private SOAPEnvelope receiveMessage()
    {
        SOAPEnvelope message = null;

        try
        {
            synchronized(_messageListsSyncObject)
            {
                do
                {
                    if (! _noDialogIdentifierMessageList.isEmpty())
                        message = (SOAPEnvelope) _noDialogIdentifierMessageList.remove(0);
                    else if (! _dialogIdentifierMessageMap.isEmpty())
                    {
                        String dialogIdentifier = (String) _dialogIdentifierMessageMap.keySet().iterator().next();
                        List   messageList      = (List)   _dialogIdentifierMessageMap.get(dialogIdentifier);

                        message = (SOAPEnvelope) messageList.remove(0);

                        if (messageList.isEmpty())
                            _dialogIdentifierMessageMap.remove(dialogIdentifier);
                    }
                    else
                        _messageListsSyncObject.wait();
                }
                while (message == null);
            }
        }
        catch (Exception exception)
        {
            System.err.println("PhaseZeroCoordinatorImpl: " + exception);
        }
        catch (Error error)
        {
            System.err.println("PhaseZeroCoordinatorImpl: " + error);
        }

        return message;
    }

    private SOAPEnvelope receiveMessage(String dialogIdentifier)
    {
        SOAPEnvelope message = null;

        try
        {
            if (dialogIdentifier == null)
            {
                synchronized(_messageListsSyncObject)
                {
                    do
                    {
                        if (_noDialogIdentifierMessageList.isEmpty())
                            _messageListsSyncObject.wait();
                        else
                            message = (SOAPEnvelope) _noDialogIdentifierMessageList.remove(0);
                    }
                    while (message == null);
                }
            }
            else
            {
                synchronized(_messageListsSyncObject)
                {
                    List messageList = null;

                    do
                    {
                        messageList = (List) _dialogIdentifierMessageMap.get(dialogIdentifier);

                        if ((messageList == null) || messageList.isEmpty())
                            _messageListsSyncObject.wait();
                        else
                            message = (SOAPEnvelope) messageList.remove(0);
                    }
                    while (message == null);

                    if (messageList.isEmpty())
                        _dialogIdentifierMessageMap.remove(dialogIdentifier);
                }
            }
        }
        catch (Exception exception)
        {
            System.err.println("PhaseZeroCoordinatorImpl: " + exception);
        }
        catch (Error error)
        {
            System.err.println("PhaseZeroCoordinatorImpl: " + error);
        }

        return message;
    }

    public String getServiceURI()
    {
        return _serviceURI;
    }

    public void sendPhaseZero(PhaseZeroMessage phaseZeroMessage)
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

            Name            phaseZeroName                = envelope.createName(XMLUtil.PHASE_ZERO_TAG, XMLUtil.WSTX_PREFIX, XMLUtil.WSTX_NAMESPACE_URI);
            SOAPBodyElement phaseZeroBodyElement         = body.addBodyElement(phaseZeroName);
            SOAPElement     targetProtocolServiceElement = phaseZeroBodyElement.addChildElement(XMLUtil.TARGET_PROTOCOL_SERVICE_TAG, XMLUtil.WSTX_PREFIX, XMLUtil.WSTX_NAMESPACE_URI);
            SOAPElement     sourceProtocolServiceElement = phaseZeroBodyElement.addChildElement(XMLUtil.SOURCE_PROTOCOL_SERVICE_TAG, XMLUtil.WSTX_PREFIX, XMLUtil.WSTX_NAMESPACE_URI);

            PortReferenceHelper.toXML(envelope, targetProtocolServiceElement, phaseZeroMessage.targetProtocolService);
            PortReferenceHelper.toXML(envelope, sourceProtocolServiceElement, phaseZeroMessage.sourceProtocolService);
            if (phaseZeroMessage.dialogIdentifier != null)
            {
                SOAPElement dialogIdentifierElement = phaseZeroBodyElement.addChildElement(XMLUtil.DIALOG_IDENTIFIER_TAG, XMLUtil.WSARJTX_PREFIX, XMLUtil.WSARJTX_NAMESPACE_URI);

                dialogIdentifierElement.addTextNode(phaseZeroMessage.dialogIdentifier);
            }

            SOAPConnection connection = _connectionFactory.createConnection();
            SOAPMessage    responce   = connection.call(message, phaseZeroMessage.targetProtocolService.getAddress());

            connection.close();
        }
        catch (com.arjuna.wsc.messaging.MarshalException marshalException)
        {
            throw new MarshalException("Marshal failure: " + marshalException);
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

            Name            errorName                    = envelope.createName(XMLUtil.ERROR_TAG, XMLUtil.WSTX_PREFIX, XMLUtil.WSTX_NAMESPACE_URI);
            SOAPBodyElement errorBodyElement             = body.addBodyElement(errorName);
            SOAPElement     targetProtocolServiceElement = errorBodyElement.addChildElement(XMLUtil.TARGET_PROTOCOL_SERVICE_TAG, XMLUtil.WSTX_PREFIX, XMLUtil.WSTX_NAMESPACE_URI);
            SOAPElement     errorCodeElement             = errorBodyElement.addChildElement(XMLUtil.ERROR_CODE_TAG, XMLUtil.WSTX_PREFIX, XMLUtil.WSTX_NAMESPACE_URI);

            PortReferenceHelper.toXML(envelope, targetProtocolServiceElement, errorMessage.targetProtocolService);
            ErrorCodeHelper.toXML(envelope, errorCodeElement, errorMessage.errorCode);
            if (errorMessage.dialogIdentifier != null)
            {
                SOAPElement dialogIdentifierElement = errorBodyElement.addChildElement(XMLUtil.DIALOG_IDENTIFIER_TAG, XMLUtil.WSARJTX_PREFIX, XMLUtil.WSARJTX_NAMESPACE_URI);

                dialogIdentifierElement.addTextNode(errorMessage.dialogIdentifier);
            }

            SOAPConnection connection = _connectionFactory.createConnection();
            SOAPMessage    responce   = connection.call(message, errorMessage.targetProtocolService.getAddress());

            connection.close();
        }
        catch (com.arjuna.wsc.messaging.MarshalException marshalException)
        {
            throw new MarshalException("Marshal failure: " + marshalException);
        }
        catch (SOAPException soapException)
        {
            throw new MarshalException("Marshal failure: " + soapException);
        }
    }

    public Object recv()
        throws MarshalException
    {
        return recv(receiveMessage());
    }

    public Object recv(String dialogIdentifier)
        throws MarshalException
    {
        return recv(receiveMessage(dialogIdentifier));
    }

    private Object recv(SOAPEnvelope envelope)
        throws MarshalException
    {
        Object wstMessage = null;

        try
        {
            SOAPBody        body         = envelope.getBody();
            Iterator        bodyElements = body.getChildElements();
            SOAPBodyElement bodyElement  = (SOAPBodyElement) bodyElements.next();

            if (bodyElements.hasNext())
                throw new MarshalException("Unexpected extra body element");

            Name bodyElementName = bodyElement.getElementName();

            if ((bodyElementName.getLocalName().equals(XMLUtil.PHASE_ZERO_COMPLETED_TAG) && bodyElementName.getURI().equals(XMLUtil.WSTX_NAMESPACE_URI)))
            {
                PhaseZeroCompletedMessage phaseZeroCompletedMessage = new PhaseZeroCompletedMessage();

                Iterator    parameters    = bodyElement.getChildElements();
                SOAPElement parameter     = null;
                Name        parameterName = null;

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();

                    if (parameterName.getLocalName().equals(XMLUtil.TARGET_PROTOCOL_SERVICE_TAG) && parameterName.getURI().equals(XMLUtil.WSTX_NAMESPACE_URI))
                        phaseZeroCompletedMessage.targetProtocolService = PortReferenceHelper.fromXML(envelope, parameter);
                    else
                        throw new MarshalException("Unexpected element: " + parameterName.getLocalName() + "; " + parameterName.getURI());
                }
                else
                    throw new MarshalException("Missing element: \"" + XMLUtil.TARGET_PROTOCOL_SERVICE_TAG + "\"");

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();

                    if (parameterName.getLocalName().equals(XMLUtil.SOURCE_PROTOCOL_SERVICE_TAG) && parameterName.getURI().equals(XMLUtil.WSTX_NAMESPACE_URI))
                        phaseZeroCompletedMessage.sourceProtocolService = PortReferenceHelper.fromXML(envelope, parameter);
                    else
                        throw new MarshalException("Unexpected element: " + parameterName.getLocalName() + "; " + parameterName.getURI());
                }
                else
                    throw new MarshalException("Missing element: \"" + XMLUtil.SOURCE_PROTOCOL_SERVICE_TAG + "\"");

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();

                    if (parameterName.getLocalName().equals(XMLUtil.DIALOG_IDENTIFIER_TAG) && parameterName.getURI().equals(XMLUtil.WSARJTX_NAMESPACE_URI))
                        phaseZeroCompletedMessage.dialogIdentifier = parameter.getValue();
                    else
                        throw new MarshalException("Unexpected extra element(s): " + parameterName.getLocalName() + "; " + parameterName.getURI());
                }
                else
                    phaseZeroCompletedMessage.dialogIdentifier = null;

                if (parameters.hasNext())
                    throw new MarshalException("Unexpected extra element(s)");

                wstMessage = phaseZeroCompletedMessage;
            }
            else if ((bodyElementName.getLocalName().equals(XMLUtil.UNKNOWN_TAG) && bodyElementName.getURI().equals(XMLUtil.WSTX_NAMESPACE_URI)))
            {
                UnknownMessage unknownMessage = new UnknownMessage();

                Iterator    parameters    = bodyElement.getChildElements();
                SOAPElement parameter     = null;
                Name        parameterName = null;

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();

                    if (parameterName.getLocalName().equals(XMLUtil.TARGET_PROTOCOL_SERVICE_TAG) && parameterName.getURI().equals(XMLUtil.WSTX_NAMESPACE_URI))
                        unknownMessage.targetProtocolService = PortReferenceHelper.fromXML(envelope, parameter);
                    else
                        throw new MarshalException("Unexpected element: " + parameterName.getLocalName() + "; " + parameterName.getURI());
                }
                else
                    throw new MarshalException("Missing element: \"" + XMLUtil.TARGET_PROTOCOL_SERVICE_TAG + "\"");

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();

                    if (parameterName.getLocalName().equals(XMLUtil.SOURCE_PROTOCOL_SERVICE_TAG) && parameterName.getURI().equals(XMLUtil.WSTX_NAMESPACE_URI))
                        unknownMessage.sourceProtocolService = PortReferenceHelper.fromXML(envelope, parameter);
                    else
                        throw new MarshalException("Unexpected element: " + parameterName.getLocalName() + "; " + parameterName.getURI());
                }
                else
                    throw new MarshalException("Missing element: \"" + XMLUtil.SOURCE_PROTOCOL_SERVICE_TAG + "\"");

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();

                    if (parameterName.getLocalName().equals(XMLUtil.DIALOG_IDENTIFIER_TAG) && parameterName.getURI().equals(XMLUtil.WSARJTX_NAMESPACE_URI))
                        unknownMessage.dialogIdentifier = parameter.getValue();
                    else
                        throw new MarshalException("Unexpected extra element(s): " + parameterName.getLocalName() + "; " + parameterName.getURI());
                }
                else
                    unknownMessage.dialogIdentifier = null;

                if (parameters.hasNext())
                    throw new MarshalException("Unexpected extra element(s)");

                wstMessage = unknownMessage;
            }
            else if (bodyElementName.getLocalName().equals(XMLUtil.ERROR_TAG) && bodyElementName.getURI().equals(XMLUtil.WSTX_NAMESPACE_URI))
            {
                ErrorMessage errorMessage = new ErrorMessage();

                Iterator    parameters    = bodyElement.getChildElements();
                SOAPElement parameter     = null;
                Name        parameterName = null;

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();

                    if (parameterName.getLocalName().equals(XMLUtil.TARGET_PROTOCOL_SERVICE_TAG) && parameterName.getURI().equals(XMLUtil.WSTX_NAMESPACE_URI))
                        errorMessage.targetProtocolService = PortReferenceHelper.fromXML(envelope, parameter);
                    else
                        throw new MarshalException("Unexpected element: " + parameterName.getLocalName() + "; " + parameterName.getURI());
                }
                else
                    throw new MarshalException("Missing element: \"" + XMLUtil.TARGET_PROTOCOL_SERVICE_TAG + "\"");

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();

                    if (parameterName.getLocalName().equals(XMLUtil.ERROR_CODE_TAG) && parameterName.getURI().equals(XMLUtil.WSTX_NAMESPACE_URI))
                        errorMessage.errorCode = ErrorCodeHelper.fromXML(envelope, parameter);
                    else
                        throw new MarshalException("Unexpected element: " + parameterName.getLocalName() + "; " + parameterName.getURI());
                }
                else
                    throw new MarshalException("Missing element: \"" + XMLUtil.ERROR_CODE_TAG + "\"");

                if (parameters.hasNext())
                {
                    parameter     = (SOAPElement) parameters.next();
                    parameterName = parameter.getElementName();

                    if (parameterName.getLocalName().equals(XMLUtil.DIALOG_IDENTIFIER_TAG) && parameterName.getURI().equals(XMLUtil.WSARJTX_NAMESPACE_URI))
                        errorMessage.dialogIdentifier = parameter.getValue();
                    else
                        throw new MarshalException("Unexpected extra element(s): " + parameterName.getLocalName() + "; " + parameterName.getURI());
                }
                else
                    errorMessage.dialogIdentifier = null;

                if (parameters.hasNext())
                    throw new MarshalException("Unexpected extra element(s)");

                wstMessage = errorMessage;
            }
            else
                throw new MarshalException("Not expecting Tag: " + bodyElementName);
        }
        catch (SOAPException soapException)
        {
            throw new MarshalException("Marshal failure: " + soapException);
        }
        catch (com.arjuna.wsc.messaging.MarshalException marshalException)
        {
            throw new MarshalException("Marshal failure: " + marshalException);
        }

        return wstMessage;
    }

    private void writeObject(java.io.ObjectOutputStream objectOutputStream )
        throws java.io.IOException
    {
    }

    private void readObject(java.io.ObjectInputStream objectInputStream)
        throws java.io.IOException, ClassNotFoundException
    {
    }

    private SOAPConnectionFactory _connectionFactory             = null;
    private String                _serviceURI                    = null;
    private Object                _messageListsSyncObject        = new Object();
    private List                  _noDialogIdentifierMessageList = new LinkedList();
    private Map                   _dialogIdentifierMessageMap    = new HashMap();
}
