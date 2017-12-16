/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionWithAckDispatcher.java
 */

package com.arjuna.wst.dispatcher;

import java.util.Map;
import java.util.HashMap;
import com.arjuna.wst.SystemException;
import com.arjuna.wst.TransactionRolledBackException;
import com.arjuna.wst.UnknownTransactionException;
import com.arjuna.wst.messaging.ErrorCode;
import com.arjuna.wst.messaging.RollbackMessage;
import com.arjuna.wst.messaging.AbortedMessage;
import com.arjuna.wst.messaging.CommitMessage;
import com.arjuna.wst.messaging.CommittedMessage;
import com.arjuna.wst.messaging.NotifiedMessage;
import com.arjuna.wst.messaging.ErrorMessage;
import com.arjuna.wst.impl.messaging.XMLUtil;

public class CompletionWithAckDispatcher implements java.io.Serializable
{
    public CompletionWithAckDispatcher(com.arjuna.wst.messaging.CompletionWithAckCoordinator completionWithAckCoordinatorEndpoint)
        throws Exception
    {
        _completionWithAckCoordinatorEndpoint = completionWithAckCoordinatorEndpoint;

        _dispatcher = new Dispatcher();
        _dispatcher.start();
    }

    public String getServiceURI()
    {
        return _completionWithAckCoordinatorEndpoint.getServiceURI();
    }

    public void activateObject(com.arjuna.wst.CompletionWithAckCoordinatorParticipant completionWithAckCoordinatorObject, String transactionIdentifier)
    {
        synchronized(_completionWithAckCoordinatorObjectMap)
        {
            _completionWithAckCoordinatorObjectMap.put(transactionIdentifier, completionWithAckCoordinatorObject);
            _transactionIdentifierMap.put(completionWithAckCoordinatorObject, transactionIdentifier);
        }
    }

