/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionCoordinatorTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class CompletionCoordinatorTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String completionCoordinatorJNDIName = "completioncoordinator-msg";
        String completionParticipantJNDIName = "completionparticipant";

        _completionCoordinator           = (CompletionCoordinator) initialContext.lookup(completionCoordinatorJNDIName);
        _completionCoordinatorServiceURI = _completionCoordinator.getServiceURI();
        _completionParticipant           = (CompletionParticipant) initialContext.lookup(completionParticipantJNDIName);
        _completionParticipantServiceURI = _completionParticipant.getServiceURI();
    }

    public void testSendCommittedWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        CommittedMessage sentCommittedMessage     = new CommittedMessage();
        CommittedMessage receivedCommittedMessage = null;

        sentCommittedMessage.targetProtocolService = new PortReference(_completionParticipantServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentCommittedMessage.dialogIdentifier      = null;

        _completionCoordinator.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _completionParticipant.recv();

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_completionParticipantServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentAbortedMessage.dialogIdentifier      = null;

        _completionCoordinator.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _completionParticipant.recv();

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _completionCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionParticipant.recv();

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

        sentCommittedMessage.targetProtocolService = new PortReference(_completionParticipantServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentCommittedMessage.dialogIdentifier      = "123456";

        _completionCoordinator.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _completionParticipant.recv();

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_completionParticipantServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentAbortedMessage.dialogIdentifier      = "123456";

        _completionCoordinator.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _completionParticipant.recv();

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithoutSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _completionCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionParticipant.recv();

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

        sentCommittedMessage.targetProtocolService = new PortReference(_completionParticipantServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentCommittedMessage.dialogIdentifier      = null;

        _completionCoordinator.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _completionParticipant.recv(sentCommittedMessage.dialogIdentifier);

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_completionParticipantServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentAbortedMessage.dialogIdentifier      = null;

        _completionCoordinator.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _completionParticipant.recv(sentAbortedMessage.dialogIdentifier);

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendErrorWithoutDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

        _completionCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionParticipant.recv(sentErrorMessage.dialogIdentifier);

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

        sentCommittedMessage.targetProtocolService = new PortReference(_completionParticipantServiceURI);
        sentCommittedMessage.sourceProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentCommittedMessage.dialogIdentifier      = "123456";

        _completionCoordinator.sendCommitted(sentCommittedMessage);

        receivedCommittedMessage = (CommittedMessage) _completionParticipant.recv(sentCommittedMessage.dialogIdentifier);

        assertEquals(sentCommittedMessage.targetProtocolService.getAddress(), receivedCommittedMessage.targetProtocolService.getAddress());
        assertEquals(sentCommittedMessage.sourceProtocolService.getAddress(), receivedCommittedMessage.sourceProtocolService.getAddress());
        assertEquals(sentCommittedMessage.dialogIdentifier, receivedCommittedMessage.dialogIdentifier);
    }

    public void testSendAbortedWithDialogIdentifierWithSelection()
        throws Exception
    {
        AbortedMessage sentAbortedMessage     = new AbortedMessage();
        AbortedMessage receivedAbortedMessage = null;

        sentAbortedMessage.targetProtocolService = new PortReference(_completionParticipantServiceURI);
        sentAbortedMessage.sourceProtocolService = new PortReference(_completionCoordinatorServiceURI);
        sentAbortedMessage.dialogIdentifier      = "123456";

        _completionCoordinator.sendAborted(sentAbortedMessage);

        receivedAbortedMessage = (AbortedMessage) _completionParticipant.recv(sentAbortedMessage.dialogIdentifier);

        assertEquals(sentAbortedMessage.targetProtocolService.getAddress(), receivedAbortedMessage.targetProtocolService.getAddress());
        assertEquals(sentAbortedMessage.sourceProtocolService.getAddress(), receivedAbortedMessage.sourceProtocolService.getAddress());
        assertEquals(sentAbortedMessage.dialogIdentifier, receivedAbortedMessage.dialogIdentifier);
    }

    public void testSendErrorWithDialogIdentifierWithSelection()
        throws Exception
    {
        ErrorMessage sentErrorMessage     = new ErrorMessage();
        ErrorMessage receivedErrorMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_completionParticipantServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJTX_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

        _completionCoordinator.sendError(sentErrorMessage);

        receivedErrorMessage = (ErrorMessage) _completionParticipant.recv(sentErrorMessage.dialogIdentifier);

        assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
        assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
        assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
        assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
    }

    protected void tearDown()
        throws Exception
    {
    }

    private CompletionCoordinator _completionCoordinator           = null;
    private String                _completionCoordinatorServiceURI = null;
    private CompletionParticipant _completionParticipant           = null;
    private String                _completionParticipantServiceURI = null;
}
