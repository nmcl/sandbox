/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionParticipantTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class CompletionParticipantTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String completionParticipantJNDIName = "completionparticipant";
        String completionCoordinatorJNDIName = "completioncoordinator-msg";

        _completionParticipant           = (CompletionParticipant) initialContext.lookup(completionParticipantJNDIName);
        _completionParticipantServiceURI = _completionParticipant.getServiceURI();
        _completionCoordinator           = (CompletionCoordinator) initialContext.lookup(completionCoordinatorJNDIName);
        _completionCoordinatorServiceURI = _completionCoordinator.getServiceURI();
    }

    public void testSendCommitWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        CommitMessage sentCommitMessage     = new CommitMessage();
        CommitMessage receivedCommitMessage = null;

        sentCommitMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentCommitMessage.sourceProtocolService = new PortReference(_completionParticipantServiceURI);
        sentCommitMessage.dialogIdentifier      = null;

        _completionParticipant.sendCommit(sentCommitMessage);

        receivedCommitMessage = (CommitMessage) _completionCoordinator.recv();

        assertEquals(sentCommitMessage.targetProtocolService.getAddress(), receivedCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentCommitMessage.sourceProtocolService.getAddress(), receivedCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommitMessage.dialogIdentifier, receivedCommitMessage.dialogIdentifier);
    }

    public void testSendRollbackWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        RollbackMessage sentRollbackMessage     = new RollbackMessage();
        RollbackMessage receivedRollbackMessage = null;

        sentRollbackMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentRollbackMessage.sourceProtocolService = new PortReference(_completionParticipantServiceURI);
        sentRollbackMessage.dialogIdentifier      = null;

        _completionParticipant.sendRollback(sentRollbackMessage);

        receivedRollbackMessage = (RollbackMessage) _completionCoordinator.recv();

        assertEquals(sentRollbackMessage.targetProtocolService.getAddress(), receivedRollbackMessage.targetProtocolService.getAddress());
        assertEquals(sentRollbackMessage.sourceProtocolService.getAddress(), receivedRollbackMessage.sourceProtocolService.getAddress());
        assertEquals(sentRollbackMessage.dialogIdentifier, receivedRollbackMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_completionParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _completionParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _completionCoordinator.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _completionParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionCoordinator.recv();

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

        sentCommitMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentCommitMessage.sourceProtocolService = new PortReference(_completionParticipantServiceURI);
        sentCommitMessage.dialogIdentifier      = "123456";

        _completionParticipant.sendCommit(sentCommitMessage);

        receivedCommitMessage = (CommitMessage) _completionCoordinator.recv();

        assertEquals(sentCommitMessage.targetProtocolService.getAddress(), receivedCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentCommitMessage.sourceProtocolService.getAddress(), receivedCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommitMessage.dialogIdentifier, receivedCommitMessage.dialogIdentifier);
    }

    public void testSendRollbackWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        RollbackMessage sentRollbackMessage     = new RollbackMessage();
        RollbackMessage receivedRollbackMessage = null;

        sentRollbackMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentRollbackMessage.sourceProtocolService = new PortReference(_completionParticipantServiceURI);
        sentRollbackMessage.dialogIdentifier      = "123456";

        _completionParticipant.sendRollback(sentRollbackMessage);

        receivedRollbackMessage = (RollbackMessage) _completionCoordinator.recv();

        assertEquals(sentRollbackMessage.targetProtocolService.getAddress(), receivedRollbackMessage.targetProtocolService.getAddress());
        assertEquals(sentRollbackMessage.sourceProtocolService.getAddress(), receivedRollbackMessage.sourceProtocolService.getAddress());
        assertEquals(sentRollbackMessage.dialogIdentifier, receivedRollbackMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_completionParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _completionParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _completionCoordinator.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _completionParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionCoordinator.recv();

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

        sentCommitMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentCommitMessage.sourceProtocolService = new PortReference(_completionParticipantServiceURI);
        sentCommitMessage.dialogIdentifier      = null;

        _completionParticipant.sendCommit(sentCommitMessage);

        receivedCommitMessage = (CommitMessage) _completionCoordinator.recv(sentCommitMessage.dialogIdentifier);

        assertEquals(sentCommitMessage.targetProtocolService.getAddress(), receivedCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentCommitMessage.sourceProtocolService.getAddress(), receivedCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommitMessage.dialogIdentifier, receivedCommitMessage.dialogIdentifier);
    }

    public void testSendRollbackWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        RollbackMessage sentRollbackMessage     = new RollbackMessage();
        RollbackMessage receivedRollbackMessage = null;

        sentRollbackMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentRollbackMessage.sourceProtocolService = new PortReference(_completionParticipantServiceURI);
        sentRollbackMessage.dialogIdentifier      = null;

        _completionParticipant.sendRollback(sentRollbackMessage);

        receivedRollbackMessage = (RollbackMessage) _completionCoordinator.recv(sentRollbackMessage.dialogIdentifier);

        assertEquals(sentRollbackMessage.targetProtocolService.getAddress(), receivedRollbackMessage.targetProtocolService.getAddress());
        assertEquals(sentRollbackMessage.sourceProtocolService.getAddress(), receivedRollbackMessage.sourceProtocolService.getAddress());
        assertEquals(sentRollbackMessage.dialogIdentifier, receivedRollbackMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_completionParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _completionParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _completionCoordinator.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _completionParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionCoordinator.recv(sentErrorMessage.dialogIdentifier);

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

        sentCommitMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentCommitMessage.sourceProtocolService = new PortReference(_completionParticipantServiceURI);
        sentCommitMessage.dialogIdentifier      = "123456";

        _completionParticipant.sendCommit(sentCommitMessage);

        receivedCommitMessage = (CommitMessage) _completionCoordinator.recv(sentCommitMessage.dialogIdentifier);

        assertEquals(sentCommitMessage.targetProtocolService.getAddress(), receivedCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentCommitMessage.sourceProtocolService.getAddress(), receivedCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommitMessage.dialogIdentifier, receivedCommitMessage.dialogIdentifier);
    }

    public void testSendRollbackWithDialogIdentifierWithSelection()
        throws Exception
    {
        RollbackMessage sentRollbackMessage     = new RollbackMessage();
        RollbackMessage receivedRollbackMessage = null;

        sentRollbackMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentRollbackMessage.sourceProtocolService = new PortReference(_completionParticipantServiceURI);
        sentRollbackMessage.dialogIdentifier      = "123456";

        _completionParticipant.sendRollback(sentRollbackMessage);

        receivedRollbackMessage = (RollbackMessage) _completionCoordinator.recv(sentRollbackMessage.dialogIdentifier);

        assertEquals(sentRollbackMessage.targetProtocolService.getAddress(), receivedRollbackMessage.targetProtocolService.getAddress());
        assertEquals(sentRollbackMessage.sourceProtocolService.getAddress(), receivedRollbackMessage.sourceProtocolService.getAddress());
        assertEquals(sentRollbackMessage.dialogIdentifier, receivedRollbackMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_completionParticipantServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _completionParticipant.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _completionCoordinator.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _completionParticipant.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionCoordinator.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    protected void tearDown()
        throws Exception
    {
    }

    private CompletionParticipant _completionParticipant           = null;
    private String                _completionParticipantServiceURI = null;
    private CompletionCoordinator _completionCoordinator           = null;
    private String                _completionCoordinatorServiceURI = null;
}