    public void deactivateObject(com.arjuna.wst.CompletionWithAckCoordinatorParticipant completionWithAckCoordinatorObject)
    {
        synchronized(_completionWithAckCoordinatorObjectMap)
        {
            String transactionIdentifier = (String) _transactionIdentifierMap.get(completionWithAckCoordinatorObject);

            _completionWithAckCoordinatorObjectMap.remove(transactionIdentifier);
            _transactionIdentifierMap.remove(completionWithAckCoordinatorObject);
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
                    Object message = _completionWithAckCoordinatorEndpoint.recv();

                    if (message instanceof CommitMessage)
	            {
                        CommitMessage commitMessage = (CommitMessage) message;

                        com.arjuna.wst.CompletionWithAckCoordinatorParticipant completionWithAckCoordinatorObject = null;

                        synchronized(_completionWithAckCoordinatorObjectMap)
                        {
                            completionWithAckCoordinatorObject = (com.arjuna.wst.CompletionWithAckCoordinatorParticipant) _completionWithAckCoordinatorObjectMap.get(commitMessage.targetProtocolService.getExtensionValue(XMLUtil.TRANSACTION_IDENTIFIER_TAG));
                        }

                        if (completionWithAckCoordinatorObject != null)
                        {
                            try
                            {
                                completionWithAckCoordinatorObject.commit();

                                CommittedMessage committedMessage = new CommittedMessage();

                                committedMessage.targetProtocolService = commitMessage.sourceProtocolService;
                                committedMessage.sourceProtocolService = commitMessage.targetProtocolService;
                                committedMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                                _completionWithAckCoordinatorEndpoint.sendCommitted(committedMessage);
                            }
                            catch (TransactionRolledBackException transactionRolledBackException)
                            {
                                AbortedMessage abortedMessage = new AbortedMessage();

                                abortedMessage.targetProtocolService = commitMessage.sourceProtocolService;
                                abortedMessage.sourceProtocolService = commitMessage.targetProtocolService;
                                abortedMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                                _completionWithAckCoordinatorEndpoint.sendAborted(abortedMessage);
                            }
                            catch (UnknownTransactionException unknownTransactionException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = commitMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNTRANSACTION_ERROR_CODE;
                                errorMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                                _completionWithAckCoordinatorEndpoint.sendError(errorMessage);
                            }
                            catch (SystemException systemException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = commitMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                                _completionWithAckCoordinatorEndpoint.sendError(errorMessage);
                            }
                            catch (Throwable throwable)
                            {
                                System.err.println("Unexpected exception thrown from commit: \"" + throwable + "\"");

                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = commitMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                                _completionWithAckCoordinatorEndpoint.sendError(errorMessage);
                            }
                        }
                        else
                        {
                            System.err.println("Commit called on unknown transaction: \"" + commitMessage.targetProtocolService.getExtensionValue(XMLUtil.TRANSACTION_IDENTIFIER_TAG) + "\"");

                            ErrorMessage errorMessage = new ErrorMessage();

                            errorMessage.targetProtocolService = commitMessage.sourceProtocolService;
                            errorMessage.errorCode             = new ErrorCode();
                            errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                            errorMessage.errorCode.name        = XMLUtil.UNKNOWNTRANSACTION_ERROR_CODE;
                            errorMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                            _completionWithAckCoordinatorEndpoint.sendError(errorMessage);
                        }
                    }
                    else if (message instanceof RollbackMessage)
                    {
                        RollbackMessage rollbackMessage = (RollbackMessage) message;

                        com.arjuna.wst.CompletionWithAckCoordinatorParticipant completionWithAckCoordinatorObject = null;

                        synchronized(_completionWithAckCoordinatorObjectMap)
                        {
                            completionWithAckCoordinatorObject = (com.arjuna.wst.CompletionWithAckCoordinatorParticipant) _completionWithAckCoordinatorObjectMap.get(rollbackMessage.targetProtocolService.getExtensionValue(XMLUtil.TRANSACTION_IDENTIFIER_TAG));
                        }

                        if (completionWithAckCoordinatorObject != null)
                        {
                            try
                            {
                                completionWithAckCoordinatorObject.rollback();

                                AbortedMessage abortedMessage = new AbortedMessage();

                                abortedMessage.targetProtocolService = rollbackMessage.sourceProtocolService;
                                abortedMessage.sourceProtocolService = rollbackMessage.targetProtocolService;
                                abortedMessage.dialogIdentifier      = rollbackMessage.dialogIdentifier;

                                _completionWithAckCoordinatorEndpoint.sendAborted(abortedMessage);
                            }
                            catch (UnknownTransactionException unknownTransactionException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = rollbackMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNTRANSACTION_ERROR_CODE;
                                errorMessage.dialogIdentifier      = rollbackMessage.dialogIdentifier;

                                _completionWithAckCoordinatorEndpoint.sendError(errorMessage);
                            }
                            catch (SystemException systemException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = rollbackMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = rollbackMessage.dialogIdentifier;

                                _completionWithAckCoordinatorEndpoint.sendError(errorMessage);
                            }
                            catch (Throwable throwable)
                            {
                                System.err.println("Unexpected exception thrown from rollback: \"" + throwable + "\"");

                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = rollbackMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = rollbackMessage.dialogIdentifier;

                                _completionWithAckCoordinatorEndpoint.sendError(errorMessage);
                            }
                        }
                        else
                        {
                            System.err.println("Rollback called on unknown transaction: \"" + rollbackMessage.targetProtocolService.getExtensionValue(XMLUtil.TRANSACTION_IDENTIFIER_TAG) + "\"");

                            ErrorMessage errorMessage = new ErrorMessage();

                            errorMessage.targetProtocolService = rollbackMessage.sourceProtocolService;
                            errorMessage.errorCode             = new ErrorCode();
                            errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                            errorMessage.errorCode.name        = XMLUtil.UNKNOWNTRANSACTION_ERROR_CODE;
                            errorMessage.dialogIdentifier      = rollbackMessage.dialogIdentifier;

                            _completionWithAckCoordinatorEndpoint.sendError(errorMessage);
                        }
                    }
                    else if (message instanceof NotifiedMessage)
	            {
                        NotifiedMessage notifiedMessage = (NotifiedMessage) message;

                        com.arjuna.wst.CompletionWithAckCoordinatorParticipant completionWithAckCoordinatorObject = null;

                        synchronized(_completionWithAckCoordinatorObjectMap)
                        {
                            completionWithAckCoordinatorObject = (com.arjuna.wst.CompletionWithAckCoordinatorParticipant) _completionWithAckCoordinatorObjectMap.get(notifiedMessage.targetProtocolService.getExtensionValue(XMLUtil.TRANSACTION_IDENTIFIER_TAG));
                        }

                        if (completionWithAckCoordinatorObject != null)
                            try
                            {
                                completionWithAckCoordinatorObject.notified();
                            }
                            catch (Throwable throwable)
                            {
                                System.err.println("Unexpected exception thrown from notified: \"" + throwable + "\"");
                            }
                        else
                            System.err.println("Notified called on unknown transaction: \"" + notifiedMessage.targetProtocolService.getExtensionValue(XMLUtil.TRANSACTION_IDENTIFIER_TAG) + "\"");
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

    private com.arjuna.wst.messaging.CompletionWithAckCoordinator _completionWithAckCoordinatorEndpoint  = null;
    private Map                                                   _completionWithAckCoordinatorObjectMap = new HashMap();
    private Map                                                   _transactionIdentifierMap              = new HashMap();
    private Dispatcher                                            _dispatcher                            = null;
}
