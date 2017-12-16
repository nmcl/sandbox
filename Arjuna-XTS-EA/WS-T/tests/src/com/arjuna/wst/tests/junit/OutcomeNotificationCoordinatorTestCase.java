/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * OutcomeNotificationCoordinatorTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class OutcomeNotificationCoordinatorTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String outcomeNotificationCoordinatorJNDIName = "outcomenotificationcoordinator";
        String outcomeNotificationParticipantJNDIName = "outcomenotificationparticipant-msg";

        _outcomeNotificationCoordinator           = (OutcomeNotificationCoordinator) initialContext.lookup(outcomeNotificationCoordinatorJNDIName);
        _outcomeNotificationCoordinatorServiceURI = _outcomeNotificationCoordinator.getServiceURI();
        _outcomeNotificationParticipant           = (OutcomeNotificationParticipant) initialContext.lookup(outcomeNotificationParticipantJNDIName);
        _outcomeNotificationParticipantServiceURI = _outcomeNotificationParticipant.getServiceURI();
    }

    public void testSendCommittedWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        CommittedMessage sentCommittedMessage     = new CommittedMessage();
        CommittedMessage receivedCommittedMessage = null;

        sentCommittedMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentCommittedMessage.dialogIdentifier      = null;

        _outcomeNotificationCoordinator.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _outcomeNotificationParticipant.recv();

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentAbortedMessage.dialogIdentifier      = null;

        _outcomeNotificationCoordinator.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _outcomeNotificationParticipant.recv();

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _outcomeNotificationCoordinator.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _outcomeNotificationParticipant.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _outcomeNotificationCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _outcomeNotificationParticipant.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendCommittedWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        CommittedMessage sentCommittedMessage     = new CommittedMessage();
        CommittedMessage receivedCommittedMessage = null;

        sentCommittedMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentCommittedMessage.dialogIdentifier      = "123456";

        _outcomeNotificationCoordinator.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _outcomeNotificationParticipant.recv();

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentAbortedMessage.dialogIdentifier      = "123456";

        _outcomeNotificationCoordinator.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _outcomeNotificationParticipant.recv();

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _outcomeNotificationCoordinator.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _outcomeNotificationParticipant.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _outcomeNotificationCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _outcomeNotificationParticipant.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendCommittedWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        CommittedMessage sentCommittedMessage     = new CommittedMessage();
        CommittedMessage receivedCommittedMessage = null;

        sentCommittedMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentCommittedMessage.dialogIdentifier      = null;

        _outcomeNotificationCoordinator.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _outcomeNotificationParticipant.recv(sentCommittedMessage.dialogIdentifier);

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentAbortedMessage.dialogIdentifier      = null;

        _outcomeNotificationCoordinator.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _outcomeNotificationParticipant.recv(sentAbortedMessage.dialogIdentifier);

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _outcomeNotificationCoordinator.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _outcomeNotificationParticipant.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _outcomeNotificationCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _outcomeNotificationParticipant.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendCommittedWithDialogIdentifierWithSelection()
        throws Exception
    {
        CommittedMessage sentCommittedMessage     = new CommittedMessage();
        CommittedMessage receivedCommittedMessage = null;

        sentCommittedMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentCommittedMessage.dialogIdentifier      = "123456";

        _outcomeNotificationCoordinator.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _outcomeNotificationParticipant.recv(sentCommittedMessage.dialogIdentifier);

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithDialogIdentifierWithSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentAbortedMessage.dialogIdentifier      = "123456";

        _outcomeNotificationCoordinator.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _outcomeNotificationParticipant.recv(sentAbortedMessage.dialogIdentifier);

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _outcomeNotificationCoordinator.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _outcomeNotificationParticipant.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _outcomeNotificationCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _outcomeNotificationParticipant.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    protected void tearDown()
        throws Exception
    {
    }

    private OutcomeNotificationCoordinator _outcomeNotificationCoordinator           = null;
    private String                         _outcomeNotificationCoordinatorServiceURI = null;
    private OutcomeNotificationParticipant _outcomeNotificationParticipant           = null;
    private String                         _outcomeNotificationParticipantServiceURI = null;
}
