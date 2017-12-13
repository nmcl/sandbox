/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ActivationTestCase.java
 */

package com.arjuna.wsc.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.*;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.impl.messaging.XMLUtil;
import com.arjuna.wsc.tests.TestUtil;
import com.arjuna.wsc.tests.TestCoordinationContext;

public class ActivationTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String activationRequesterJNDIName   = "activationrequester";
        String activationCoordinatorJNDIName = "activationcoordinator-msg";

        _activationRequester             = (ActivationRequester) initialContext.lookup(activationRequesterJNDIName);
        _activationRequesterServiceURI   = _activationRequester.getServiceURI();
        _activationCoordinator           = (ActivationCoordinator) initialContext.lookup(activationCoordinatorJNDIName);
        _activationCoordinatorServiceURI = _activationCoordinator.getServiceURI();
    }

    public void testRequestWithoutExpiresWithoutCurrentContextWithoutDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage     = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        sentCreateCoordinationContextMessage.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.expires            = null;
        sentCreateCoordinationContextMessage.currentContext     = null;
        sentCreateCoordinationContextMessage.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType   = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.dialogIdentifier   = null;

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        assertEquals(sentCreateCoordinationContextMessage.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.expires, receivedCreateCoordinationContextMessage.expires);
        assertEquals(sentCreateCoordinationContextMessage.currentContext, receivedCreateCoordinationContextMessage.currentContext);
        assertEquals(sentCreateCoordinationContextMessage.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
   }

    public void testRequestWithExpiresWithoutCurrentContextWithoutDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage     = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        sentCreateCoordinationContextMessage.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.expires            = new AttributedDateTime();
        sentCreateCoordinationContextMessage.expires.dataTime   = "TestDateTime";
        sentCreateCoordinationContextMessage.expires.valueType  = "TestValueType";
        sentCreateCoordinationContextMessage.currentContext     = null;
        sentCreateCoordinationContextMessage.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType   = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.dialogIdentifier   = null;

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        assertEquals(sentCreateCoordinationContextMessage.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.expires.dataTime, receivedCreateCoordinationContextMessage.expires.dataTime);
        assertEquals(sentCreateCoordinationContextMessage.expires.valueType, receivedCreateCoordinationContextMessage.expires.valueType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext, receivedCreateCoordinationContextMessage.currentContext);
        assertEquals(sentCreateCoordinationContextMessage.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
    }

    public void testRequestWithoutExpiresWithCurrentContextWithoutExpiresWithoutDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage     = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        TestCoordinationContext sentTestCurrentContext                          = new TestCoordinationContext();
        sentCreateCoordinationContextMessage.activationService                  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.expires                            = null;
        sentCreateCoordinationContextMessage.currentContext                     = sentTestCurrentContext;
        sentCreateCoordinationContextMessage.currentContext.identifier          = TestUtil.PROTOCOL_IDENTIFIER;
        sentCreateCoordinationContextMessage.currentContext.expires             = null;
        sentCreateCoordinationContextMessage.currentContext.coordinationType    = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.currentContext.registrationService = new PortReference(TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL);
        sentCreateCoordinationContextMessage.requesterReference                 = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType                   = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.dialogIdentifier                   = null;

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        TestCoordinationContext receivedTestCurrentContext = (TestCoordinationContext) receivedCreateCoordinationContextMessage.currentContext;
        assertEquals(sentCreateCoordinationContextMessage.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.expires, receivedCreateCoordinationContextMessage.expires);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.identifier, receivedCreateCoordinationContextMessage.currentContext.identifier);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.expires, receivedCreateCoordinationContextMessage.currentContext.expires);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.coordinationType, receivedCreateCoordinationContextMessage.currentContext.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.registrationService.getAddress(), receivedCreateCoordinationContextMessage.currentContext.registrationService.getAddress());
        assertEquals(sentTestCurrentContext.extensionValue, receivedTestCurrentContext.extensionValue);
        assertEquals(sentCreateCoordinationContextMessage.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
    }

    public void testRequestWithoutExpiresWithCurrentContextWithExpiresWithoutDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage     = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        TestCoordinationContext sentTestCurrentContext                          = new TestCoordinationContext();
        sentCreateCoordinationContextMessage.activationService                  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.expires                            = null;
        sentCreateCoordinationContextMessage.currentContext                     = sentTestCurrentContext;
        sentCreateCoordinationContextMessage.currentContext.identifier          = TestUtil.PROTOCOL_IDENTIFIER;
        sentCreateCoordinationContextMessage.currentContext.expires             = new AttributedDateTime();
        sentCreateCoordinationContextMessage.currentContext.expires.dataTime    = "CC-TestDateTime";
        sentCreateCoordinationContextMessage.currentContext.expires.valueType   = "CC-TestValueType";
        sentCreateCoordinationContextMessage.currentContext.coordinationType    = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.currentContext.registrationService = new PortReference(TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL);
        sentCreateCoordinationContextMessage.requesterReference                 = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType                   = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.dialogIdentifier                   = null;

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        TestCoordinationContext receivedTestCurrentContext = (TestCoordinationContext) receivedCreateCoordinationContextMessage.currentContext;
        assertEquals(sentCreateCoordinationContextMessage.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.expires, receivedCreateCoordinationContextMessage.expires);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.identifier, receivedCreateCoordinationContextMessage.currentContext.identifier);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.expires.dataTime, receivedCreateCoordinationContextMessage.currentContext.expires.dataTime);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.expires.valueType, receivedCreateCoordinationContextMessage.currentContext.expires.valueType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.coordinationType, receivedCreateCoordinationContextMessage.currentContext.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.registrationService.getAddress(), receivedCreateCoordinationContextMessage.currentContext.registrationService.getAddress());
        assertEquals(sentTestCurrentContext.extensionValue, receivedTestCurrentContext.extensionValue);
        assertEquals(sentCreateCoordinationContextMessage.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
    }

    public void testRequestWithExpiresWithCurrentContextWithoutExpiresWithoutDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage     = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        TestCoordinationContext sentTestCurrentContext                          = new TestCoordinationContext();
        sentCreateCoordinationContextMessage.activationService                  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.expires                            = new AttributedDateTime();
        sentCreateCoordinationContextMessage.expires.dataTime                   = "TestDateTime";
        sentCreateCoordinationContextMessage.expires.valueType                  = "TestValueType";
        sentCreateCoordinationContextMessage.currentContext                     = sentTestCurrentContext;
        sentCreateCoordinationContextMessage.currentContext.identifier          = TestUtil.PROTOCOL_IDENTIFIER;
        sentCreateCoordinationContextMessage.currentContext.expires             = null;
        sentCreateCoordinationContextMessage.currentContext.coordinationType    = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.currentContext.registrationService = new PortReference(TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL);
        sentCreateCoordinationContextMessage.requesterReference                 = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType                   = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.dialogIdentifier                   = null;

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        TestCoordinationContext receivedTestCurrentContext = (TestCoordinationContext) receivedCreateCoordinationContextMessage.currentContext;
        assertEquals(sentCreateCoordinationContextMessage.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.expires.dataTime, receivedCreateCoordinationContextMessage.expires.dataTime);
        assertEquals(sentCreateCoordinationContextMessage.expires.valueType, receivedCreateCoordinationContextMessage.expires.valueType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.identifier, receivedCreateCoordinationContextMessage.currentContext.identifier);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.expires, receivedCreateCoordinationContextMessage.currentContext.expires);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.coordinationType, receivedCreateCoordinationContextMessage.currentContext.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.registrationService.getAddress(), receivedCreateCoordinationContextMessage.currentContext.registrationService.getAddress());
        assertEquals(sentTestCurrentContext.extensionValue, receivedTestCurrentContext.extensionValue);
        assertEquals(sentCreateCoordinationContextMessage.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
    }

    public void testRequestWithExpiresWithCurrentContextWithExpiresWithoutDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage     = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        TestCoordinationContext sentTestCurrentContext                          = new TestCoordinationContext();
        sentCreateCoordinationContextMessage.activationService                  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.expires                            = new AttributedDateTime();
        sentCreateCoordinationContextMessage.expires.dataTime                   = "TestDateTime";
        sentCreateCoordinationContextMessage.expires.valueType                  = "TestValueType";
        sentCreateCoordinationContextMessage.currentContext                     = sentTestCurrentContext;
        sentCreateCoordinationContextMessage.currentContext.identifier          = TestUtil.PROTOCOL_IDENTIFIER;
        sentCreateCoordinationContextMessage.currentContext.expires             = new AttributedDateTime();
        sentCreateCoordinationContextMessage.currentContext.expires.dataTime    = "CC-TestDateTime";
        sentCreateCoordinationContextMessage.currentContext.expires.valueType   = "CC-TestValueType";
        sentCreateCoordinationContextMessage.currentContext.coordinationType    = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.currentContext.registrationService = new PortReference(TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL);
        sentCreateCoordinationContextMessage.requesterReference                 = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType                   = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.dialogIdentifier                   = null;

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        TestCoordinationContext receivedTestCurrentContext = (TestCoordinationContext) receivedCreateCoordinationContextMessage.currentContext;
        assertEquals(sentCreateCoordinationContextMessage.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.expires.dataTime, receivedCreateCoordinationContextMessage.expires.dataTime);
        assertEquals(sentCreateCoordinationContextMessage.expires.valueType, receivedCreateCoordinationContextMessage.expires.valueType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.identifier, receivedCreateCoordinationContextMessage.currentContext.identifier);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.expires.dataTime, receivedCreateCoordinationContextMessage.currentContext.expires.dataTime);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.expires.valueType, receivedCreateCoordinationContextMessage.currentContext.expires.valueType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.coordinationType, receivedCreateCoordinationContextMessage.currentContext.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.registrationService.getAddress(), receivedCreateCoordinationContextMessage.currentContext.registrationService.getAddress());
        assertEquals(sentTestCurrentContext.extensionValue, receivedTestCurrentContext.extensionValue);
        assertEquals(sentCreateCoordinationContextMessage.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
    }

    public void testResponseWithContextWithoutDialogIdentifier()
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
        sentCreateCoordinationContextResponseMessage.dialogIdentifier                        = null;

        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage);

        receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage.dialogIdentifier);

        TestCoordinationContext receivedTestCoordinationContext = (TestCoordinationContext) receivedCreateCoordinationContextResponseMessage.coordinationContext;
        assertEquals(sentCreateCoordinationContextResponseMessage.requesterReference.getAddress(), receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage.coordinationContext.identifier, receivedCreateCoordinationContextResponseMessage.coordinationContext.identifier);
        assertEquals(sentCreateCoordinationContextResponseMessage.coordinationContext.coordinationType, receivedCreateCoordinationContextResponseMessage.coordinationContext.coordinationType);
        assertEquals(sentCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress(), receivedCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress());
        assertEquals(sentTestCoordinationContext.extensionValue, receivedTestCoordinationContext.extensionValue);
        assertNull(receivedCreateCoordinationContextResponseMessage.dialogIdentifier);
    }

    public void testResponseNoContextWithoutDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextResponseMessage sentCreateCoordinationContextResponseMessage     = new CreateCoordinationContextResponseMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        sentCreateCoordinationContextResponseMessage.requesterReference  = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextResponseMessage.coordinationContext = null;
        sentCreateCoordinationContextResponseMessage.dialogIdentifier    = null;

        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage);

        receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage.dialogIdentifier);

        assertEquals(sentCreateCoordinationContextResponseMessage.requesterReference.getAddress(), receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress());
        assertNull(receivedCreateCoordinationContextResponseMessage.coordinationContext);
        assertNull(receivedCreateCoordinationContextResponseMessage.dialogIdentifier);
    }

    public void testErrorWithoutDialogIdentifier()
        throws Exception
    {
        ErrorMessage                             sentErrorMessage                                 = new ErrorMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_activationRequesterServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJCOOR_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

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
            assertNull(receivedErrorMessage.dialogIdentifier);
        }
    }

    public void testRequestWithoutExpiresWithoutCurrentContextWithDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage     = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        sentCreateCoordinationContextMessage.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.expires            = null;
        sentCreateCoordinationContextMessage.currentContext     = null;
        sentCreateCoordinationContextMessage.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType   = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.dialogIdentifier   = "123456";

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        assertEquals(sentCreateCoordinationContextMessage.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.expires, receivedCreateCoordinationContextMessage.expires);
        assertEquals(sentCreateCoordinationContextMessage.currentContext, receivedCreateCoordinationContextMessage.currentContext);
        assertEquals(sentCreateCoordinationContextMessage.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
   }

    public void testRequestWithExpiresWithoutCurrentContextWithDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage     = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        sentCreateCoordinationContextMessage.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.expires            = new AttributedDateTime();
        sentCreateCoordinationContextMessage.expires.dataTime   = "TestDateTime";
        sentCreateCoordinationContextMessage.expires.valueType  = "TestValueType";
        sentCreateCoordinationContextMessage.currentContext     = null;
        sentCreateCoordinationContextMessage.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType   = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.dialogIdentifier   = "123456";

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        assertEquals(sentCreateCoordinationContextMessage.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.expires.dataTime, receivedCreateCoordinationContextMessage.expires.dataTime);
        assertEquals(sentCreateCoordinationContextMessage.expires.valueType, receivedCreateCoordinationContextMessage.expires.valueType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext, receivedCreateCoordinationContextMessage.currentContext);
        assertEquals(sentCreateCoordinationContextMessage.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
    }

    public void testRequestWithoutExpiresWithCurrentContextWithoutExpiresWithDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage     = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        TestCoordinationContext sentTestCurrentContext                          = new TestCoordinationContext();
        sentCreateCoordinationContextMessage.activationService                  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.expires                            = null;
        sentCreateCoordinationContextMessage.currentContext                     = sentTestCurrentContext;
        sentCreateCoordinationContextMessage.currentContext.identifier          = TestUtil.PROTOCOL_IDENTIFIER;
        sentCreateCoordinationContextMessage.currentContext.expires             = null;
        sentCreateCoordinationContextMessage.currentContext.coordinationType    = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.currentContext.registrationService = new PortReference(TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL);
        sentCreateCoordinationContextMessage.requesterReference                 = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType                   = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.dialogIdentifier                   = "123456";

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        TestCoordinationContext receivedTestCurrentContext = (TestCoordinationContext) receivedCreateCoordinationContextMessage.currentContext;
        assertEquals(sentCreateCoordinationContextMessage.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.expires, receivedCreateCoordinationContextMessage.expires);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.identifier, receivedCreateCoordinationContextMessage.currentContext.identifier);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.expires, receivedCreateCoordinationContextMessage.currentContext.expires);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.coordinationType, receivedCreateCoordinationContextMessage.currentContext.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.registrationService.getAddress(), receivedCreateCoordinationContextMessage.currentContext.registrationService.getAddress());
        assertEquals(sentTestCurrentContext.extensionValue, receivedTestCurrentContext.extensionValue);
        assertEquals(sentCreateCoordinationContextMessage.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
    }

    public void testRequestWithoutExpiresWithCurrentContextWithExpiresWithDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage     = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        TestCoordinationContext sentTestCurrentContext                          = new TestCoordinationContext();
        sentCreateCoordinationContextMessage.activationService                  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.expires                            = null;
        sentCreateCoordinationContextMessage.currentContext                     = sentTestCurrentContext;
        sentCreateCoordinationContextMessage.currentContext.identifier          = TestUtil.PROTOCOL_IDENTIFIER;
        sentCreateCoordinationContextMessage.currentContext.expires             = new AttributedDateTime();
        sentCreateCoordinationContextMessage.currentContext.expires.dataTime    = "CC-TestDateTime";
        sentCreateCoordinationContextMessage.currentContext.expires.valueType   = "CC-TestValueType";
        sentCreateCoordinationContextMessage.currentContext.coordinationType    = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.currentContext.registrationService = new PortReference(TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL);
        sentCreateCoordinationContextMessage.requesterReference                 = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType                   = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.dialogIdentifier                   = "123456";

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        TestCoordinationContext receivedTestCurrentContext = (TestCoordinationContext) receivedCreateCoordinationContextMessage.currentContext;
        assertEquals(sentCreateCoordinationContextMessage.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.expires, receivedCreateCoordinationContextMessage.expires);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.identifier, receivedCreateCoordinationContextMessage.currentContext.identifier);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.expires.dataTime, receivedCreateCoordinationContextMessage.currentContext.expires.dataTime);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.expires.valueType, receivedCreateCoordinationContextMessage.currentContext.expires.valueType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.coordinationType, receivedCreateCoordinationContextMessage.currentContext.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.registrationService.getAddress(), receivedCreateCoordinationContextMessage.currentContext.registrationService.getAddress());
        assertEquals(sentTestCurrentContext.extensionValue, receivedTestCurrentContext.extensionValue);
        assertEquals(sentCreateCoordinationContextMessage.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
    }

    public void testRequestWithExpiresWithCurrentContextWithoutExpiresWithDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage     = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        TestCoordinationContext sentTestCurrentContext                          = new TestCoordinationContext();
        sentCreateCoordinationContextMessage.activationService                  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.expires                            = new AttributedDateTime();
        sentCreateCoordinationContextMessage.expires.dataTime                   = "TestDateTime";
        sentCreateCoordinationContextMessage.expires.valueType                  = "TestValueType";
        sentCreateCoordinationContextMessage.currentContext                     = sentTestCurrentContext;
        sentCreateCoordinationContextMessage.currentContext.identifier          = TestUtil.PROTOCOL_IDENTIFIER;
        sentCreateCoordinationContextMessage.currentContext.expires             = null;
        sentCreateCoordinationContextMessage.currentContext.coordinationType    = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.currentContext.registrationService = new PortReference(TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL);
        sentCreateCoordinationContextMessage.requesterReference                 = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType                   = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.dialogIdentifier                   = "123456";

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        TestCoordinationContext receivedTestCurrentContext = (TestCoordinationContext) receivedCreateCoordinationContextMessage.currentContext;
        assertEquals(sentCreateCoordinationContextMessage.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.expires.dataTime, receivedCreateCoordinationContextMessage.expires.dataTime);
        assertEquals(sentCreateCoordinationContextMessage.expires.valueType, receivedCreateCoordinationContextMessage.expires.valueType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.identifier, receivedCreateCoordinationContextMessage.currentContext.identifier);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.expires, receivedCreateCoordinationContextMessage.currentContext.expires);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.coordinationType, receivedCreateCoordinationContextMessage.currentContext.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.registrationService.getAddress(), receivedCreateCoordinationContextMessage.currentContext.registrationService.getAddress());
        assertEquals(sentTestCurrentContext.extensionValue, receivedTestCurrentContext.extensionValue);
        assertEquals(sentCreateCoordinationContextMessage.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
    }

    public void testRequestWithExpiresWithCurrentContextWithExpiresWithDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage     = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        TestCoordinationContext sentTestCurrentContext                          = new TestCoordinationContext();
        sentCreateCoordinationContextMessage.activationService                  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.expires                            = new AttributedDateTime();
        sentCreateCoordinationContextMessage.expires.dataTime                   = "TestDateTime";
        sentCreateCoordinationContextMessage.expires.valueType                  = "TestValueType";
        sentCreateCoordinationContextMessage.currentContext                     = sentTestCurrentContext;
        sentCreateCoordinationContextMessage.currentContext.identifier          = TestUtil.PROTOCOL_IDENTIFIER;
        sentCreateCoordinationContextMessage.currentContext.expires             = new AttributedDateTime();
        sentCreateCoordinationContextMessage.currentContext.expires.dataTime    = "CC-TestDateTime";
        sentCreateCoordinationContextMessage.currentContext.expires.valueType   = "CC-TestValueType";
        sentCreateCoordinationContextMessage.currentContext.coordinationType    = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.currentContext.registrationService = new PortReference(TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL);
        sentCreateCoordinationContextMessage.requesterReference                 = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType                   = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextMessage.dialogIdentifier                   = "123456";

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        TestCoordinationContext receivedTestCurrentContext = (TestCoordinationContext) receivedCreateCoordinationContextMessage.currentContext;
        assertEquals(sentCreateCoordinationContextMessage.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.expires.dataTime, receivedCreateCoordinationContextMessage.expires.dataTime);
        assertEquals(sentCreateCoordinationContextMessage.expires.valueType, receivedCreateCoordinationContextMessage.expires.valueType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.identifier, receivedCreateCoordinationContextMessage.currentContext.identifier);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.expires.dataTime, receivedCreateCoordinationContextMessage.currentContext.expires.dataTime);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.expires.valueType, receivedCreateCoordinationContextMessage.currentContext.expires.valueType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.coordinationType, receivedCreateCoordinationContextMessage.currentContext.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.currentContext.registrationService.getAddress(), receivedCreateCoordinationContextMessage.currentContext.registrationService.getAddress());
        assertEquals(sentTestCurrentContext.extensionValue, receivedTestCurrentContext.extensionValue);
        assertEquals(sentCreateCoordinationContextMessage.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
    }

    public void testResponseWithContextWithDialogIdentifier()
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
        sentCreateCoordinationContextResponseMessage.dialogIdentifier                        = "123456";

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

    public void testResponseNoContextWithDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextResponseMessage sentCreateCoordinationContextResponseMessage     = new CreateCoordinationContextResponseMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        sentCreateCoordinationContextResponseMessage.requesterReference  = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextResponseMessage.coordinationContext = null;
        sentCreateCoordinationContextResponseMessage.dialogIdentifier    = "123456";

        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage);

        receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage.dialogIdentifier);

        assertEquals(sentCreateCoordinationContextResponseMessage.requesterReference.getAddress(), receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress());
        assertNull(receivedCreateCoordinationContextResponseMessage.coordinationContext);
        assertEquals(sentCreateCoordinationContextResponseMessage.dialogIdentifier, receivedCreateCoordinationContextResponseMessage.dialogIdentifier);
    }

    public void testErrorWithDialogIdentifier()
        throws Exception
    {
        ErrorMessage                             sentErrorMessage                                 = new ErrorMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_activationRequesterServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJCOOR_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

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

    protected void tearDown()
        throws Exception
    {
    }

    private ActivationRequester   _activationRequester             = null;
    private String                _activationRequesterServiceURI   = null;
    private ActivationCoordinator _activationCoordinator           = null;
    private String                _activationCoordinatorServiceURI = null;
}
