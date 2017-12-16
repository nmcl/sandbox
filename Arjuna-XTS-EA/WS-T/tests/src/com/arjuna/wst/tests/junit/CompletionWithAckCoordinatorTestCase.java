/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionWithAckCoordinatorTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class CompletionWithAckCoordinatorTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String completionWithAckCoordinatorJNDIName = "completionwithackcoordinator-msg";
        String completionWithAckParticipantJNDIName = "completionwithackparticipant";

        _completionWithAckCoordinator           = (CompletionWithAckCoordinator) initialContext.lookup(completionWithAckCoordinatorJNDIName);
        _completionWithAckCoordinatorServiceURI = _completionWithAckCoordinator.getServiceURI();
        _completionWithAckParticipant           = (CompletionWithAckParticipant) initialContext.lookup(completionWithAckParticipantJNDIName);
        _completionWithAckParticipantServiceURI = _completionWithAckParticipant.getServiceURI();
    }

    public void testSendCommittedWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        CommittedMessage sentCommittedMessage     = new CommittedMessage();
        CommittedMessage receivedCommittedMessage = null;

        sentCommittedMessage.targetProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentCommittedMessage.dialogIdentifier      = null;

        _completionWithAckCoordinator.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _completionWithAckParticipant.recv();

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentAbortedMessage.dialogIdentifier      = null;

        _completionWithAckCoordinator.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _completionWithAckParticipant.recv();

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _completionWithAckCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionWithAckParticipant.recv();

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

        sentCommittedMessage.targetProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentCommittedMessage.dialogIdentifier      = "123456";

        _completionWithAckCoordinator.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _completionWithAckParticipant.recv();

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentAbortedMessage.dialogIdentifier      = "123456";

        _completionWithAckCoordinator.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _completionWithAckParticipant.recv();

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _completionWithAckCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionWithAckParticipant.recv();

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

        sentCommittedMessage.targetProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentCommittedMessage.dialogIdentifier      = null;

        _completionWithAckCoordinator.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _completionWithAckParticipant.recv(sentCommittedMessage.dialogIdentifier);

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentAbortedMessage.dialogIdentifier      = null;

        _completionWithAckCoordinator.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _completionWithAckParticipant.recv(sentAbortedMessage.dialogIdentifier);

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _completionWithAckCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionWithAckParticipant.recv(sentErrorMessage.dialogIdentifier);

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

        sentCommittedMessage.targetProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentCommittedMessage.dialogIdentifier      = "123456";

        _completionWithAckCoordinator.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _completionWithAckParticipant.recv(sentCommittedMessage.dialogIdentifier);

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithDialogIdentifierWithSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_completionWithAckCoordinatorServiceURI);
        sentAbortedMessage.dialogIdentifier      = "123456";

        _completionWithAckCoordinator.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _completionWithAckParticipant.recv(sentAbortedMessage.dialogIdentifier);

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionWithAckParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _completionWithAckCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionWithAckParticipant.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    protected void tearDown()
        throws Exception
    {
    }

    private CompletionWithAckCoordinator _completionWithAckCoordinator           = null;
    private String                       _completionWithAckCoordinatorServiceURI = null;
    private CompletionWithAckParticipant _completionWithAckParticipant           = null;
    private String                       _completionWithAckParticipantServiceURI = null;
}
