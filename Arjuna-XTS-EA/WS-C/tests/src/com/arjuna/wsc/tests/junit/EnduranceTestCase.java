/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * EnduranceTestCase.java
 */

package com.arjuna.wsc.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.*;
import com.arjuna.wsc.impl.messaging.XMLUtil;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.tests.TestUtil;
import com.arjuna.wsc.tests.TestCoordinationContext;

public class EnduranceTestCase extends TestCase
{
    private static final long TEST_DURATION = 30 * 1000;

    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String activationRequesterJNDIName     = "activationrequester";
        String activationCoordinatorJNDIName   = "activationcoordinator-msg";
        String registrationRequesterJNDIName   = "registrationrequester";
        String registrationCoordinatorJNDIName = "registrationcoordinator-msg";

        _activationRequester               =(ActivationRequester) initialContext.lookup(activationRequesterJNDIName);
        _activationRequesterServiceURI     = _activationRequester.getServiceURI();
        _activationCoordinator             = (ActivationCoordinator) initialContext.lookup(activationCoordinatorJNDIName);
        _activationCoordinatorServiceURI   = _activationCoordinator.getServiceURI();
        _registrationRequester             = (RegistrationRequester) initialContext.lookup(registrationRequesterJNDIName);
        _registrationRequesterServiceURI   = _registrationRequester.getServiceURI();
        _registrationCoordinator           = (RegistrationCoordinator) initialContext.lookup(registrationCoordinatorJNDIName);
        _registrationCoordinatorServiceURI = _registrationCoordinator.getServiceURI();
    }

    public void testCreateCoordinationContextRequestWithoutDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
            doCreateCoordinationContextRequest(null);
    }

    public void testCreateCoordinationContextResponseWithoutDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
            doCreateCoordinationContextResponse(null);
    }

    public void testCreateCoordinationContextErrorWithoutDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
            doCreateCoordinationContextError(null);
    }

    public void testRegisterRequestWithoutDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
            doRegisterRequest(null);
    }

    public void testRegisterResponseWithoutDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
            doRegisterResponse(null);
    }

    public void testRegisterErrorWithoutDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
            doRegisterError(null);
    }

    public void testEachInTurnWithoutDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        int count = 0;
        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
        {
            if (count == 0)
                doCreateCoordinationContextRequest(null);
            else if (count == 1)
                doCreateCoordinationContextResponse(null);
            else if (count == 2)
                doCreateCoordinationContextError(null);
            else if (count == 3)
                doRegisterRequest(null);
            else if (count == 4)
                doRegisterResponse(null);
            else
                doRegisterError(null);

            count = (count + 1) % 6;
        }
    }

    public void testCreateCoordinationContextRequestWithDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        int dialogIdentifierNumber = 0;
        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
        {
            doCreateCoordinationContextRequest(Integer.toString(dialogIdentifierNumber));
            dialogIdentifierNumber++;
        }
    }

    public void testCreateCoordinationContextResponseWithDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        int dialogIdentifierNumber = 0;
        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
        {
            doCreateCoordinationContextResponse(Integer.toString(dialogIdentifierNumber));
            dialogIdentifierNumber++;
        }
    }

    public void testCreateCoordinationContextErrorWithDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        int dialogIdentifierNumber = 0;
        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
        {
            doCreateCoordinationContextError(Integer.toString(dialogIdentifierNumber));
            dialogIdentifierNumber++;
        }
    }

    public void testRegisterRequestWithDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        int dialogIdentifierNumber = 0;
        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
        {
            doRegisterRequest(Integer.toString(dialogIdentifierNumber));
            dialogIdentifierNumber++;
        }
    }

    public void testRegisterResponseWithDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        int dialogIdentifierNumber = 0;
        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
        {
            doRegisterResponse(Integer.toString(dialogIdentifierNumber));
            dialogIdentifierNumber++;
        }
    }

    public void testRegisterErrorWithDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        int dialogIdentifierNumber = 0;
        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
        {
            doRegisterError(Integer.toString(dialogIdentifierNumber));
            dialogIdentifierNumber++;
        }
    }

    public void testEachInTurnWithDialogIdentifier()
        throws Exception
    {
        long startTime = System.currentTimeMillis();

        int count                  = 0;
        int dialogIdentifierNumber = 0;
        while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
        {
            if (count == 0)
                doCreateCoordinationContextRequest(Integer.toString(dialogIdentifierNumber));
            else if (count == 1)
                doCreateCoordinationContextResponse(Integer.toString(dialogIdentifierNumber));
            else if (count == 2)
                doCreateCoordinationContextError(Integer.toString(dialogIdentifierNumber));
            else if (count == 3)
                doRegisterRequest(Integer.toString(dialogIdentifierNumber));
            else if (count == 4)
                doRegisterResponse(Integer.toString(dialogIdentifierNumber));
            else
                doRegisterError(Integer.toString(dialogIdentifierNumber));

            count = (count + 1) % 6;
            dialogIdentifierNumber++;
        }
    }

    public void doCreateCoordinationContextRequest(String dialogIdentifier)
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage     = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        sentCreateCoordinationContextMessage.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType   = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.dialogIdentifier   = dialogIdentifier;

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        assertEquals(sentCreateCoordinationContextMessage.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
    }

    public void doCreateCoordinationContextResponse(String dialogIdentifier)
        throws Exception
    {
        CreateCoordinationContextResponseMessage sentCreateCoordinationContextResponseMessage     = new CreateCoordinationContextResponseMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        TestCoordinationContext sentTestCoordinationContext                                  = new TestCoordinationContext();
        sentCreateCoordinationContextResponseMessage.requesterReference                      = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextResponseMessage.coordinationContext                     = sentTestCoordinationContext;
        sentCreateCoordinationContextResponseMessage.coordinationContext.identifier          = TestUtil.PROTOCOL_IDENTIFIER;
        sentCreateCoordinationContextResponseMessage.coordinationContext.coordinationType    = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextResponseMessage.coordinationContext.registrationService = new PortReference(TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL);
        sentTestCoordinationContext.extensionValue                                           = TestUtil.TEST_EXTENSION_VALUE;
        sentCreateCoordinationContextResponseMessage.dialogIdentifier                        = dialogIdentifier;

        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage);

        receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage.dialogIdentifier);

        TestCoordinationContext receivedTestCoordinationContext = (TestCoordinationContext) receivedCreateCoordinationContextResponseMessage.coordinationContext;
        assertEquals(sentCreateCoordinationContextResponseMessage.requesterReference.getAddress(), receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage.coordinationContext.identifier, receivedCreateCoordinationContextResponseMessage.coordinationContext.identifier);
        assertEquals(sentCreateCoordinationContextResponseMessage.coordinationContext.coordinationType, receivedCreateCoordinationContextResponseMessage.coordinationContext.coordinationType);
        assertEquals(sentCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress(), receivedCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress());
        assertEquals(sentTestCoordinationContext.extensionValue, receivedTestCoordinationContext.extensionValue);
        assertEquals(sentCreateCoordinationContextResponseMessage.dialogIdentifier, receivedCreateCoordinationContextResponseMessage.dialogIdentifier);
    }

    public void doCreateCoordinationContextError(String dialogIdentifier)
        throws Exception
    {
        ErrorMessage                             sentErrorMessage                                 = new ErrorMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_activationRequesterServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJCOOR_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = dialogIdentifier;

        _activationCoordinator.sendError(sentErrorMessage);

        try
        {
            receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentErrorMessage.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage receivedErrorMessage = errorMessageException.getErrorMessage();

            assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
            assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
            assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
            assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
        }
    }

    public void doRegisterRequest(String dialogIdentifier)
        throws Exception
    {
        RegisterMessage sentRegisterMessage     = new RegisterMessage();
        RegisterMessage receivedRegisterMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = TestUtil.PROTOCOL_IDENTIFIER;
        sentRegisterMessage.participantProtocolService = new PortReference(TestUtil.PROTOCOL_PARTICIPANT_SERVICE);
        sentRegisterMessage.dialogIdentifier           = dialogIdentifier;

        _registrationRequester.sendRegister(sentRegisterMessage);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);
    }

    public void doRegisterResponse(String dialogIdentifier)
        throws Exception
    {
        RegisterResponseMessage sentRegisterResponseMessage     = new RegisterResponseMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterResponseMessage.requesterReference          = new PortReference(_registrationRequesterServiceURI);
        sentRegisterResponseMessage.coordinationProtocolService = new PortReference(TestUtil.PROTOCOL_COORDINATOR_SERVICE);
        sentRegisterResponseMessage.dialogIdentifier            = dialogIdentifier;

        _registrationCoordinator.sendRegisterResponse(sentRegisterResponseMessage);

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterResponseMessage.dialogIdentifier);

        assertEquals(sentRegisterResponseMessage.requesterReference.getAddress(), receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(sentRegisterResponseMessage.coordinationProtocolService.getAddress(), receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertEquals(sentRegisterResponseMessage.dialogIdentifier, receivedRegisterResponseMessage.dialogIdentifier);
    }

    public void doRegisterError(String dialogIdentifier)
        throws Exception
    {
        ErrorMessage            sentErrorMessage                = new ErrorMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJCOOR_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = dialogIdentifier;

        _registrationCoordinator.sendError(sentErrorMessage);

        try
        {
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentErrorMessage.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage receivedErrorMessage = errorMessageException.getErrorMessage();

            assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
            assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
            assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
            assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
        }
    }

    protected void tearDown()
        throws Exception
    {
    }

    private ActivationRequester     _activationRequester               = null;
    private String                  _activationRequesterServiceURI     = null;
    private ActivationCoordinator   _activationCoordinator             = null;
    private String                  _activationCoordinatorServiceURI   = null;
    private RegistrationRequester   _registrationRequester             = null;
    private String                  _registrationRequesterServiceURI   = null;
    private RegistrationCoordinator _registrationCoordinator           = null;
    private String                  _registrationCoordinatorServiceURI = null;
}
