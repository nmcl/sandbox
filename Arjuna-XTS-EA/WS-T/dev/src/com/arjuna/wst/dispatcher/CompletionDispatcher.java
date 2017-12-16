/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionDispatcher.java
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
import com.arjuna.wst.messaging.ErrorMessage;
import com.arjuna.wst.impl.messaging.XMLUtil;

public class CompletionDispatcher implements java.io.Serializable
{
    public CompletionDispatcher(com.arjuna.wst.messaging.CompletionCoordinator completionCoordinatorEndpoint)
        throws Exception
    {
        _completionCoordinatorEndpoint = completionCoordinatorEndpoint;

        _dispatcher = new Dispatcher();
        _dispatcher.start();
    }

    public String getServiceURI()
    {
        return _completionCoordinatorEndpoint.getServiceURI();
    }

    public void activateObject(com.arjuna.wst.CompletionCoordinatorParticipant completionCoordinatorObject, String transactionIdentifier)
    {
        synchronized(_completionCoordinatorObjectMap)
        {
            _completionCoordinatorObjectMap.put(transactionIdentifier, completionCoordinatorObject);
            _transactionIdentifierMap.put(completionCoordinatorObject, transactionIdentifier);
        }
    }

    public void deactivateObject(com.arjuna.wst.CompletionCoordinatorParticipant completionCoordinatorObject)
    {
        synchronized(_completionCoordinatorObjectMap)
        {
            String transactionIdentifier = (String) _transactionIdentifierMap.get(completionCoordinatorObject);

            _completionCoordinatorObjectMap.remove(transactionIdentifier);
            _transactionIdentifierMap.remove(completionCoordinatorObject);
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
                    Object message = _completionCoordinatorEndpoint.recv();

                    if (message instanceof CommitMessage)
	            {
                        CommitMessage commitMessage = (CommitMessage) message;

                        com.arjuna.wst.CompletionCoordinatorParticipant completionCoordinatorObject = null;

                        synchronized(_completionCoordinatorObjectMap)
                        {
                            completionCoordinatorObject = (com.arjuna.wst.CompletionCoordinatorParticipant) _completionCoordinatorObjectMap.get(commitMessage.targetProtocolService.getExtensionValue(XMLUtil.TRANSACTION_IDENTIFIER_TAG));
                        }

                        if (completionCoordinatorObject != null)
                        {
                            try
                            {
                                completionCoordinatorObject.commit();

                                CommittedMessage committedMessage = new CommittedMessage();

                                committedMessage.targetProtocolService = commitMessage.sourceProtocolService;
                                committedMessage.sourceProtocolService = commitMessage.targetProtocolService;
                                committedMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                                _completionCoordinatorEndpoint.sendCommitted(committedMessage);
                            }
                            catch (TransactionRolledBackException transactionRolledBackException)
                            {
                                AbortedMessage abortedMessage = new AbortedMessage();

                                abortedMessage.targetProtocolService = commitMessage.sourceProtocolService;
                                abortedMessage.sourceProtocolService = commitMessage.targetProtocolService;
                                abortedMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                                _completionCoordinatorEndpoint.sendAborted(abortedMessage);
                            }
                            catch (UnknownTransactionException unknownTransactionException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = commitMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNTRANSACTION_ERROR_CODE;
                                errorMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                                _completionCoordinatorEndpoint.sendError(errorMessage);
                            }
                            catch (SystemException systemException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = commitMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                                _completionCoordinatorEndpoint.sendError(errorMessage);
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

                                _completionCoordinatorEndpoint.sendError(errorMessage);
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

                            _completionCoordinatorEndpoint.sendError(errorMessage);
                        }
                    }
                    else if (message instanceof RollbackMessage)
                    {
                        RollbackMessage rollbackMessage = (RollbackMessage) message;

                        com.arjuna.wst.CompletionCoordinatorParticipant completionCoordinatorObject = null;

                        synchronized(_completionCoordinatorObjectMap)
                        {
                            completionCoordinatorObject = (com.arjuna.wst.CompletionCoordinatorParticipant) _completionCoordinatorObjectMap.get(rollbackMessage.targetProtocolService.getExtensionValue(XMLUtil.TRANSACTION_IDENTIFIER_TAG));
                        }

                        if (completionCoordinatorObject != null)
                        {
                            try
                            {
                                completionCoordinatorObject.rollback();

                                AbortedMessage abortedMessage = new AbortedMessage();

                                abortedMessage.targetProtocolService = rollbackMessage.sourceProtocolService;
                                abortedMessage.sourceProtocolService = rollbackMessage.targetProtocolService;
                                abortedMessage.dialogIdentifier      = rollbackMessage.dialogIdentifier;

                                _completionCoordinatorEndpoint.sendAborted(abortedMessage);
                            }
                            catch (UnknownTransactionException unknownTransactionException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = rollbackMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNTRANSACTION_ERROR_CODE;
                                errorMessage.dialogIdentifier      = rollbackMessage.dialogIdentifier;

                                _completionCoordinatorEndpoint.sendError(errorMessage);
                            }
                            catch (SystemException systemException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = rollbackMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = rollbackMessage.dialogIdentifier;

                                _completionCoordinatorEndpoint.sendError(errorMessage);
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

                                _completionCoordinatorEndpoint.sendError(errorMessage);
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

                            _completionCoordinatorEndpoint.sendError(errorMessage);
                        }
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

    private com.arjuna.wst.messaging.CompletionCoordinator _completionCoordinatorEndpoint  = null;
    private Map                                            _completionCoordinatorObjectMap = new HashMap();
    private Map                                            _transactionIdentifierMap       = new HashMap();
    private Dispatcher                                     _dispatcher                     = null;
}
