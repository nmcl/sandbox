/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * PhaseZeroStub.java
 */

package com.arjuna.wst.stub;

import java.util.Map;
import java.util.HashMap;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.SystemException;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;

public class PhaseZeroStub implements com.arjuna.wst.PhaseZeroParticipant
{
    public PhaseZeroStub(PhaseZeroCoordinator phaseZeroCoordinatorEndpoint, PortReference phaseZeroParticipant)
        throws Exception
    {
        _phaseZeroCoordinatorEndpoint = phaseZeroCoordinatorEndpoint;
        _phaseZeroCoordinator         = new PortReference(_phaseZeroCoordinatorEndpoint.getServiceURI());
        _phaseZeroCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, phaseZeroParticipant.getExtensionValue(XMLUtil.PARTICIPANT_IDENTIFIER_TAG));
        _phaseZeroParticipant         = phaseZeroParticipant;
    }

    public void phaseZero()
        throws SystemException
    {
        PhaseZeroMessage phaseZeroMessage = new PhaseZeroMessage();
        Object           receivedMessage  = null;

        phaseZeroMessage.targetProtocolService = _phaseZeroParticipant;
        phaseZeroMessage.sourceProtocolService = _phaseZeroCoordinator;
        phaseZeroMessage.dialogIdentifier      = Long.toString(System.currentTimeMillis());

        try
        {
            _phaseZeroCoordinatorEndpoint.sendPhaseZero(phaseZeroMessage);

            receivedMessage = _phaseZeroCoordinatorEndpoint.recv(phaseZeroMessage.dialogIdentifier);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }

        if (receivedMessage instanceof PhaseZeroCompletedMessage)
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

        errorMessage.targetProtocolService = _phaseZeroParticipant;
        errorMessage.errorCode             = new ErrorCode();
        errorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        errorMessage.dialogIdentifier      = null;

        try
        {
            _phaseZeroCoordinatorEndpoint.sendError(errorMessage);
        }
        catch (Exception exception)
        {
            throw new SystemException();
        }
    }

    private PhaseZeroCoordinator _phaseZeroCoordinatorEndpoint = null;
    private PortReference        _phaseZeroCoordinator         = null;
    private PortReference        _phaseZeroParticipant         = null;
}
