/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionStub.java
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
import com.arjuna.wst.messaging.ErrorMessage;
import com.arjuna.wst.messaging.UnknownMessage;
import com.arjuna.wst.messaging.CompletionParticipant;
import com.arjuna.wst.impl.messaging.XMLUtil;

public class CompletionStub implements com.arjuna.wst.CompletionCoordinatorParticipant
{
    public CompletionStub(CompletionParticipant completionParticipantEndpoint, PortReference completionCoordinator)
        throws Exception
    {
        _completionParticipantEndpoint = completionParticipantEndpoint;
        _completionParticipant         = new PortReference(_completionParticipantEndpoint.getServiceURI());
        _completionParticipant.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, completionCoordinator.getExtensionValue(XMLUtil.TRANSACTION_IDENTIFIER_TAG));
        _completionCoordinator         = completionCoordinator;
    }

    public void commit()
        throws TransactionRolledBackException, UnknownTransactionException, SystemException
    {
        CommitMessage commitMessage   = new CommitMessage();
        Object        receivedMessage = null;

        commitMessage.targetProtocolService = _completionCoordinator;
        commitMessage.sourceProtocolService = _completionParticipant;
        commitMessage.dialogIdentifier      = Long.toString(System.currentTimeMillis());

        try
        {
            _completionParticipantEndpoint.sendCommit(commitMessage);

            receivedMessage = _completionParticipantEndpoint.recv(commitMessage.dialogIdentifier);
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

        rollbackMessage.targetProtocolService = _completionCoordinator;
        rollbackMessage.sourceProtocolService = _completionParticipant;
        rollbackMessage.dialogIdentifier      = Long.toString(System.currentTimeMillis());

        try
        {
            _completionParticipantEndpoint.sendRollback(rollbackMessage);

            receivedMessage = _completionParticipantEndpoint.recv(rollbackMessage.dialogIdentifier);
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

    private CompletionParticipant _completionParticipantEndpoint = null;
    private PortReference         _completionParticipant         = null;
    private PortReference         _completionCoordinator         = null;
}
