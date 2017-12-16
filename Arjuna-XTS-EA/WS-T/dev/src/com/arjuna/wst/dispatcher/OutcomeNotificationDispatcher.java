/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * OutcomeNotificationDispatcher.java
 */

package com.arjuna.wst.dispatcher;

import java.util.Map;
import java.util.HashMap;
import com.arjuna.wst.SystemException;
import com.arjuna.wst.messaging.ErrorCode;
import com.arjuna.wst.messaging.CommittedMessage;
import com.arjuna.wst.messaging.AbortedMessage;
import com.arjuna.wst.messaging.NotifiedMessage;
import com.arjuna.wst.messaging.UnknownMessage;
import com.arjuna.wst.messaging.ErrorMessage;
import com.arjuna.wst.impl.messaging.XMLUtil;

public class OutcomeNotificationDispatcher implements java.io.Serializable
{
    public OutcomeNotificationDispatcher(com.arjuna.wst.messaging.OutcomeNotificationParticipant outcomeNotificationParticipantEndpoint)
        throws Exception
    {
        _outcomeNotificationParticipantEndpoint = outcomeNotificationParticipantEndpoint;

        _dispatcher = new Dispatcher();
        _dispatcher.start();
    }

    public String getServiceURI()
    {
        return _outcomeNotificationParticipantEndpoint.getServiceURI();
    }

    public void activateObject(com.arjuna.wst.OutcomeNotificationParticipant outcomeNotificationParticipantObject, String transactionIdentifier)
    {
        synchronized(_outcomeNotificationParticipantObjectMap)
        {
            _outcomeNotificationParticipantObjectMap.put(transactionIdentifier, outcomeNotificationParticipantObject);
            _transactionIdentifierMap.put(outcomeNotificationParticipantObject, transactionIdentifier);
        }
    }

    public void deactivateObject(com.arjuna.wst.OutcomeNotificationParticipant outcomeNotificationParticipantObject)
    {
        synchronized(_outcomeNotificationParticipantObjectMap)
        {
            String transactionIdentifier = (String) _transactionIdentifierMap.get(outcomeNotificationParticipantObject);

            _outcomeNotificationParticipantObjectMap.remove(transactionIdentifier);
            _transactionIdentifierMap.remove(outcomeNotificationParticipantObject);
        }
    }

