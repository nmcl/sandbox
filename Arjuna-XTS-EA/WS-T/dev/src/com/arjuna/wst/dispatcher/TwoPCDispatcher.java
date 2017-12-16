/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TwoPCDispatcher.java
 */

package com.arjuna.wst.dispatcher;

import java.util.Map;
import java.util.HashMap;
import com.arjuna.wst.SystemException;
import com.arjuna.wst.WrongStateException;
import com.arjuna.wst.TransactionRolledBackException;
import com.arjuna.wst.Vote;
import com.arjuna.wst.Prepared;
import com.arjuna.wst.ReadOnly;
import com.arjuna.wst.Aborted;
import com.arjuna.wst.messaging.ErrorCode;
import com.arjuna.wst.messaging.PrepareMessage;
import com.arjuna.wst.messaging.PreparedMessage;
import com.arjuna.wst.messaging.ReadOnlyMessage;
import com.arjuna.wst.messaging.RollbackMessage;
import com.arjuna.wst.messaging.AbortedMessage;
import com.arjuna.wst.messaging.CommitMessage;
import com.arjuna.wst.messaging.OnePhaseCommitMessage;
import com.arjuna.wst.messaging.CommittedMessage;
import com.arjuna.wst.messaging.ErrorMessage;
import com.arjuna.wst.messaging.UnknownMessage;
import com.arjuna.wst.impl.messaging.XMLUtil;

public class TwoPCDispatcher implements java.io.Serializable
{
    public TwoPCDispatcher(com.arjuna.wst.messaging.TwoPCParticipant twoPCParticipantEndpoint)
        throws Exception
    {
        _twoPCParticipantEndpoint = twoPCParticipantEndpoint;

        _dispatcher = new Dispatcher();
        _dispatcher.start();
    }

    public String getServiceURI()
    {
        return _twoPCParticipantEndpoint.getServiceURI();
    }

    public void activateObject(com.arjuna.wst.TwoPCParticipant twoPCParticipantObject, String transactionIdentifier)
    {
        synchronized(_twoPCParticipantObjectMap)
        {
            _twoPCParticipantObjectMap.put(transactionIdentifier, twoPCParticipantObject);
            _transactionIdentifierMap.put(twoPCParticipantObject, transactionIdentifier);
        }
    }

