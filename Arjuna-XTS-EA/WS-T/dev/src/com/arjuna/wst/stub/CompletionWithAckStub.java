/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionWithAckStub.java
 */

package com.arjuna.wst.stub;

import java.util.Map;
import java.util.HashMap;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.SystemException;
import com.arjuna.wst.UnknownTransactionException;
import com.arjuna.wst.TransactionRolledBackException;
import com.arjuna.wst.messaging.CommitMessage;
import com.arjuna.wst.messaging.CommittedMessage;
import com.arjuna.wst.messaging.RollbackMessage;
import com.arjuna.wst.messaging.AbortedMessage;
import com.arjuna.wst.messaging.NotifiedMessage;
import com.arjuna.wst.messaging.ErrorMessage;
import com.arjuna.wst.messaging.UnknownMessage;
import com.arjuna.wst.messaging.CompletionWithAckParticipant;
import com.arjuna.wst.impl.messaging.XMLUtil;

public class CompletionWithAckStub implements com.arjuna.wst.CompletionWithAckCoordinatorParticipant
{
    public CompletionWithAckStub(CompletionWithAckParticipant completionWithAckParticipantEndpoint, PortReference completionWithAckCoordinator)
        throws Exception
    {
        _completionWithAckParticipantEndpoint = completionWithAckParticipantEndpoint;
        _completionWithAckParticipant         = new PortReference(_completionWithAckParticipantEndpoint.getServiceURI());
        _completionWithAckParticipant.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, completionWithAckCoordinator.getExtensionValue(XMLUtil.TRANSACTION_IDENTIFIER_TAG));
        _completionWithAckCoordinator         = completionWithAckCoordinator;
    }

    public void commit()
        throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
        CommitMessage commitMessage   = new CommitMessage();
        Object        receivedMessage = null;

        commitMessage.targetProtocolService = _completionWithAckCoordinator;
        commitMessage.sourceProtocolService = _completionWithAckParticipant;
        commitMessage.dialogIdentifier      = Long.toString(System.currentTimeMillis());

        try
        {
            _completionWithAckParticipantEndpoint.sendCommit(commitMessage);

            receivedMessage = _completionWithAckParticipantEndpoint.recv(commitMessage.dialogIdentifier);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }

        if (receivedMessage instanceof CommittedMessage)
        {
            // Check transactionIdentifier?
        }
        else if (receivedMessage instanceof AbortedMessage)
            throw new TransactionRolledBackException();
        else if (receivedMessage instanceof UnknownMessage)
            throw new UnknownTransactionException();
        else if (receivedMessage instanceof ErrorMessage)
        {
            ErrorMessage errorMessage = (ErrorMessage) receivedMessage;

            if (XMLUtil.WSARJTX_NAMESPACE_URI.equals(errorMessage.errorCode.namespace) && XMLUtil.UNKNOWNTRANSACTION_ERROR_CODE.equals(errorMessage.errorCode.name))
                throw new UnknownTransactionException();
            else
                throw new SystemException();
        }
        else
            throw new SystemException();
    }

    public void rollback()
        throws UnknownTransactionException, SystemException
    {
        RollbackMessage rollbackMessage = new RollbackMessage();
        Object          receivedMessage = null;

        rollbackMessage.targetProtocolService = _completionWithAckCoordinator;
        rollbackMessage.sourceProtocolService = _completionWithAckParticipant;
        rollbackMessage.dialogIdentifier      = Long.toString(System.currentTimeMillis());

        try
        {
            _completionWithAckParticipantEndpoint.sendRollback(rollbackMessage);

            receivedMessage = _completionWithAckParticipantEndpoint.recv(rollbackMessage.dialogIdentifier);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }

        if (receivedMessage instanceof AbortedMessage)
        {
            // Check transactionIdentifier?
        }
        else if (receivedMessage instanceof UnknownMessage)
            throw new UnknownTransactionException();
        else if (receivedMessage instanceof ErrorMessage)
        {
            ErrorMessage errorMessage = (ErrorMessage) receivedMessage;

            if (XMLUtil.WSARJTX_NAMESPACE_URI.equals(errorMessage.errorCode.namespace) && XMLUtil.UNKNOWNTRANSACTION_ERROR_CODE.equals(errorMessage.errorCode.name))
                throw new UnknownTransactionException();
            else
                throw new SystemException();
        }
        else
            throw new SystemException();
    }

    public void notified()
        throws UnknownTransactionException, SystemException
    {
        NotifiedMessage notifiedMessage   = new NotifiedMessage();

        notifiedMessage.targetProtocolService = _completionWithAckCoordinator;
        notifiedMessage.sourceProtocolService = _completionWithAckParticipant;
        notifiedMessage.dialogIdentifier      = Long.toString(System.currentTimeMillis());

        try
        {
            _completionWithAckParticipantEndpoint.sendNotified(notifiedMessage);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }
    }

    private CompletionWithAckParticipant _completionWithAckParticipantEndpoint = null;
    private PortReference                _completionWithAckParticipant         = null;
    private PortReference                _completionWithAckCoordinator         = null;
}
