/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * OutcomeNotificationStub.java
 */

package com.arjuna.wst.stub;

import java.util.Map;
import java.util.HashMap;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.SystemException;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;

public class OutcomeNotificationStub implements com.arjuna.wst.OutcomeNotificationParticipant
{
    public OutcomeNotificationStub(OutcomeNotificationCoordinator outcomeNotificationCoordinatorEndpoint, PortReference outcomeNotificationParticipant)
        throws Exception
    {
        _outcomeNotificationCoordinatorEndpoint = outcomeNotificationCoordinatorEndpoint;
        _outcomeNotificationCoordinator         = new PortReference(_outcomeNotificationCoordinatorEndpoint.getServiceURI());
        _outcomeNotificationCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, outcomeNotificationParticipant.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
        _outcomeNotificationParticipant         = outcomeNotificationParticipant;
    }

    public void committed()
        throws SystemException
    {
        CommittedMessage committedMessage = new CommittedMessage();
        Object           receivedMessage  = null;

        committedMessage.targetProtocolService = _outcomeNotificationParticipant;
        committedMessage.sourceProtocolService = _outcomeNotificationCoordinator;
        committedMessage.dialogIdentifier      = Long.toString(System.currentTimeMillis());

        try
        {
            _outcomeNotificationCoordinatorEndpoint.sendCommitted(committedMessage);

            receivedMessage = _outcomeNotificationCoordinatorEndpoint.recv(committedMessage.dialogIdentifier);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }

        if (receivedMessage instanceof NotifiedMessage)
        {
            // Check transactionIdentifier?
        }
        else if (receivedMessage instanceof ErrorMessage)
            throw new SystemException();
        else
            throw new SystemException();
    }

    public void aborted()
        throws SystemException
    {
        AbortedMessage abortedMessage   = new AbortedMessage();
        Object         receivedMessage  = null;

        abortedMessage.targetProtocolService = _outcomeNotificationParticipant;
        abortedMessage.sourceProtocolService = _outcomeNotificationCoordinator;
        abortedMessage.dialogIdentifier      = Long.toString(System.currentTimeMillis());

        try
        {
            _outcomeNotificationCoordinatorEndpoint.sendAborted(abortedMessage);

            receivedMessage = _outcomeNotificationCoordinatorEndpoint.recv(abortedMessage.dialogIdentifier);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }

        if (receivedMessage instanceof NotifiedMessage)
        {
            // Check transactionIdentifier?
        }
        else if (receivedMessage instanceof ErrorMessage)
            throw new SystemException();
        else
            throw new SystemException();
    }

    public void error()
        throws SystemException
    {
        ErrorMessage errorMessage = new ErrorMessage();

        errorMessage.targetProtocolService = _outcomeNotificationParticipant;
        errorMessage.errorCode             = new ErrorCode();
        errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        errorMessage.dialogIdentifier      = null;

        try
        {
            _outcomeNotificationCoordinatorEndpoint.sendError(errorMessage);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }
    }

    public void unknown()
        throws SystemException
    {
        UnknownMessage unknownMessage = new UnknownMessage();

        unknownMessage.targetProtocolService = _outcomeNotificationParticipant;
        unknownMessage.sourceProtocolService = _outcomeNotificationCoordinator;
        unknownMessage.dialogIdentifier      = null;

        try
        {
            _outcomeNotificationCoordinatorEndpoint.sendUnknown(unknownMessage);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }
    }

    private OutcomeNotificationCoordinator _outcomeNotificationCoordinatorEndpoint = null;
    private PortReference                  _outcomeNotificationCoordinator         = null;
    private PortReference                  _outcomeNotificationParticipant         = null;
}
