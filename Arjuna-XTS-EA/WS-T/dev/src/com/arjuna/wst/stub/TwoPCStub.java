/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TwoPCStub.java
 */

package com.arjuna.wst.stub;

import java.util.Map;
import java.util.HashMap;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.SystemException;
import com.arjuna.wst.WrongStateException;
import com.arjuna.wst.TransactionRolledBackException;
import com.arjuna.wst.Vote;
import com.arjuna.wst.Prepared;
import com.arjuna.wst.ReadOnly;
import com.arjuna.wst.Aborted;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;

public class TwoPCStub implements com.arjuna.wst.TwoPCParticipant
{
    public TwoPCStub(TwoPCCoordinator twoPCCoordinatorEndpoint, PortReference twoPCParticipant)
        throws Exception
    {
        _twoPCCoordinatorEndpoint = twoPCCoordinatorEndpoint;
        _twoPCCoordinator         = new PortReference(_twoPCCoordinatorEndpoint.getServiceURI());
        _twoPCCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, twoPCParticipant.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
        _twoPCParticipant         = twoPCParticipant;
    }

    public Vote prepare()
        throws WrongStateException, SystemException
    {
        PrepareMessage prepareMessage  = new PrepareMessage();
        Object         receivedMessage = null;

        prepareMessage.targetProtocolService = _twoPCParticipant;
        prepareMessage.sourceProtocolService = _twoPCCoordinator;
        prepareMessage.dialogIdentifier      = Long.toString(System.currentTimeMillis());

        try
        {
            _twoPCCoordinatorEndpoint.sendPrepare(prepareMessage);

            receivedMessage = _twoPCCoordinatorEndpoint.recv(prepareMessage.dialogIdentifier);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }

        if (receivedMessage instanceof PreparedMessage)
            return new Prepared();
        else if (receivedMessage instanceof ReadOnlyMessage)
            return new ReadOnly();
        else if (receivedMessage instanceof AbortedMessage)
            return new Aborted();
        else if (receivedMessage instanceof ErrorMessage)
        {
            ErrorMessage errorMessage = (ErrorMessage) receivedMessage;

            if (XMLUtil.WSARJTX_NAMESPACE_URI.equals(errorMessage.errorCode.namespace) && XMLUtil.WRONGSTATE_ERROR_CODE.equals(errorMessage.errorCode.name))
                throw new WrongStateException();
            else
                throw new SystemException();
        }
        else
            throw new SystemException();
    }

    public void commit()
        throws WrongStateException, SystemException
    {
        CommitMessage commitMessage   = new CommitMessage();
        Object        receivedMessage = null;

        commitMessage.targetProtocolService = _twoPCParticipant;
        commitMessage.sourceProtocolService = _twoPCCoordinator;
        commitMessage.dialogIdentifier      = Long.toString(System.currentTimeMillis());

        try
        {
            _twoPCCoordinatorEndpoint.sendCommit(commitMessage);

            receivedMessage = _twoPCCoordinatorEndpoint.recv(commitMessage.dialogIdentifier);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }

        if (receivedMessage instanceof CommittedMessage)
        {
            // Check transactionIdentifier?
        }
        else if (receivedMessage instanceof ErrorMessage)
        {
            ErrorMessage errorMessage = (ErrorMessage) receivedMessage;

            if (XMLUtil.WSARJTX_NAMESPACE_URI.equals(errorMessage.errorCode.namespace) && XMLUtil.WRONGSTATE_ERROR_CODE.equals(errorMessage.errorCode.name))
                throw new WrongStateException();
            else
                throw new SystemException();
        }
        else
            throw new SystemException();
    }

    public void rollback()
        throws WrongStateException, SystemException
    {
        RollbackMessage rollbackMessage = new RollbackMessage();
        Object          receivedMessage = null;

        rollbackMessage.targetProtocolService = _twoPCParticipant;
        rollbackMessage.sourceProtocolService = _twoPCCoordinator;
        rollbackMessage.dialogIdentifier      = Long.toString(System.currentTimeMillis());

        try
        {
            _twoPCCoordinatorEndpoint.sendRollback(rollbackMessage);

            receivedMessage = _twoPCCoordinatorEndpoint.recv(rollbackMessage.dialogIdentifier);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }

        if (receivedMessage instanceof AbortedMessage)
        {
            // Check transactionIdentifier?
        }
        else if (receivedMessage instanceof ErrorMessage)
        {
            ErrorMessage errorMessage = (ErrorMessage) receivedMessage;

            if (XMLUtil.WSARJTX_NAMESPACE_URI.equals(errorMessage.errorCode.namespace) && XMLUtil.WRONGSTATE_ERROR_CODE.equals(errorMessage.errorCode.name))
                throw new WrongStateException();
            else
                throw new SystemException();
        }
       else
            throw new SystemException();
    }

    public void commitOnePhase()
        throws TransactionRolledBackException, WrongStateException, SystemException
    {
        OnePhaseCommitMessage onePhaseCommitMessage = new OnePhaseCommitMessage();
        Object                receivedMessage       = null;

        onePhaseCommitMessage.targetProtocolService = _twoPCParticipant;
        onePhaseCommitMessage.sourceProtocolService = _twoPCCoordinator;
        onePhaseCommitMessage.dialogIdentifier      = Long.toString(System.currentTimeMillis());

        try
        {
            _twoPCCoordinatorEndpoint.sendOnePhaseCommit(onePhaseCommitMessage);

            receivedMessage = _twoPCCoordinatorEndpoint.recv(onePhaseCommitMessage.dialogIdentifier);
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
        else if (receivedMessage instanceof ErrorMessage)
        {
            ErrorMessage errorMessage = (ErrorMessage) receivedMessage;

            if (XMLUtil.WSARJTX_NAMESPACE_URI.equals(errorMessage.errorCode.namespace) && XMLUtil.WRONGSTATE_ERROR_CODE.equals(errorMessage.errorCode.name))
                throw new WrongStateException();
            else
                throw new SystemException();
        }
        else
            throw new SystemException();
    }

    public void unknown()
        throws SystemException
    {
        UnknownMessage unknownMessage = new UnknownMessage();

        unknownMessage.targetProtocolService = _twoPCParticipant;
        unknownMessage.sourceProtocolService = _twoPCCoordinator;
        unknownMessage.dialogIdentifier      = null;

        try
        {
            _twoPCCoordinatorEndpoint.sendUnknown(unknownMessage);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }
    }

    public void error()
        throws SystemException
    {
        ErrorMessage errorMessage = new ErrorMessage();

        errorMessage.targetProtocolService = _twoPCParticipant;
        errorMessage.errorCode             = new ErrorCode();
        errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        errorMessage.dialogIdentifier      = null;

        try
        {
            _twoPCCoordinatorEndpoint.sendError(errorMessage);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }
    }

    private TwoPCCoordinator _twoPCCoordinatorEndpoint = null;
    private PortReference    _twoPCCoordinator         = null;
    private PortReference    _twoPCParticipant         = null;
}
