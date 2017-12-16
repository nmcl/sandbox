/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * PhaseZeroParticipantTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class PhaseZeroParticipantTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String phaseZeroParticipantJNDIName = "phasezeroparticipant-msg";
        String phaseZeroCoordinatorJNDIName = "phasezerocoordinator";

        _phaseZeroParticipant           = (PhaseZeroParticipant) initialContext.lookup(phaseZeroParticipantJNDIName);
        _phaseZeroParticipantServiceURI = _phaseZeroParticipant.getServiceURI();
        _phaseZeroCoordinator           = (PhaseZeroCoordinator) initialContext.lookup(phaseZeroCoordinatorJNDIName);
        _phaseZeroCoordinatorServiceURI = _phaseZeroCoordinator.getServiceURI();
    }

    public void testSendPhaseZeroCompletedWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        PhaseZeroCompletedMessage sentPhaseZeroCompletedMessage     = new PhaseZeroCompletedMessage();
        PhaseZeroCompletedMessage receivedPhaseZeroCompletedMessage = null;

        sentPhaseZeroCompletedMessage.targetProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentPhaseZeroCompletedMessage.sourceProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentPhaseZeroCompletedMessage.dialogIdentifier      = null;

        _phaseZeroParticipant.sendPhaseZeroCompleted(sentPhaseZeroCompletedMessage);

        receivedPhaseZeroCompletedMessage = (PhaseZeroCompletedMessage) _phaseZeroCoordinator.recv();

        assertEquals(sentPhaseZeroCompletedMessage.targetProtocolService.getAddress(), receivedPhaseZeroCompletedMessage.targetProtocolService.getAddress());
        assertEquals(sentPhaseZeroCompletedMessage.sourceProtocolService.getAddress(), receivedPhaseZeroCompletedMessage.sourceProtocolService.getAddress());
        assertEquals(sentPhaseZeroCompletedMessage.dialogIdentifier, receivedPhaseZeroCompletedMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _phaseZeroParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _phaseZeroCoordinator.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _phaseZeroParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _phaseZeroCoordinator.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendPhaseZeroCompletedWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        PhaseZeroCompletedMessage sentPhaseZeroCompletedMessage     = new PhaseZeroCompletedMessage();
        PhaseZeroCompletedMessage receivedPhaseZeroCompletedMessage = null;

        sentPhaseZeroCompletedMessage.targetProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentPhaseZeroCompletedMessage.sourceProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentPhaseZeroCompletedMessage.dialogIdentifier      = "123456";

        _phaseZeroParticipant.sendPhaseZeroCompleted(sentPhaseZeroCompletedMessage);

        receivedPhaseZeroCompletedMessage = (PhaseZeroCompletedMessage) _phaseZeroCoordinator.recv();

        assertEquals(sentPhaseZeroCompletedMessage.targetProtocolService.getAddress(), receivedPhaseZeroCompletedMessage.targetProtocolService.getAddress());
        assertEquals(sentPhaseZeroCompletedMessage.sourceProtocolService.getAddress(), receivedPhaseZeroCompletedMessage.sourceProtocolService.getAddress());
        assertEquals(sentPhaseZeroCompletedMessage.dialogIdentifier, receivedPhaseZeroCompletedMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _phaseZeroParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _phaseZeroCoordinator.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _phaseZeroParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _phaseZeroCoordinator.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendPhaseZeroCompletedWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        PhaseZeroCompletedMessage sentPhaseZeroCompletedMessage     = new PhaseZeroCompletedMessage();
        PhaseZeroCompletedMessage receivedPhaseZeroCompletedMessage = null;

        sentPhaseZeroCompletedMessage.targetProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentPhaseZeroCompletedMessage.sourceProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentPhaseZeroCompletedMessage.dialogIdentifier      = null;

        _phaseZeroParticipant.sendPhaseZeroCompleted(sentPhaseZeroCompletedMessage);

        receivedPhaseZeroCompletedMessage = (PhaseZeroCompletedMessage) _phaseZeroCoordinator.recv(sentPhaseZeroCompletedMessage.dialogIdentifier);

        assertEquals(sentPhaseZeroCompletedMessage.targetProtocolService.getAddress(), receivedPhaseZeroCompletedMessage.targetProtocolService.getAddress());
        assertEquals(sentPhaseZeroCompletedMessage.sourceProtocolService.getAddress(), receivedPhaseZeroCompletedMessage.sourceProtocolService.getAddress());
        assertEquals(sentPhaseZeroCompletedMessage.dialogIdentifier, receivedPhaseZeroCompletedMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _phaseZeroParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _phaseZeroCoordinator.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _phaseZeroParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _phaseZeroCoordinator.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendPhaseZeroCompletedWithDialogIdentifierWithSelection()
        throws Exception
    {
        PhaseZeroCompletedMessage sentPhaseZeroCompletedMessage     = new PhaseZeroCompletedMessage();
        PhaseZeroCompletedMessage receivedPhaseZeroCompletedMessage = null;

        sentPhaseZeroCompletedMessage.targetProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentPhaseZeroCompletedMessage.sourceProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentPhaseZeroCompletedMessage.dialogIdentifier      = "123456";

        _phaseZeroParticipant.sendPhaseZeroCompleted(sentPhaseZeroCompletedMessage);

        receivedPhaseZeroCompletedMessage = (PhaseZeroCompletedMessage) _phaseZeroCoordinator.recv(sentPhaseZeroCompletedMessage.dialogIdentifier);

        assertEquals(sentPhaseZeroCompletedMessage.targetProtocolService.getAddress(), receivedPhaseZeroCompletedMessage.targetProtocolService.getAddress());
        assertEquals(sentPhaseZeroCompletedMessage.sourceProtocolService.getAddress(), receivedPhaseZeroCompletedMessage.sourceProtocolService.getAddress());
        assertEquals(sentPhaseZeroCompletedMessage.dialogIdentifier, receivedPhaseZeroCompletedMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _phaseZeroParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _phaseZeroCoordinator.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _phaseZeroParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _phaseZeroCoordinator.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    protected void tearDown()
        throws Exception
    {
    }

    private PhaseZeroParticipant _phaseZeroParticipant           = null;
    private String               _phaseZeroParticipantServiceURI = null;
    private PhaseZeroCoordinator _phaseZeroCoordinator           = null;
    private String               _phaseZeroCoordinatorServiceURI = null;
}
