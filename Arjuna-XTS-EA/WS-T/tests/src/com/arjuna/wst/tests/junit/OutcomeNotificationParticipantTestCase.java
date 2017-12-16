/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * OutcomeNotificationParticipantTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class OutcomeNotificationParticipantTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String outcomeNotificationParticipantJNDIName = "outcomenotificationparticipant-msg";
        String outcomeNotificationCoordinatorJNDIName = "outcomenotificationcoordinator";

        _outcomeNotificationParticipant           = (OutcomeNotificationParticipant) initialContext.lookup(outcomeNotificationParticipantJNDIName);
        _outcomeNotificationParticipantServiceURI = _outcomeNotificationParticipant.getServiceURI();
        _outcomeNotificationCoordinator           = (OutcomeNotificationCoordinator) initialContext.lookup(outcomeNotificationCoordinatorJNDIName);
        _outcomeNotificationCoordinatorServiceURI = _outcomeNotificationCoordinator.getServiceURI();
    }

    public void testSendNotifiedWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        NotifiedMessage sentNotifiedMessage     = new NotifiedMessage();
        NotifiedMessage receivedNotifiedMessage = null;

        sentNotifiedMessage.targetProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentNotifiedMessage.sourceProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentNotifiedMessage.dialogIdentifier      = null;

        _outcomeNotificationParticipant.sendNotified(sentNotifiedMessage);

        receivedNotifiedMessage = (NotifiedMessage) _outcomeNotificationCoordinator.recv();

        assertEquals(sentNotifiedMessage.targetProtocolService.getAddress(), receivedNotifiedMessage.targetProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.sourceProtocolService.getAddress(), receivedNotifiedMessage.sourceProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.dialogIdentifier, receivedNotifiedMessage.dialogIdentifier);
    }

    public void testSendReplayWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        ReplayMessage sentReplayMessage     = new ReplayMessage();
        ReplayMessage receivedReplayMessage = null;

        sentReplayMessage.coordinatorProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentReplayMessage.participantProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentReplayMessage.dialogIdentifier           = null;

        _outcomeNotificationParticipant.sendReplay(sentReplayMessage);

        receivedReplayMessage = (ReplayMessage) _outcomeNotificationCoordinator.recv();

        assertEquals(sentReplayMessage.coordinatorProtocolService.getAddress(), receivedReplayMessage.coordinatorProtocolService.getAddress());
        assertEquals(sentReplayMessage.participantProtocolService.getAddress(), receivedReplayMessage.participantProtocolService.getAddress());
        assertEquals(sentReplayMessage.dialogIdentifier, receivedReplayMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _outcomeNotificationParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _outcomeNotificationCoordinator.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _outcomeNotificationParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _outcomeNotificationCoordinator.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendNotifiedWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        NotifiedMessage sentNotifiedMessage     = new NotifiedMessage();
        NotifiedMessage receivedNotifiedMessage = null;

        sentNotifiedMessage.targetProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentNotifiedMessage.sourceProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentNotifiedMessage.dialogIdentifier      = "123456";

        _outcomeNotificationParticipant.sendNotified(sentNotifiedMessage);

        receivedNotifiedMessage = (NotifiedMessage) _outcomeNotificationCoordinator.recv();

        assertEquals(sentNotifiedMessage.targetProtocolService.getAddress(), receivedNotifiedMessage.targetProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.sourceProtocolService.getAddress(), receivedNotifiedMessage.sourceProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.dialogIdentifier, receivedNotifiedMessage.dialogIdentifier);
    }

    public void testSendReplayWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        ReplayMessage sentReplayMessage     = new ReplayMessage();
        ReplayMessage receivedReplayMessage = null;

        sentReplayMessage.coordinatorProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentReplayMessage.participantProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentReplayMessage.dialogIdentifier           = "123456";

        _outcomeNotificationParticipant.sendReplay(sentReplayMessage);

        receivedReplayMessage = (ReplayMessage) _outcomeNotificationCoordinator.recv();

        assertEquals(sentReplayMessage.coordinatorProtocolService.getAddress(), receivedReplayMessage.coordinatorProtocolService.getAddress());
        assertEquals(sentReplayMessage.participantProtocolService.getAddress(), receivedReplayMessage.participantProtocolService.getAddress());
        assertEquals(sentReplayMessage.dialogIdentifier, receivedReplayMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _outcomeNotificationParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _outcomeNotificationCoordinator.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _outcomeNotificationParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _outcomeNotificationCoordinator.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendNotifiedWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        NotifiedMessage sentNotifiedMessage     = new NotifiedMessage();
        NotifiedMessage receivedNotifiedMessage = null;

        sentNotifiedMessage.targetProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentNotifiedMessage.sourceProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentNotifiedMessage.dialogIdentifier      = null;

        _outcomeNotificationParticipant.sendNotified(sentNotifiedMessage);

        receivedNotifiedMessage = (NotifiedMessage) _outcomeNotificationCoordinator.recv(sentNotifiedMessage.dialogIdentifier);

        assertEquals(sentNotifiedMessage.targetProtocolService.getAddress(), receivedNotifiedMessage.targetProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.sourceProtocolService.getAddress(), receivedNotifiedMessage.sourceProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.dialogIdentifier, receivedNotifiedMessage.dialogIdentifier);
    }

    public void testSendReplayWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        ReplayMessage sentReplayMessage     = new ReplayMessage();
        ReplayMessage receivedReplayMessage = null;

        sentReplayMessage.coordinatorProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentReplayMessage.participantProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentReplayMessage.dialogIdentifier           = null;

        _outcomeNotificationParticipant.sendReplay(sentReplayMessage);

        receivedReplayMessage = (ReplayMessage) _outcomeNotificationCoordinator.recv(sentReplayMessage.dialogIdentifier);

        assertEquals(sentReplayMessage.coordinatorProtocolService.getAddress(), receivedReplayMessage.coordinatorProtocolService.getAddress());
        assertEquals(sentReplayMessage.participantProtocolService.getAddress(), receivedReplayMessage.participantProtocolService.getAddress());
        assertEquals(sentReplayMessage.dialogIdentifier, receivedReplayMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _outcomeNotificationParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _outcomeNotificationCoordinator.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _outcomeNotificationParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _outcomeNotificationCoordinator.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendNotifiedWithDialogIdentifierWithSelection()
        throws Exception
    {
        NotifiedMessage sentNotifiedMessage     = new NotifiedMessage();
        NotifiedMessage receivedNotifiedMessage = null;

        sentNotifiedMessage.targetProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentNotifiedMessage.sourceProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentNotifiedMessage.dialogIdentifier      = "123456";

        _outcomeNotificationParticipant.sendNotified(sentNotifiedMessage);

        receivedNotifiedMessage = (NotifiedMessage) _outcomeNotificationCoordinator.recv(sentNotifiedMessage.dialogIdentifier);

        assertEquals(sentNotifiedMessage.targetProtocolService.getAddress(), receivedNotifiedMessage.targetProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.sourceProtocolService.getAddress(), receivedNotifiedMessage.sourceProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.dialogIdentifier, receivedNotifiedMessage.dialogIdentifier);
    }

    public void testSendReplayWithDialogIdentifierWithSelection()
        throws Exception
    {
        ReplayMessage sentReplayMessage     = new ReplayMessage();
        ReplayMessage receivedReplayMessage = null;

        sentReplayMessage.coordinatorProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentReplayMessage.participantProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentReplayMessage.dialogIdentifier           = "123456";

        _outcomeNotificationParticipant.sendReplay(sentReplayMessage);

        receivedReplayMessage = (ReplayMessage) _outcomeNotificationCoordinator.recv(sentReplayMessage.dialogIdentifier);

        assertEquals(sentReplayMessage.coordinatorProtocolService.getAddress(), receivedReplayMessage.coordinatorProtocolService.getAddress());
        assertEquals(sentReplayMessage.participantProtocolService.getAddress(), receivedReplayMessage.participantProtocolService.getAddress());
        assertEquals(sentReplayMessage.dialogIdentifier, receivedReplayMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_outcomeNotificationParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _outcomeNotificationParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _outcomeNotificationCoordinator.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_outcomeNotificationCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _outcomeNotificationParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _outcomeNotificationCoordinator.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    protected void tearDown()
        throws Exception
    {
    }

    private OutcomeNotificationParticipant _outcomeNotificationParticipant           = null;
    private String                         _outcomeNotificationParticipantServiceURI = null;
    private OutcomeNotificationCoordinator _outcomeNotificationCoordinator           = null;
    private String                         _outcomeNotificationCoordinatorServiceURI = null;
}
