/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionWithAckParticipantTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class CompletionWithAckParticipantTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String completionWithAckParticipantJNDIName = "completionwithackparticipant";
        String completionWithAckCoordinatorJNDIName = "completionwithackcoordinator-msg";

        _completionWithAckParticipant           = (CompletionWithAckParticipant) initialContext.lookup(completionWithAckParticipantJNDIName);
        _completionWithAckParticipantServiceURI = _completionWithAckParticipant.getServiceURI();
        _completionWithAckCoordinator           = (CompletionWithAckCoordinator) initialContext.lookup(completionWithAckCoordinatorJNDIName);
        _completionWithAckCoordinatorServiceURI = _completionWithAckCoordinator.getServiceURI();
    }

    public void testSendCommitWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        CommitMessage sentCommitMessage     = new CommitMessage();
        CommitMessage receivedCommitMessage = null;

        sentCommitMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentCommitMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentCommitMessage.dialogIdentifier      = null;

        _completionWithAckParticipant.sendCommit(sentCommitMessage);

        receivedCommitMessage = (CommitMessage) _completionWithAckCoordinator.recv();

        assertEquals(sentCommitMessage.targetProtocolService.getAddress(), receivedCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentCommitMessage.sourceProtocolService.getAddress(), receivedCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommitMessage.dialogIdentifier, receivedCommitMessage.dialogIdentifier);
    }

    public void testSendRollbackWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        RollbackMessage sentRollbackMessage     = new RollbackMessage();
        RollbackMessage receivedRollbackMessage = null;

        sentRollbackMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentRollbackMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentRollbackMessage.dialogIdentifier      = null;

        _completionWithAckParticipant.sendRollback(sentRollbackMessage);

        receivedRollbackMessage = (RollbackMessage) _completionWithAckCoordinator.recv();

        assertEquals(sentRollbackMessage.targetProtocolService.getAddress(), receivedRollbackMessage.targetProtocolService.getAddress());
        assertEquals(sentRollbackMessage.sourceProtocolService.getAddress(), receivedRollbackMessage.sourceProtocolService.getAddress());
        assertEquals(sentRollbackMessage.dialogIdentifier, receivedRollbackMessage.dialogIdentifier);
    }

    public void testSendNotifiedWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        NotifiedMessage sentNotifiedMessage     = new NotifiedMessage();
        NotifiedMessage receivedNotifiedMessage = null;

        sentNotifiedMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentNotifiedMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentNotifiedMessage.dialogIdentifier      = null;

        _completionWithAckParticipant.sendNotified(sentNotifiedMessage);

        receivedNotifiedMessage = (NotifiedMessage) _completionWithAckCoordinator.recv();

        assertEquals(sentNotifiedMessage.targetProtocolService.getAddress(), receivedNotifiedMessage.targetProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.sourceProtocolService.getAddress(), receivedNotifiedMessage.sourceProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.dialogIdentifier, receivedNotifiedMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _completionWithAckParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _completionWithAckCoordinator.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _completionWithAckParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionWithAckCoordinator.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendCommitWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        CommitMessage sentCommitMessage     = new CommitMessage();
        CommitMessage receivedCommitMessage = null;

        sentCommitMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentCommitMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentCommitMessage.dialogIdentifier      = "123456";

        _completionWithAckParticipant.sendCommit(sentCommitMessage);

        receivedCommitMessage = (CommitMessage) _completionWithAckCoordinator.recv();

        assertEquals(sentCommitMessage.targetProtocolService.getAddress(), receivedCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentCommitMessage.sourceProtocolService.getAddress(), receivedCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommitMessage.dialogIdentifier, receivedCommitMessage.dialogIdentifier);
    }

    public void testSendRollbackWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        RollbackMessage sentRollbackMessage     = new RollbackMessage();
        RollbackMessage receivedRollbackMessage = null;

        sentRollbackMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentRollbackMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentRollbackMessage.dialogIdentifier      = "123456";

        _completionWithAckParticipant.sendRollback(sentRollbackMessage);

        receivedRollbackMessage = (RollbackMessage) _completionWithAckCoordinator.recv();

        assertEquals(sentRollbackMessage.targetProtocolService.getAddress(), receivedRollbackMessage.targetProtocolService.getAddress());
        assertEquals(sentRollbackMessage.sourceProtocolService.getAddress(), receivedRollbackMessage.sourceProtocolService.getAddress());
        assertEquals(sentRollbackMessage.dialogIdentifier, receivedRollbackMessage.dialogIdentifier);
    }

    public void testSendNotifiedWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        NotifiedMessage sentNotifiedMessage     = new NotifiedMessage();
        NotifiedMessage receivedNotifiedMessage = null;

        sentNotifiedMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentNotifiedMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentNotifiedMessage.dialogIdentifier      = "123456";

        _completionWithAckParticipant.sendNotified(sentNotifiedMessage);

        receivedNotifiedMessage = (NotifiedMessage) _completionWithAckCoordinator.recv();

        assertEquals(sentNotifiedMessage.targetProtocolService.getAddress(), receivedNotifiedMessage.targetProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.sourceProtocolService.getAddress(), receivedNotifiedMessage.sourceProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.dialogIdentifier, receivedNotifiedMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _completionWithAckParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _completionWithAckCoordinator.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _completionWithAckParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionWithAckCoordinator.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendCommitWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        CommitMessage sentCommitMessage     = new CommitMessage();
        CommitMessage receivedCommitMessage = null;

        sentCommitMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentCommitMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentCommitMessage.dialogIdentifier      = null;

        _completionWithAckParticipant.sendCommit(sentCommitMessage);

        receivedCommitMessage = (CommitMessage) _completionWithAckCoordinator.recv(sentCommitMessage.dialogIdentifier);

        assertEquals(sentCommitMessage.targetProtocolService.getAddress(), receivedCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentCommitMessage.sourceProtocolService.getAddress(), receivedCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommitMessage.dialogIdentifier, receivedCommitMessage.dialogIdentifier);
    }

    public void testSendRollbackWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        RollbackMessage sentRollbackMessage     = new RollbackMessage();
        RollbackMessage receivedRollbackMessage = null;

        sentRollbackMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentRollbackMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentRollbackMessage.dialogIdentifier      = null;

        _completionWithAckParticipant.sendRollback(sentRollbackMessage);

        receivedRollbackMessage = (RollbackMessage) _completionWithAckCoordinator.recv(sentRollbackMessage.dialogIdentifier);

        assertEquals(sentRollbackMessage.targetProtocolService.getAddress(), receivedRollbackMessage.targetProtocolService.getAddress());
        assertEquals(sentRollbackMessage.sourceProtocolService.getAddress(), receivedRollbackMessage.sourceProtocolService.getAddress());
        assertEquals(sentRollbackMessage.dialogIdentifier, receivedRollbackMessage.dialogIdentifier);
    }

    public void testSendNotifiedWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        NotifiedMessage sentNotifiedMessage     = new NotifiedMessage();
        NotifiedMessage receivedNotifiedMessage = null;

        sentNotifiedMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentNotifiedMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentNotifiedMessage.dialogIdentifier      = null;

        _completionWithAckParticipant.sendNotified(sentNotifiedMessage);

        receivedNotifiedMessage = (NotifiedMessage) _completionWithAckCoordinator.recv(sentNotifiedMessage.dialogIdentifier);

        assertEquals(sentNotifiedMessage.targetProtocolService.getAddress(), receivedNotifiedMessage.targetProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.sourceProtocolService.getAddress(), receivedNotifiedMessage.sourceProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.dialogIdentifier, receivedNotifiedMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _completionWithAckParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _completionWithAckCoordinator.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _completionWithAckParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionWithAckCoordinator.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendCommitWithDialogIdentifierWithSelection()
        throws Exception
    {
        CommitMessage sentCommitMessage     = new CommitMessage();
        CommitMessage receivedCommitMessage = null;

        sentCommitMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentCommitMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentCommitMessage.dialogIdentifier      = "123456";

        _completionWithAckParticipant.sendCommit(sentCommitMessage);

        receivedCommitMessage = (CommitMessage) _completionWithAckCoordinator.recv(sentCommitMessage.dialogIdentifier);

        assertEquals(sentCommitMessage.targetProtocolService.getAddress(), receivedCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentCommitMessage.sourceProtocolService.getAddress(), receivedCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommitMessage.dialogIdentifier, receivedCommitMessage.dialogIdentifier);
    }

    public void testSendRollbackWithDialogIdentifierWithSelection()
        throws Exception
    {
        RollbackMessage sentRollbackMessage     = new RollbackMessage();
        RollbackMessage receivedRollbackMessage = null;

        sentRollbackMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentRollbackMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentRollbackMessage.dialogIdentifier      = "123456";

        _completionWithAckParticipant.sendRollback(sentRollbackMessage);

        receivedRollbackMessage = (RollbackMessage) _completionWithAckCoordinator.recv(sentRollbackMessage.dialogIdentifier);

        assertEquals(sentRollbackMessage.targetProtocolService.getAddress(), receivedRollbackMessage.targetProtocolService.getAddress());
        assertEquals(sentRollbackMessage.sourceProtocolService.getAddress(), receivedRollbackMessage.sourceProtocolService.getAddress());
        assertEquals(sentRollbackMessage.dialogIdentifier, receivedRollbackMessage.dialogIdentifier);
    }

    public void testSendNotifiedWithDialogIdentifierWithSelection()
        throws Exception
    {
        NotifiedMessage sentNotifiedMessage     = new NotifiedMessage();
        NotifiedMessage receivedNotifiedMessage = null;

        sentNotifiedMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentNotifiedMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentNotifiedMessage.dialogIdentifier      = "123456";

        _completionWithAckParticipant.sendNotified(sentNotifiedMessage);

        receivedNotifiedMessage = (NotifiedMessage) _completionWithAckCoordinator.recv(sentNotifiedMessage.dialogIdentifier);

        assertEquals(sentNotifiedMessage.targetProtocolService.getAddress(), receivedNotifiedMessage.targetProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.sourceProtocolService.getAddress(), receivedNotifiedMessage.sourceProtocolService.getAddress());
        assertEquals(sentNotifiedMessage.dialogIdentifier, receivedNotifiedMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _completionWithAckParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _completionWithAckCoordinator.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _completionWithAckParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionWithAckCoordinator.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    protected void tearDown()
        throws Exception
    {
    }

    private CompletionWithAckParticipant _completionWithAckParticipant           = null;
    private String                       _completionWithAckParticipantServiceURI = null;
    private CompletionWithAckCoordinator _completionWithAckCoordinator           = null;
    private String                       _completionWithAckCoordinatorServiceURI = null;
}
