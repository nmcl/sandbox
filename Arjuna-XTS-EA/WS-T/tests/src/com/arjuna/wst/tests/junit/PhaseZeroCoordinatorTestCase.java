/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * PhaseZeroCoordinatorTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class PhaseZeroCoordinatorTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String phaseZeroCoordinatorJNDIName = "phasezerocoordinator";
        String phaseZeroParticipantJNDIName = "phasezeroparticipant-msg";

        _phaseZeroCoordinator           = (PhaseZeroCoordinator) initialContext.lookup(phaseZeroCoordinatorJNDIName);
        _phaseZeroCoordinatorServiceURI = _phaseZeroCoordinator.getServiceURI();
        _phaseZeroParticipant           = (PhaseZeroParticipant) initialContext.lookup(phaseZeroParticipantJNDIName);
        _phaseZeroParticipantServiceURI = _phaseZeroParticipant.getServiceURI();
    }

    public void testSendPhaseZeroWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        PhaseZeroMessage sentPhaseZeroMessage     = new PhaseZeroMessage();
        PhaseZeroMessage receivedPhaseZeroMessage = null;

        sentPhaseZeroMessage.targetProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentPhaseZeroMessage.sourceProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentPhaseZeroMessage.dialogIdentifier      = null;

        _phaseZeroCoordinator.sendPhaseZero(sentPhaseZeroMessage);

        receivedPhaseZeroMessage = (PhaseZeroMessage) _phaseZeroParticipant.recv();

        assertEquals(sentPhaseZeroMessage.targetProtocolService.getAddress(), receivedPhaseZeroMessage.targetProtocolService.getAddress());
        assertEquals(sentPhaseZeroMessage.sourceProtocolService.getAddress(), receivedPhaseZeroMessage.sourceProtocolService.getAddress());
        assertEquals(sentPhaseZeroMessage.dialogIdentifier, receivedPhaseZeroMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _phaseZeroCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _phaseZeroParticipant.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendPhaseZeroWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        PhaseZeroMessage sentPhaseZeroMessage     = new PhaseZeroMessage();
        PhaseZeroMessage receivedPhaseZeroMessage = null;

        sentPhaseZeroMessage.targetProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentPhaseZeroMessage.sourceProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentPhaseZeroMessage.dialogIdentifier      = "123456";

        _phaseZeroCoordinator.sendPhaseZero(sentPhaseZeroMessage);

        receivedPhaseZeroMessage = (PhaseZeroMessage) _phaseZeroParticipant.recv();

        assertEquals(sentPhaseZeroMessage.targetProtocolService.getAddress(), receivedPhaseZeroMessage.targetProtocolService.getAddress());
        assertEquals(sentPhaseZeroMessage.sourceProtocolService.getAddress(), receivedPhaseZeroMessage.sourceProtocolService.getAddress());
        assertEquals(sentPhaseZeroMessage.dialogIdentifier, receivedPhaseZeroMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _phaseZeroCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _phaseZeroParticipant.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendPhaseZeroWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        PhaseZeroMessage sentPhaseZeroMessage     = new PhaseZeroMessage();
        PhaseZeroMessage receivedPhaseZeroMessage = null;

        sentPhaseZeroMessage.targetProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentPhaseZeroMessage.sourceProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentPhaseZeroMessage.dialogIdentifier      = null;

        _phaseZeroCoordinator.sendPhaseZero(sentPhaseZeroMessage);

        receivedPhaseZeroMessage = (PhaseZeroMessage) _phaseZeroParticipant.recv(sentPhaseZeroMessage.dialogIdentifier);

        assertEquals(sentPhaseZeroMessage.targetProtocolService.getAddress(), receivedPhaseZeroMessage.targetProtocolService.getAddress());
        assertEquals(sentPhaseZeroMessage.sourceProtocolService.getAddress(), receivedPhaseZeroMessage.sourceProtocolService.getAddress());
        assertEquals(sentPhaseZeroMessage.dialogIdentifier, receivedPhaseZeroMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _phaseZeroCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _phaseZeroParticipant.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendPhaseZeroWithDialogIdentifierWithSelection()
        throws Exception
    {
        PhaseZeroMessage sentPhaseZeroMessage     = new PhaseZeroMessage();
        PhaseZeroMessage receivedPhaseZeroMessage = null;

        sentPhaseZeroMessage.targetProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentPhaseZeroMessage.sourceProtocolService = new PortReference(_phaseZeroCoordinatorServiceURI);
        sentPhaseZeroMessage.dialogIdentifier      = "123456";

        _phaseZeroCoordinator.sendPhaseZero(sentPhaseZeroMessage);

        receivedPhaseZeroMessage = (PhaseZeroMessage) _phaseZeroParticipant.recv(sentPhaseZeroMessage.dialogIdentifier);

        assertEquals(sentPhaseZeroMessage.targetProtocolService.getAddress(), receivedPhaseZeroMessage.targetProtocolService.getAddress());
        assertEquals(sentPhaseZeroMessage.sourceProtocolService.getAddress(), receivedPhaseZeroMessage.sourceProtocolService.getAddress());
        assertEquals(sentPhaseZeroMessage.dialogIdentifier, receivedPhaseZeroMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_phaseZeroParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _phaseZeroCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _phaseZeroParticipant.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    protected void tearDown()
        throws Exception
    {
    }

    private PhaseZeroCoordinator _phaseZeroCoordinator           = null;
    private String               _phaseZeroCoordinatorServiceURI = null;
    private PhaseZeroParticipant _phaseZeroParticipant           = null;
    private String               _phaseZeroParticipantServiceURI = null;
}
