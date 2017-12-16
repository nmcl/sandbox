/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TwoPCParticipantTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class TwoPCParticipantTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String twoPCParticipantJNDIName = "twopcparticipant-msg";
        String twoPCCoordinatorJNDIName = "twopccoordinator";

        _twoPCParticipant           = (TwoPCParticipant) initialContext.lookup(twoPCParticipantJNDIName);
        _twoPCParticipantServiceURI = _twoPCParticipant.getServiceURI();
        _twoPCCoordinator           = (TwoPCCoordinator) initialContext.lookup(twoPCCoordinatorJNDIName);
        _twoPCCoordinatorServiceURI = _twoPCCoordinator.getServiceURI();
    }

    public void testSendPreparedWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        PreparedMessage sentPreparedMessage     = new PreparedMessage();
        PreparedMessage receivedPreparedMessage = null;

        sentPreparedMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentPreparedMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentPreparedMessage.dialogIdentifier      = null;

        _twoPCParticipant.sendPrepared(sentPreparedMessage);

        receivedPreparedMessage = (PreparedMessage) _twoPCCoordinator.recv();

        assertEquals(sentPreparedMessage.targetProtocolService.getAddress(), receivedPreparedMessage.targetProtocolService.getAddress());
        assertEquals(sentPreparedMessage.sourceProtocolService.getAddress(), receivedPreparedMessage.sourceProtocolService.getAddress());
        assertEquals(sentPreparedMessage.dialogIdentifier, receivedPreparedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentAbortedMessage.dialogIdentifier      = null;

        _twoPCParticipant.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _twoPCCoordinator.recv();

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendReadOnlyWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        ReadOnlyMessage sentReadOnlyMessage     = new ReadOnlyMessage();
        ReadOnlyMessage receivedReadOnlyMessage = null;

        sentReadOnlyMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentReadOnlyMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentReadOnlyMessage.dialogIdentifier      = null;

        _twoPCParticipant.sendReadOnly(sentReadOnlyMessage);

        receivedReadOnlyMessage = (ReadOnlyMessage) _twoPCCoordinator.recv();

        assertEquals(sentReadOnlyMessage.targetProtocolService.getAddress(), receivedReadOnlyMessage.targetProtocolService.getAddress());
        assertEquals(sentReadOnlyMessage.sourceProtocolService.getAddress(), receivedReadOnlyMessage.sourceProtocolService.getAddress());
        assertEquals(sentReadOnlyMessage.dialogIdentifier, receivedReadOnlyMessage.dialogIdentifier);
    }

    public void testSendCommittedWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        CommittedMessage sentCommittedMessage     = new CommittedMessage();
        CommittedMessage receivedCommittedMessage = null;

        sentCommittedMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentCommittedMessage.dialogIdentifier      = null;

        _twoPCParticipant.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _twoPCCoordinator.recv();

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendReplayWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        ReplayMessage sentReplayMessage     = new ReplayMessage();
        ReplayMessage receivedReplayMessage = null;

        sentReplayMessage.coordinatorProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentReplayMessage.participantProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentReplayMessage.dialogIdentifier           = null;

        _twoPCParticipant.sendReplay(sentReplayMessage);

        receivedReplayMessage = (ReplayMessage) _twoPCCoordinator.recv();

        assertEquals(sentReplayMessage.coordinatorProtocolService.getAddress(), receivedReplayMessage.coordinatorProtocolService.getAddress());
        assertEquals(sentReplayMessage.participantProtocolService.getAddress(), receivedReplayMessage.participantProtocolService.getAddress());
        assertEquals(sentReplayMessage.dialogIdentifier, receivedReplayMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _twoPCParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _twoPCCoordinator.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _twoPCParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _twoPCCoordinator.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendPreparedWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        PreparedMessage sentPreparedMessage     = new PreparedMessage();
        PreparedMessage receivedPreparedMessage = null;

        sentPreparedMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentPreparedMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentPreparedMessage.dialogIdentifier      = "123456";

        _twoPCParticipant.sendPrepared(sentPreparedMessage);

        receivedPreparedMessage = (PreparedMessage) _twoPCCoordinator.recv();

        assertEquals(sentPreparedMessage.targetProtocolService.getAddress(), receivedPreparedMessage.targetProtocolService.getAddress());
        assertEquals(sentPreparedMessage.sourceProtocolService.getAddress(), receivedPreparedMessage.sourceProtocolService.getAddress());
        assertEquals(sentPreparedMessage.dialogIdentifier, receivedPreparedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentAbortedMessage.dialogIdentifier      = "123456";

        _twoPCParticipant.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _twoPCCoordinator.recv();

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendReadOnlyWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        ReadOnlyMessage sentReadOnlyMessage     = new ReadOnlyMessage();
        ReadOnlyMessage receivedReadOnlyMessage = null;

        sentReadOnlyMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentReadOnlyMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentReadOnlyMessage.dialogIdentifier      = "123456";

        _twoPCParticipant.sendReadOnly(sentReadOnlyMessage);

        receivedReadOnlyMessage = (ReadOnlyMessage) _twoPCCoordinator.recv();

        assertEquals(sentReadOnlyMessage.targetProtocolService.getAddress(), receivedReadOnlyMessage.targetProtocolService.getAddress());
        assertEquals(sentReadOnlyMessage.sourceProtocolService.getAddress(), receivedReadOnlyMessage.sourceProtocolService.getAddress());
        assertEquals(sentReadOnlyMessage.dialogIdentifier, receivedReadOnlyMessage.dialogIdentifier);
    }

    public void testSendCommittedWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        CommittedMessage sentCommittedMessage     = new CommittedMessage();
        CommittedMessage receivedCommittedMessage = null;

        sentCommittedMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentCommittedMessage.dialogIdentifier      = "123456";

        _twoPCParticipant.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _twoPCCoordinator.recv();

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendReplayWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        ReplayMessage sentReplayMessage     = new ReplayMessage();
        ReplayMessage receivedReplayMessage = null;

        sentReplayMessage.coordinatorProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentReplayMessage.participantProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentReplayMessage.dialogIdentifier           = "123456";

        _twoPCParticipant.sendReplay(sentReplayMessage);

        receivedReplayMessage = (ReplayMessage) _twoPCCoordinator.recv();

        assertEquals(sentReplayMessage.coordinatorProtocolService.getAddress(), receivedReplayMessage.coordinatorProtocolService.getAddress());
        assertEquals(sentReplayMessage.participantProtocolService.getAddress(), receivedReplayMessage.participantProtocolService.getAddress());
        assertEquals(sentReplayMessage.dialogIdentifier, receivedReplayMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _twoPCParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _twoPCCoordinator.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _twoPCParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _twoPCCoordinator.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendPreparedWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        PreparedMessage sentPreparedMessage     = new PreparedMessage();
        PreparedMessage receivedPreparedMessage = null;

        sentPreparedMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentPreparedMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentPreparedMessage.dialogIdentifier      = null;

        _twoPCParticipant.sendPrepared(sentPreparedMessage);

        receivedPreparedMessage = (PreparedMessage) _twoPCCoordinator.recv(sentPreparedMessage.dialogIdentifier);

        assertEquals(sentPreparedMessage.targetProtocolService.getAddress(), receivedPreparedMessage.targetProtocolService.getAddress());
        assertEquals(sentPreparedMessage.sourceProtocolService.getAddress(), receivedPreparedMessage.sourceProtocolService.getAddress());
        assertEquals(sentPreparedMessage.dialogIdentifier, receivedPreparedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentAbortedMessage.dialogIdentifier      = null;

        _twoPCParticipant.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _twoPCCoordinator.recv(sentAbortedMessage.dialogIdentifier);

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendReadOnlyWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        ReadOnlyMessage sentReadOnlyMessage     = new ReadOnlyMessage();
        ReadOnlyMessage receivedReadOnlyMessage = null;

        sentReadOnlyMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentReadOnlyMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentReadOnlyMessage.dialogIdentifier      = null;

        _twoPCParticipant.sendReadOnly(sentReadOnlyMessage);

        receivedReadOnlyMessage = (ReadOnlyMessage) _twoPCCoordinator.recv(sentReadOnlyMessage.dialogIdentifier);

        assertEquals(sentReadOnlyMessage.targetProtocolService.getAddress(), receivedReadOnlyMessage.targetProtocolService.getAddress());
        assertEquals(sentReadOnlyMessage.sourceProtocolService.getAddress(), receivedReadOnlyMessage.sourceProtocolService.getAddress());
        assertEquals(sentReadOnlyMessage.dialogIdentifier, receivedReadOnlyMessage.dialogIdentifier);
    }

    public void testSendCommittedWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        CommittedMessage sentCommittedMessage     = new CommittedMessage();
        CommittedMessage receivedCommittedMessage = null;

        sentCommittedMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentCommittedMessage.dialogIdentifier      = null;

        _twoPCParticipant.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _twoPCCoordinator.recv(sentCommittedMessage.dialogIdentifier);

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendReplayWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        ReplayMessage sentReplayMessage     = new ReplayMessage();
        ReplayMessage receivedReplayMessage = null;

        sentReplayMessage.coordinatorProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentReplayMessage.participantProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentReplayMessage.dialogIdentifier           = null;

        _twoPCParticipant.sendReplay(sentReplayMessage);

        receivedReplayMessage = (ReplayMessage) _twoPCCoordinator.recv(sentReplayMessage.dialogIdentifier);

        assertEquals(sentReplayMessage.coordinatorProtocolService.getAddress(), receivedReplayMessage.coordinatorProtocolService.getAddress());
        assertEquals(sentReplayMessage.participantProtocolService.getAddress(), receivedReplayMessage.participantProtocolService.getAddress());
        assertEquals(sentReplayMessage.dialogIdentifier, receivedReplayMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _twoPCParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _twoPCCoordinator.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _twoPCParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _twoPCCoordinator.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendPreparedWithDialogIdentifierWithSelection()
        throws Exception
    {
        PreparedMessage sentPreparedMessage     = new PreparedMessage();
        PreparedMessage receivedPreparedMessage = null;

        sentPreparedMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentPreparedMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentPreparedMessage.dialogIdentifier      = "123456";

        _twoPCParticipant.sendPrepared(sentPreparedMessage);

        receivedPreparedMessage = (PreparedMessage) _twoPCCoordinator.recv(sentPreparedMessage.dialogIdentifier);

        assertEquals(sentPreparedMessage.targetProtocolService.getAddress(), receivedPreparedMessage.targetProtocolService.getAddress());
        assertEquals(sentPreparedMessage.sourceProtocolService.getAddress(), receivedPreparedMessage.sourceProtocolService.getAddress());
        assertEquals(sentPreparedMessage.dialogIdentifier, receivedPreparedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithDialogIdentifierWithSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentAbortedMessage.dialogIdentifier      = "123456";

        _twoPCParticipant.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _twoPCCoordinator.recv(sentAbortedMessage.dialogIdentifier);

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendReadOnlyWithDialogIdentifierWithSelection()
        throws Exception
    {
        ReadOnlyMessage sentReadOnlyMessage     = new ReadOnlyMessage();
        ReadOnlyMessage receivedReadOnlyMessage = null;

        sentReadOnlyMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentReadOnlyMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentReadOnlyMessage.dialogIdentifier      = "123456";

        _twoPCParticipant.sendReadOnly(sentReadOnlyMessage);

        receivedReadOnlyMessage = (ReadOnlyMessage) _twoPCCoordinator.recv(sentReadOnlyMessage.dialogIdentifier);

        assertEquals(sentReadOnlyMessage.targetProtocolService.getAddress(), receivedReadOnlyMessage.targetProtocolService.getAddress());
        assertEquals(sentReadOnlyMessage.sourceProtocolService.getAddress(), receivedReadOnlyMessage.sourceProtocolService.getAddress());
        assertEquals(sentReadOnlyMessage.dialogIdentifier, receivedReadOnlyMessage.dialogIdentifier);
    }

    public void testSendCommittedWithDialogIdentifierWithSelection()
        throws Exception
    {
        CommittedMessage sentCommittedMessage     = new CommittedMessage();
        CommittedMessage receivedCommittedMessage = null;

        sentCommittedMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentCommittedMessage.dialogIdentifier      = "123456";

        _twoPCParticipant.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _twoPCCoordinator.recv(sentCommittedMessage.dialogIdentifier);

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendReplayWithDialogIdentifierWithSelection()
        throws Exception
    {
        ReplayMessage sentReplayMessage     = new ReplayMessage();
        ReplayMessage receivedReplayMessage = null;

        sentReplayMessage.coordinatorProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentReplayMessage.participantProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentReplayMessage.dialogIdentifier           = "123456";

        _twoPCParticipant.sendReplay(sentReplayMessage);

        receivedReplayMessage = (ReplayMessage) _twoPCCoordinator.recv(sentReplayMessage.dialogIdentifier);

        assertEquals(sentReplayMessage.coordinatorProtocolService.getAddress(), receivedReplayMessage.coordinatorProtocolService.getAddress());
        assertEquals(sentReplayMessage.participantProtocolService.getAddress(), receivedReplayMessage.participantProtocolService.getAddress());
        assertEquals(sentReplayMessage.dialogIdentifier, receivedReplayMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _twoPCParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _twoPCCoordinator.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _twoPCParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _twoPCCoordinator.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    protected void tearDown()
        throws Exception
    {
    }

    private TwoPCParticipant _twoPCParticipant           = null;
    private String           _twoPCParticipantServiceURI = null;
    private TwoPCCoordinator _twoPCCoordinator           = null;
    private String           _twoPCCoordinatorServiceURI = null;
}
