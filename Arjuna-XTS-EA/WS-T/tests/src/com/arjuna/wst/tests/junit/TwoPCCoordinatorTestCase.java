/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TwoPCCoordinatorTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class TwoPCCoordinatorTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String twoPCCoordinatorJNDIName = "twopccoordinator";
        String twoPCParticipantJNDIName = "twopcparticipant-msg";

        _twoPCCoordinator           = (TwoPCCoordinator) initialContext.lookup(twoPCCoordinatorJNDIName);
        _twoPCCoordinatorServiceURI = _twoPCCoordinator.getServiceURI();
        _twoPCParticipant           = (TwoPCParticipant) initialContext.lookup(twoPCParticipantJNDIName);
        _twoPCParticipantServiceURI = _twoPCParticipant.getServiceURI();
    }

    public void testSendPrepareWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        PrepareMessage sentPrepareMessage     = new PrepareMessage();
        PrepareMessage receivedPrepareMessage = null;

        sentPrepareMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentPrepareMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentPrepareMessage.dialogIdentifier      = null;

        _twoPCCoordinator.sendPrepare(sentPrepareMessage);

        receivedPrepareMessage = (PrepareMessage) _twoPCParticipant.recv();

        assertEquals(sentPrepareMessage.targetProtocolService.getAddress(), receivedPrepareMessage.targetProtocolService.getAddress());
        assertEquals(sentPrepareMessage.sourceProtocolService.getAddress(), receivedPrepareMessage.sourceProtocolService.getAddress());
        assertEquals(sentPrepareMessage.dialogIdentifier, receivedPrepareMessage.dialogIdentifier);
    }

    public void testSendOnePhaseCommitWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        OnePhaseCommitMessage sentOnePhaseCommitMessage     = new OnePhaseCommitMessage();
        OnePhaseCommitMessage receivedOnePhaseCommitMessage = null;

        sentOnePhaseCommitMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentOnePhaseCommitMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentOnePhaseCommitMessage.dialogIdentifier      = null;

        _twoPCCoordinator.sendOnePhaseCommit(sentOnePhaseCommitMessage);

        receivedOnePhaseCommitMessage = (OnePhaseCommitMessage) _twoPCParticipant.recv();

        assertEquals(sentOnePhaseCommitMessage.targetProtocolService.getAddress(), receivedOnePhaseCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentOnePhaseCommitMessage.sourceProtocolService.getAddress(), receivedOnePhaseCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentOnePhaseCommitMessage.dialogIdentifier, receivedOnePhaseCommitMessage.dialogIdentifier);
    }

    public void testSendCommitWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        CommitMessage sentCommitMessage     = new CommitMessage();
        CommitMessage receivedCommitMessage = null;

        sentCommitMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentCommitMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentCommitMessage.dialogIdentifier      = null;

        _twoPCCoordinator.sendCommit(sentCommitMessage);

        receivedCommitMessage = (CommitMessage) _twoPCParticipant.recv();

        assertEquals(sentCommitMessage.targetProtocolService.getAddress(), receivedCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentCommitMessage.sourceProtocolService.getAddress(), receivedCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommitMessage.dialogIdentifier, receivedCommitMessage.dialogIdentifier);
    }

    public void testSendRollbackWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        RollbackMessage sentRollbackMessage     = new RollbackMessage();
        RollbackMessage receivedRollbackMessage = null;

        sentRollbackMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentRollbackMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentRollbackMessage.dialogIdentifier      = null;

        _twoPCCoordinator.sendRollback(sentRollbackMessage);

        receivedRollbackMessage = (RollbackMessage) _twoPCParticipant.recv();

        assertEquals(sentRollbackMessage.targetProtocolService.getAddress(), receivedRollbackMessage.targetProtocolService.getAddress());
        assertEquals(sentRollbackMessage.sourceProtocolService.getAddress(), receivedRollbackMessage.sourceProtocolService.getAddress());
        assertEquals(sentRollbackMessage.dialogIdentifier, receivedRollbackMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _twoPCCoordinator.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _twoPCParticipant.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _twoPCCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _twoPCParticipant.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendPrepareWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        PrepareMessage sentPrepareMessage     = new PrepareMessage();
        PrepareMessage receivedPrepareMessage = null;

        sentPrepareMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentPrepareMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentPrepareMessage.dialogIdentifier      = "123456";

        _twoPCCoordinator.sendPrepare(sentPrepareMessage);

        receivedPrepareMessage = (PrepareMessage) _twoPCParticipant.recv();

        assertEquals(sentPrepareMessage.targetProtocolService.getAddress(), receivedPrepareMessage.targetProtocolService.getAddress());
        assertEquals(sentPrepareMessage.sourceProtocolService.getAddress(), receivedPrepareMessage.sourceProtocolService.getAddress());
        assertEquals(sentPrepareMessage.dialogIdentifier, receivedPrepareMessage.dialogIdentifier);
    }

    public void testSendOnePhaseCommitWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        OnePhaseCommitMessage sentOnePhaseCommitMessage     = new OnePhaseCommitMessage();
        OnePhaseCommitMessage receivedOnePhaseCommitMessage = null;

        sentOnePhaseCommitMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentOnePhaseCommitMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentOnePhaseCommitMessage.dialogIdentifier      = "123456";

        _twoPCCoordinator.sendOnePhaseCommit(sentOnePhaseCommitMessage);

        receivedOnePhaseCommitMessage = (OnePhaseCommitMessage) _twoPCParticipant.recv();

        assertEquals(sentOnePhaseCommitMessage.targetProtocolService.getAddress(), receivedOnePhaseCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentOnePhaseCommitMessage.sourceProtocolService.getAddress(), receivedOnePhaseCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentOnePhaseCommitMessage.dialogIdentifier, receivedOnePhaseCommitMessage.dialogIdentifier);
    }

    public void testSendCommitWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        CommitMessage sentCommitMessage     = new CommitMessage();
        CommitMessage receivedCommitMessage = null;

        sentCommitMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentCommitMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentCommitMessage.dialogIdentifier      = "123456";

        _twoPCCoordinator.sendCommit(sentCommitMessage);

        receivedCommitMessage = (CommitMessage) _twoPCParticipant.recv();

        assertEquals(sentCommitMessage.targetProtocolService.getAddress(), receivedCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentCommitMessage.sourceProtocolService.getAddress(), receivedCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommitMessage.dialogIdentifier, receivedCommitMessage.dialogIdentifier);
    }

    public void testSendRollbackWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        RollbackMessage sentRollbackMessage     = new RollbackMessage();
        RollbackMessage receivedRollbackMessage = null;

        sentRollbackMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentRollbackMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentRollbackMessage.dialogIdentifier      = "123456";

        _twoPCCoordinator.sendRollback(sentRollbackMessage);

        receivedRollbackMessage = (RollbackMessage) _twoPCParticipant.recv();

        assertEquals(sentRollbackMessage.targetProtocolService.getAddress(), receivedRollbackMessage.targetProtocolService.getAddress());
        assertEquals(sentRollbackMessage.sourceProtocolService.getAddress(), receivedRollbackMessage.sourceProtocolService.getAddress());
        assertEquals(sentRollbackMessage.dialogIdentifier, receivedRollbackMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _twoPCCoordinator.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _twoPCParticipant.recv();

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _twoPCCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _twoPCParticipant.recv();

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendPrepareWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        PrepareMessage sentPrepareMessage     = new PrepareMessage();
        PrepareMessage receivedPrepareMessage = null;

        sentPrepareMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentPrepareMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentPrepareMessage.dialogIdentifier      = null;

        _twoPCCoordinator.sendPrepare(sentPrepareMessage);

        receivedPrepareMessage = (PrepareMessage) _twoPCParticipant.recv(sentPrepareMessage.dialogIdentifier);

        assertEquals(sentPrepareMessage.targetProtocolService.getAddress(), receivedPrepareMessage.targetProtocolService.getAddress());
        assertEquals(sentPrepareMessage.sourceProtocolService.getAddress(), receivedPrepareMessage.sourceProtocolService.getAddress());
        assertEquals(sentPrepareMessage.dialogIdentifier, receivedPrepareMessage.dialogIdentifier);
    }

    public void testSendOnePhaseCommitWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        OnePhaseCommitMessage sentOnePhaseCommitMessage     = new OnePhaseCommitMessage();
        OnePhaseCommitMessage receivedOnePhaseCommitMessage = null;

        sentOnePhaseCommitMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentOnePhaseCommitMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentOnePhaseCommitMessage.dialogIdentifier      = null;

        _twoPCCoordinator.sendOnePhaseCommit(sentOnePhaseCommitMessage);

        receivedOnePhaseCommitMessage = (OnePhaseCommitMessage) _twoPCParticipant.recv(sentOnePhaseCommitMessage.dialogIdentifier);

        assertEquals(sentOnePhaseCommitMessage.targetProtocolService.getAddress(), receivedOnePhaseCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentOnePhaseCommitMessage.sourceProtocolService.getAddress(), receivedOnePhaseCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentOnePhaseCommitMessage.dialogIdentifier, receivedOnePhaseCommitMessage.dialogIdentifier);
    }

    public void testSendCommitWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        CommitMessage sentCommitMessage     = new CommitMessage();
        CommitMessage receivedCommitMessage = null;

        sentCommitMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentCommitMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentCommitMessage.dialogIdentifier      = null;

        _twoPCCoordinator.sendCommit(sentCommitMessage);

        receivedCommitMessage = (CommitMessage) _twoPCParticipant.recv(sentCommitMessage.dialogIdentifier);

        assertEquals(sentCommitMessage.targetProtocolService.getAddress(), receivedCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentCommitMessage.sourceProtocolService.getAddress(), receivedCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommitMessage.dialogIdentifier, receivedCommitMessage.dialogIdentifier);
    }

    public void testSendRollbackWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        RollbackMessage sentRollbackMessage     = new RollbackMessage();
        RollbackMessage receivedRollbackMessage = null;

        sentRollbackMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentRollbackMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentRollbackMessage.dialogIdentifier      = null;

        _twoPCCoordinator.sendRollback(sentRollbackMessage);

        receivedRollbackMessage = (RollbackMessage) _twoPCParticipant.recv(sentRollbackMessage.dialogIdentifier);

        assertEquals(sentRollbackMessage.targetProtocolService.getAddress(), receivedRollbackMessage.targetProtocolService.getAddress());
        assertEquals(sentRollbackMessage.sourceProtocolService.getAddress(), receivedRollbackMessage.sourceProtocolService.getAddress());
        assertEquals(sentRollbackMessage.dialogIdentifier, receivedRollbackMessage.dialogIdentifier);
    }

    public void testSendUnknownWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentUnknownMessage.dialogIdentifier      = null;

        _twoPCCoordinator.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _twoPCParticipant.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _twoPCCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _twoPCParticipant.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    public void testSendPrepareWithDialogIdentifierWithSelection()
        throws Exception
    {
        PrepareMessage sentPrepareMessage     = new PrepareMessage();
        PrepareMessage receivedPrepareMessage = null;

        sentPrepareMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentPrepareMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentPrepareMessage.dialogIdentifier      = "123456";

        _twoPCCoordinator.sendPrepare(sentPrepareMessage);

        receivedPrepareMessage = (PrepareMessage) _twoPCParticipant.recv(sentPrepareMessage.dialogIdentifier);

        assertEquals(sentPrepareMessage.targetProtocolService.getAddress(), receivedPrepareMessage.targetProtocolService.getAddress());
        assertEquals(sentPrepareMessage.sourceProtocolService.getAddress(), receivedPrepareMessage.sourceProtocolService.getAddress());
        assertEquals(sentPrepareMessage.dialogIdentifier, receivedPrepareMessage.dialogIdentifier);
    }

    public void testSendOnePhaseCommitWithDialogIdentifierWithSelection()
        throws Exception
    {
        OnePhaseCommitMessage sentOnePhaseCommitMessage     = new OnePhaseCommitMessage();
        OnePhaseCommitMessage receivedOnePhaseCommitMessage = null;

        sentOnePhaseCommitMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentOnePhaseCommitMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentOnePhaseCommitMessage.dialogIdentifier      = "123456";

        _twoPCCoordinator.sendOnePhaseCommit(sentOnePhaseCommitMessage);

        receivedOnePhaseCommitMessage = (OnePhaseCommitMessage) _twoPCParticipant.recv(sentOnePhaseCommitMessage.dialogIdentifier);

        assertEquals(sentOnePhaseCommitMessage.targetProtocolService.getAddress(), receivedOnePhaseCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentOnePhaseCommitMessage.sourceProtocolService.getAddress(), receivedOnePhaseCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentOnePhaseCommitMessage.dialogIdentifier, receivedOnePhaseCommitMessage.dialogIdentifier);
    }

    public void testSendCommitWithDialogIdentifierWithSelection()
        throws Exception
    {
        CommitMessage sentCommitMessage     = new CommitMessage();
        CommitMessage receivedCommitMessage = null;

        sentCommitMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentCommitMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentCommitMessage.dialogIdentifier      = "123456";

        _twoPCCoordinator.sendCommit(sentCommitMessage);

        receivedCommitMessage = (CommitMessage) _twoPCParticipant.recv(sentCommitMessage.dialogIdentifier);

        assertEquals(sentCommitMessage.targetProtocolService.getAddress(), receivedCommitMessage.targetProtocolService.getAddress());
        assertEquals(sentCommitMessage.sourceProtocolService.getAddress(), receivedCommitMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommitMessage.dialogIdentifier, receivedCommitMessage.dialogIdentifier);
    }

    public void testSendRollbackWithDialogIdentifierWithSelection()
        throws Exception
    {
        RollbackMessage sentRollbackMessage     = new RollbackMessage();
        RollbackMessage receivedRollbackMessage = null;

        sentRollbackMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentRollbackMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentRollbackMessage.dialogIdentifier      = "123456";

        _twoPCCoordinator.sendRollback(sentRollbackMessage);

        receivedRollbackMessage = (RollbackMessage) _twoPCParticipant.recv(sentRollbackMessage.dialogIdentifier);

        assertEquals(sentRollbackMessage.targetProtocolService.getAddress(), receivedRollbackMessage.targetProtocolService.getAddress());
        assertEquals(sentRollbackMessage.sourceProtocolService.getAddress(), receivedRollbackMessage.sourceProtocolService.getAddress());
        assertEquals(sentRollbackMessage.dialogIdentifier, receivedRollbackMessage.dialogIdentifier);
    }

    public void testSendUnknownWithDialogIdentifierWithSelection()
        throws Exception
    {
        UnknownMessage sentUnknownMessage     = new UnknownMessage();
        UnknownMessage receivedUnknownMessage = null;

        sentUnknownMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentUnknownMessage.sourceProtocolService = new PortReference(_twoPCCoordinatorServiceURI);
        sentUnknownMessage.dialogIdentifier      = "123456";

        _twoPCCoordinator.sendUnknown(sentUnknownMessage);

        receivedUnknownMessage = (UnknownMessage) _twoPCParticipant.recv(sentUnknownMessage.dialogIdentifier);

        assertEquals(sentUnknownMessage.targetProtocolService.getAddress(), receivedUnknownMessage.targetProtocolService.getAddress());
        assertEquals(sentUnknownMessage.sourceProtocolService.getAddress(), receivedUnknownMessage.sourceProtocolService.getAddress());
        assertEquals(sentUnknownMessage.dialogIdentifier, receivedUnknownMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_twoPCParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _twoPCCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _twoPCParticipant.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    protected void tearDown()
        throws Exception
    {
    }

    private TwoPCCoordinator _twoPCCoordinator           = null;
    private String           _twoPCCoordinatorServiceURI = null;
    private TwoPCParticipant _twoPCParticipant           = null;
    private String           _twoPCParticipantServiceURI = null;
}