    public void deactivateObject(com.arjuna.wst.TwoPCParticipant twoPCParticipantObject)
    {
        synchronized(_twoPCParticipantObjectMap)
        {
            String transactionIdentifier = (String) _transactionIdentifierMap.get(twoPCParticipantObject);

            _twoPCParticipantObjectMap.remove(transactionIdentifier);
            _transactionIdentifierMap.remove(twoPCParticipantObject);
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
                    Object message = _twoPCParticipantEndpoint.recv();

                    if (message instanceof PrepareMessage)
	            {
                        PrepareMessage prepareMessage = (PrepareMessage) message;

                        com.arjuna.wst.TwoPCParticipant twoPCParticipantObject = null;

                        synchronized(_twoPCParticipantObjectMap)
                        {
                            twoPCParticipantObject = (com.arjuna.wst.TwoPCParticipant) _twoPCParticipantObjectMap.get(prepareMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
                        }

                        if (twoPCParticipantObject != null)
                        {
                            try
                            {
                                Vote vote = twoPCParticipantObject.prepare();

                                if (vote instanceof Prepared)
                                {
                                    PreparedMessage preparedMessage = new PreparedMessage();

                                    preparedMessage.targetProtocolService = prepareMessage.sourceProtocolService;
                                    preparedMessage.sourceProtocolService = prepareMessage.targetProtocolService;
                                    preparedMessage.dialogIdentifier      = prepareMessage.dialogIdentifier;

                                    _twoPCParticipantEndpoint.sendPrepared(preparedMessage);
                                }
                                else if (vote instanceof ReadOnly)
                                {
                                    ReadOnlyMessage readOnlyMessage = new ReadOnlyMessage();

                                    readOnlyMessage.targetProtocolService = prepareMessage.sourceProtocolService;
                                    readOnlyMessage.sourceProtocolService = prepareMessage.targetProtocolService;
                                    readOnlyMessage.dialogIdentifier      = prepareMessage.dialogIdentifier;

                                    _twoPCParticipantEndpoint.sendReadOnly(readOnlyMessage);
                                }
                                else if (vote instanceof Aborted)
                                {
                                    AbortedMessage abortedMessage = new AbortedMessage();

                                    abortedMessage.targetProtocolService = prepareMessage.sourceProtocolService;
                                    abortedMessage.sourceProtocolService = prepareMessage.targetProtocolService;
                                    abortedMessage.dialogIdentifier      = prepareMessage.dialogIdentifier;

                                    _twoPCParticipantEndpoint.sendAborted(abortedMessage);
                                }
                                else
                                {
                                    ErrorMessage errorMessage = new ErrorMessage();

                                    errorMessage.targetProtocolService = prepareMessage.sourceProtocolService;
                                    errorMessage.errorCode             = new ErrorCode();
                                    errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                    errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                    errorMessage.dialogIdentifier      = prepareMessage.dialogIdentifier;

                                    _twoPCParticipantEndpoint.sendError(errorMessage);
                                }
                            }
                            catch (WrongStateException wrongStateException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = prepareMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.WRONGSTATE_ERROR_CODE;
                                errorMessage.dialogIdentifier      = prepareMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendError(errorMessage);
                            }
                            catch (SystemException systemException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = prepareMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = prepareMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendError(errorMessage);
                            }
                            catch (Throwable throwable)
                            {
                                System.err.println("Unexpected exception thrown from prepare: \"" + throwable + "\"");

                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = prepareMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = prepareMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendError(errorMessage);
                            }
                        }
                        else
                        {
                            System.err.println("Prepare called on unknown participant: \"" + prepareMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG) + "\"");

                            UnknownMessage unknownMessage = new UnknownMessage();

                            unknownMessage.targetProtocolService = prepareMessage.sourceProtocolService;
                            unknownMessage.sourceProtocolService = prepareMessage.targetProtocolService;
                            unknownMessage.dialogIdentifier      = prepareMessage.dialogIdentifier;

                            _twoPCParticipantEndpoint.sendUnknown(unknownMessage);
                        }
		    }
                    else if (message instanceof CommitMessage)
	            {
                        CommitMessage commitMessage = (CommitMessage) message;

                        com.arjuna.wst.TwoPCParticipant twoPCParticipantObject = null;

                        synchronized(_twoPCParticipantObjectMap)
                        {
                            twoPCParticipantObject = (com.arjuna.wst.TwoPCParticipant) _twoPCParticipantObjectMap.get(commitMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
                        }

                        if (twoPCParticipantObject != null)
                        {
                            try
                            {
                                twoPCParticipantObject.commit();

                                CommittedMessage committedMessage = new CommittedMessage();

                                committedMessage.targetProtocolService = commitMessage.sourceProtocolService;
                                committedMessage.sourceProtocolService = commitMessage.targetProtocolService;
                                committedMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendCommitted(committedMessage);
                            }
                            catch (WrongStateException wrongStateException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = commitMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.WRONGSTATE_ERROR_CODE;
                                errorMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendError(errorMessage);
                            }
                            catch (SystemException systemException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = commitMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendError(errorMessage);
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

                                _twoPCParticipantEndpoint.sendError(errorMessage);
                            }
                        }
                        else
                        {
                            System.err.println("Commit called on unknown participant: \"" + commitMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG) + "\"");

                            UnknownMessage unknownMessage = new UnknownMessage();

                            unknownMessage.targetProtocolService = commitMessage.sourceProtocolService;
                            unknownMessage.sourceProtocolService = commitMessage.targetProtocolService;
                            unknownMessage.dialogIdentifier      = commitMessage.dialogIdentifier;

                            _twoPCParticipantEndpoint.sendUnknown(unknownMessage);
                        }
		    }
                    else if (message instanceof RollbackMessage)
	            {
                        RollbackMessage rollbackMessage = (RollbackMessage) message;

                        com.arjuna.wst.TwoPCParticipant twoPCParticipantObject = null;

                        synchronized(_twoPCParticipantObjectMap)
                        {
                            twoPCParticipantObject = (com.arjuna.wst.TwoPCParticipant) _twoPCParticipantObjectMap.get(rollbackMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
                        }

                        if (twoPCParticipantObject != null)
                        {
                            try
                            {
                                twoPCParticipantObject.rollback();

                                AbortedMessage abortedMessage = new AbortedMessage();

                                abortedMessage.targetProtocolService = rollbackMessage.sourceProtocolService;
                                abortedMessage.sourceProtocolService = rollbackMessage.targetProtocolService;
                                abortedMessage.dialogIdentifier      = rollbackMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendAborted(abortedMessage);
                            }
                            catch (WrongStateException wrongStateException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = rollbackMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.WRONGSTATE_ERROR_CODE;
                                errorMessage.dialogIdentifier      = rollbackMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendError(errorMessage);
                            }
                            catch (SystemException systemException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = rollbackMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = rollbackMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendError(errorMessage);
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

                                _twoPCParticipantEndpoint.sendError(errorMessage);
                            }
                        }
                        else
                        {
                            System.err.println("Rollback called on unknown participant: \"" + rollbackMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG) + "\"");

                            UnknownMessage unknownMessage = new UnknownMessage();

                            unknownMessage.targetProtocolService = rollbackMessage.sourceProtocolService;
                            unknownMessage.sourceProtocolService = rollbackMessage.targetProtocolService;
                            unknownMessage.dialogIdentifier      = rollbackMessage.dialogIdentifier;

                            _twoPCParticipantEndpoint.sendUnknown(unknownMessage);
                        }
		    }
                    else if (message instanceof OnePhaseCommitMessage)
	            {
                        OnePhaseCommitMessage onePhaseCommitMessage = (OnePhaseCommitMessage) message;

                        com.arjuna.wst.TwoPCParticipant twoPCParticipantObject = null;

                        synchronized(_twoPCParticipantObjectMap)
                        {
                            twoPCParticipantObject = (com.arjuna.wst.TwoPCParticipant) _twoPCParticipantObjectMap.get(onePhaseCommitMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
                        }

                        if (twoPCParticipantObject != null)
                        {
                            try
                            {
                                twoPCParticipantObject.commitOnePhase();

                                CommittedMessage committedMessage = new CommittedMessage();

                                committedMessage.targetProtocolService = onePhaseCommitMessage.sourceProtocolService;
                                committedMessage.sourceProtocolService = onePhaseCommitMessage.targetProtocolService;
                                committedMessage.dialogIdentifier      = onePhaseCommitMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendCommitted(committedMessage);
                            }
                            catch (TransactionRolledBackException transactionRolledBackException)
                            {
                                AbortedMessage abortedMessage = new AbortedMessage();

                                abortedMessage.targetProtocolService = onePhaseCommitMessage.sourceProtocolService;
                                abortedMessage.sourceProtocolService = onePhaseCommitMessage.targetProtocolService;
                                abortedMessage.dialogIdentifier      = onePhaseCommitMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendAborted(abortedMessage);
                            }
                            catch (WrongStateException wrongStateException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = onePhaseCommitMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.WRONGSTATE_ERROR_CODE;
                                errorMessage.dialogIdentifier      = onePhaseCommitMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendError(errorMessage);
                            }
                            catch (SystemException systemException)
                            {
                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = onePhaseCommitMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = onePhaseCommitMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendError(errorMessage);
                            }
                            catch (Throwable throwable)
                            {
                                System.err.println("Unexpected exception thrown from commitOnePhase: \"" + throwable + "\"");

                                ErrorMessage errorMessage = new ErrorMessage();

                                errorMessage.targetProtocolService = onePhaseCommitMessage.sourceProtocolService;
                                errorMessage.errorCode             = new ErrorCode();
                                errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
                                errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                                errorMessage.dialogIdentifier      = onePhaseCommitMessage.dialogIdentifier;

                                _twoPCParticipantEndpoint.sendError(errorMessage);
                            }
                        }
                        else
                        {
                            System.err.println("OnePhaseCommit called on unknown participant: \"" + onePhaseCommitMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG) + "\"");

                            UnknownMessage unknownMessage = new UnknownMessage();

                            unknownMessage.targetProtocolService = onePhaseCommitMessage.sourceProtocolService;
                            unknownMessage.sourceProtocolService = onePhaseCommitMessage.targetProtocolService;
                            unknownMessage.dialogIdentifier      = onePhaseCommitMessage.dialogIdentifier;

                            _twoPCParticipantEndpoint.sendUnknown(unknownMessage);
                        }
		    }
                    else if (message instanceof UnknownMessage)
	            {
                        UnknownMessage unknownMessage = (UnknownMessage) message;

                        com.arjuna.wst.TwoPCParticipant twoPCParticipantObject = null;

                        synchronized(_twoPCParticipantObjectMap)
                        {
                            twoPCParticipantObject = (com.arjuna.wst.TwoPCParticipant) _twoPCParticipantObjectMap.get(unknownMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
                        }

                        if (twoPCParticipantObject != null)
                            try
                            {
                                twoPCParticipantObject.unknown();
                            }
                            catch (Throwable throwable)
                            {
                                System.err.println("Unexpected exception thrown from unknown: \"" + throwable + "\"");
                            }
                        else
                            System.err.println("Unknown called on unknown participant: \"" + unknownMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG) + "\"");
		    }
                    else if (message instanceof ErrorMessage)
	            {
                        ErrorMessage errorMessage = (ErrorMessage) message;

                        com.arjuna.wst.TwoPCParticipant twoPCParticipantObject = null;

                        synchronized(_twoPCParticipantObjectMap)
                        {
                            twoPCParticipantObject = (com.arjuna.wst.TwoPCParticipant) _twoPCParticipantObjectMap.get(errorMessage.targetProtocolService.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
                        }

                        if (twoPCParticipantObject != null)
                            try
                            {
                                twoPCParticipantObject.error();
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

    private com.arjuna.wst.messaging.TwoPCParticipant _twoPCParticipantEndpoint  = null;
    private Map                                       _twoPCParticipantObjectMap = new HashMap();
    private Map                                       _transactionIdentifierMap  = new HashMap();
    private Dispatcher                                _dispatcher                = null;
}