    private class Dispatcher extends Thread
    {
        public void run()
	{
            while (true)
	    {
                try
		{
                    Object message = _outcomeNotificationParticipantEndpoint.recv();

                    if (message instanceof CommittedMessage)
	            {
                        CommittedMessage committedMessage = (CommittedMessage) message;

                        com.arjuna.wst.OutcomeNotificationParticipant outcomeNotificationParticipantObject = null;

                        synchronized(_outcomeNotificationParticipantObjectMap)
                        {
                            outcomeNotificationParticipantObject = (com.arjuna.wst.OutcomeNotificationParticipant) _outcomeNotificationParticipantObjectMap.get(committedMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
                        }

                        if (outcomeNotificationParticipantObject != null)
                        {
                            try
                            {
                                outcomeNotificationParticipantObject.committed();

                                NotifiedMessage notifiedMessage = new NotifiedMessage();

                                notifiedMessage.targetProtocolService = committedMessage.sourceProtocolService;
                                notifiedMessage.sourceProtocolService = committedMessage.targetProtocolService;
                                notifiedMessage.dialogIdentifier      = committedMessage.dialogIdentifier;

                                _outcomeNotificationParticipantEndpoint.sendNotified(notifiedMessage);
                            }
                            catch (SystemException systemException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = committedMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = committedMessage.dialogIdentifier;

                                _outcomeNotificationParticipantEndpoint.sendError(errorMessage);
                            }
                            catch (Throwable throwable)
                            {
                                System.err.println("Unexpected exception thrown from committed: \"" + throwable + "\"");

                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = committedMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = committedMessage.dialogIdentifier;

                                _outcomeNotificationParticipantEndpoint.sendError(errorMessage);
                            }
                        }
                        else
                        {
                            System.err.println("Committed called on unknown participant: \"" + committedMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG) + "\"");

                            UnknownMessage unknownMessage = new UnknownMessage();

                            unknownMessage.targetProtocolService = committedMessage.sourceProtocolService;
                            unknownMessage.sourceProtocolService = committedMessage.targetProtocolService;
                            unknownMessage.dialogIdentifier      = committedMessage.dialogIdentifier;

                            _outcomeNotificationParticipantEndpoint.sendUnknown(unknownMessage);
                        }
		    }
                    else if (message instanceof AbortedMessage)
	            {
                        AbortedMessage abortedMessage = (AbortedMessage) message;

                        com.arjuna.wst.OutcomeNotificationParticipant outcomeNotificationParticipantObject = null;

                        synchronized(_outcomeNotificationParticipantObjectMap)
                        {
                            outcomeNotificationParticipantObject = (com.arjuna.wst.OutcomeNotificationParticipant) _outcomeNotificationParticipantObjectMap.get(abortedMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
                        }

                        if (outcomeNotificationParticipantObject != null)
                        {
                            try
                            {
                                outcomeNotificationParticipantObject.aborted();

                                NotifiedMessage notifiedMessage = new NotifiedMessage();

                                notifiedMessage.targetProtocolService = abortedMessage.sourceProtocolService;
                                notifiedMessage.sourceProtocolService = abortedMessage.targetProtocolService;
                                notifiedMessage.dialogIdentifier      = abortedMessage.dialogIdentifier;

                                _outcomeNotificationParticipantEndpoint.sendNotified(notifiedMessage);
                            }
                            catch (SystemException systemException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = abortedMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = abortedMessage.dialogIdentifier;

                                _outcomeNotificationParticipantEndpoint.sendError(errorMessage);
                            }
                            catch (Throwable throwable)
                            {
                                System.err.println("Unexpected exception thrown from aborted: \"" + throwable + "\"");

                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = abortedMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = abortedMessage.dialogIdentifier;

                                _outcomeNotificationParticipantEndpoint.sendError(errorMessage);
                            }
                        }
                        else
                        {
                            System.err.println("Aborted called on unknown participant: \"" + abortedMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG) + "\"");

                            UnknownMessage unknownMessage = new UnknownMessage();

                            unknownMessage.targetProtocolService = abortedMessage.sourceProtocolService;
                            unknownMessage.sourceProtocolService = abortedMessage.targetProtocolService;
                            unknownMessage.dialogIdentifier      = abortedMessage.dialogIdentifier;

                            _outcomeNotificationParticipantEndpoint.sendUnknown(unknownMessage);
                        }
		    }
                    else if (message instanceof UnknownMessage)
	            {
                        UnknownMessage unknownMessage = (UnknownMessage) message;

                        com.arjuna.wst.OutcomeNotificationParticipant outcomeNotificationParticipantObject = null;

                        synchronized(_outcomeNotificationParticipantObjectMap)
                        {
                            outcomeNotificationParticipantObject = (com.arjuna.wst.OutcomeNotificationParticipant) _outcomeNotificationParticipantObjectMap.get(unknownMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
                        }

                        if (outcomeNotificationParticipantObject != null)
                            try
                            {
                                outcomeNotificationParticipantObject.unknown();
                            }
                            catch (Throwable throwable)
                            {
                                System.err.println("Unexpected exception thrown from error: \"" + throwable + "\"");
                            }
                        else
                            System.err.println("Unknown called on unknown participant: \"" + unknownMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG) + "\"");
		    }
                    else if (message instanceof ErrorMessage)
	            {
                        ErrorMessage errorMessage = (ErrorMessage) message;

                        com.arjuna.wst.OutcomeNotificationParticipant outcomeNotificationParticipantObject = null;

                        synchronized(_outcomeNotificationParticipantObjectMap)
                        {
                            outcomeNotificationParticipantObject = (com.arjuna.wst.OutcomeNotificationParticipant) _outcomeNotificationParticipantObjectMap.get(errorMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
                        }

                        if (outcomeNotificationParticipantObject != null)
                            try
                            {
                                outcomeNotificationParticipantObject.error();
                            }
                            catch (Throwable throwable)
                            {
                                System.err.println("Unexpected exception thrown from error: \"" + throwable + "\"");
                            }
                        else
                            System.err.println("Error called on unknown participant: \"" + errorMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG) + "\"");
		    }
                    else
                        System.err.println("Unexpected message class: \"" + message.getClass().getName() + "\"");
		}
		catch (Throwable throwable)
	        {
                    throwable.printStackTrace(System.err);
		}
	    }
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

    private com.arjuna.wst.messaging.OutcomeNotificationParticipant _outcomeNotificationParticipantEndpoint  = null;
    private Map                                                     _outcomeNotificationParticipantObjectMap = new HashMap();
    private Map                                                     _transactionIdentifierMap                = new HashMap();
    private Dispatcher                                              _dispatcher                              = null;
}
