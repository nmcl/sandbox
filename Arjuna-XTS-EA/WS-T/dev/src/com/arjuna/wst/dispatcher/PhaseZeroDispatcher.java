/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * PhaseZeroDispatcher.java
 */

package com.arjuna.wst.dispatcher;

import java.util.Map;
import java.util.HashMap;
import com.arjuna.wst.SystemException;
import com.arjuna.wst.messaging.ErrorCode;
import com.arjuna.wst.messaging.PhaseZeroMessage;
import com.arjuna.wst.messaging.PhaseZeroCompletedMessage;
import com.arjuna.wst.messaging.UnknownMessage;
import com.arjuna.wst.messaging.ErrorMessage;
import com.arjuna.wst.impl.messaging.XMLUtil;

public class PhaseZeroDispatcher implements java.io.Serializable
{
    public PhaseZeroDispatcher(com.arjuna.wst.messaging.PhaseZeroParticipant phaseZeroParticipantEndpoint)
        throws Exception
    {
        _phaseZeroParticipantEndpoint = phaseZeroParticipantEndpoint;

        _dispatcher = new Dispatcher();
        _dispatcher.start();
    }

    public String getServiceURI()
    {
        return _phaseZeroParticipantEndpoint.getServiceURI();
    }

    public void activateObject(com.arjuna.wst.PhaseZeroParticipant phaseZeroParticipantObject, String transactionIdentifier)
    {
        synchronized(_phaseZeroParticipantObjectMap)
        {
            _phaseZeroParticipantObjectMap.put(transactionIdentifier, phaseZeroParticipantObject);
            _transactionIdentifierMap.put(phaseZeroParticipantObject, transactionIdentifier);
        }
    }

    public void deactivateObject(com.arjuna.wst.PhaseZeroParticipant phaseZeroParticipantObject)
    {
        synchronized(_phaseZeroParticipantObjectMap)
        {
            String transactionIdentifier = (String) _transactionIdentifierMap.get(phaseZeroParticipantObject);

            _phaseZeroParticipantObjectMap.remove(transactionIdentifier);
            _transactionIdentifierMap.remove(phaseZeroParticipantObject);
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
                    Object message = _phaseZeroParticipantEndpoint.recv();

                    if (message instanceof PhaseZeroMessage)
	            {
                        PhaseZeroMessage phaseZeroMessage = (PhaseZeroMessage) message;

                        com.arjuna.wst.PhaseZeroParticipant phaseZeroParticipantObject = null;

                        synchronized(_phaseZeroParticipantObjectMap)
                        {
                            phaseZeroParticipantObject = (com.arjuna.wst.PhaseZeroParticipant) _phaseZeroParticipantObjectMap.get(phaseZeroMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
                        }

                        if (phaseZeroParticipantObject != null)
                        {
                            try
                            {
                                phaseZeroParticipantObject.phaseZero();

                                PhaseZeroCompletedMessage phaseZeroCompletedMessage = new PhaseZeroCompletedMessage();

                                phaseZeroCompletedMessage.targetProtocolService = phaseZeroMessage.sourceProtocolService;
                                phaseZeroCompletedMessage.sourceProtocolService = phaseZeroMessage.targetProtocolService;
                                phaseZeroCompletedMessage.dialogIdentifier      = phaseZeroMessage.dialogIdentifier;

                                _phaseZeroParticipantEndpoint.sendPhaseZeroCompleted(phaseZeroCompletedMessage);
                            }
                            catch (SystemException systemException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = phaseZeroMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = phaseZeroMessage.dialogIdentifier;

                                _phaseZeroParticipantEndpoint.sendError(errorMessage);
                            }
                            catch (Throwable throwable)
                            {
                                System.err.println("Unexpected exception thrown from phaseZero: \"" + throwable + "\"");

                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = phaseZeroMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = phaseZeroMessage.dialogIdentifier;

                                _phaseZeroParticipantEndpoint.sendError(errorMessage);
                            }
                        }
                        else
                        {
                            System.err.println("PhaseZero called on unknown participant: \"" + phaseZeroMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG) + "\"");

                            UnknownMessage unknownMessage = new UnknownMessage();

                            unknownMessage.targetProtocolService = phaseZeroMessage.sourceProtocolService;
                            unknownMessage.sourceProtocolService = phaseZeroMessage.targetProtocolService;
                            unknownMessage.dialogIdentifier      = phaseZeroMessage.dialogIdentifier;

                            _phaseZeroParticipantEndpoint.sendUnknown(unknownMessage);
                        }
		    }
                    else if (message instanceof ErrorMessage)
	            {
                        ErrorMessage errorMessage = (ErrorMessage) message;

                        com.arjuna.wst.PhaseZeroParticipant phaseZeroParticipantObject = null;

                        synchronized(_phaseZeroParticipantObjectMap)
                        {
                            phaseZeroParticipantObject = (com.arjuna.wst.PhaseZeroParticipant) _phaseZeroParticipantObjectMap.get(errorMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
                        }

                        if (phaseZeroParticipantObject != null)
                            try
                            {
                                phaseZeroParticipantObject.error();
                            }
                            catch (Throwable throwable)
                            {
                                System.err.println("Unexpected exception thrown from error: \"" + throwable + "\"");
                            }
                        else
                            System.err.println("Notified called on unknown participant: \"" + errorMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG) + "\"");
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

    private com.arjuna.wst.messaging.PhaseZeroParticipant _phaseZeroParticipantEndpoint  = null;
    private Map                                           _phaseZeroParticipantObjectMap = new HashMap();
    private Map                                           _transactionIdentifierMap      = new HashMap();
    private Dispatcher                                    _dispatcher                    = null;
}
