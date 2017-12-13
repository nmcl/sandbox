/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * MultiSendActivationTestCase.java
 */

package com.arjuna.wsc.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.*;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.impl.messaging.XMLUtil;

public class MultiSendActivationTestCase extends TestCase
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

    public void testRequestWithoutDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage1    = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage2    = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage3    = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage4    = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        sentCreateCoordinationContextMessage1.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage1.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage1.coordinationType   = "http://foo1.example.org/bar";
        sentCreateCoordinationContextMessage1.dialogIdentifier   = null;
        sentCreateCoordinationContextMessage2.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage2.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage2.coordinationType   = "http://foo2.example.org/bar";
        sentCreateCoordinationContextMessage2.dialogIdentifier   = null;
        sentCreateCoordinationContextMessage3.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage3.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage3.coordinationType   = "http://foo3.example.org/bar";
        sentCreateCoordinationContextMessage3.dialogIdentifier   = null;
        sentCreateCoordinationContextMessage4.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage4.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage4.coordinationType   = "http://foo4.example.org/bar";
        sentCreateCoordinationContextMessage4.dialogIdentifier   = null;

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage1);
        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage2);
        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage3);
        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage4);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        assertEquals(sentCreateCoordinationContextMessage1.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage1.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage1.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertNull(receivedCreateCoordinationContextMessage.dialogIdentifier);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        assertEquals(sentCreateCoordinationContextMessage2.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage2.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage2.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertNull(receivedCreateCoordinationContextMessage.dialogIdentifier);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        assertEquals(sentCreateCoordinationContextMessage3.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage3.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage3.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertNull(receivedCreateCoordinationContextMessage.dialogIdentifier);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        assertEquals(sentCreateCoordinationContextMessage4.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage4.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage4.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertNull(receivedCreateCoordinationContextMessage.dialogIdentifier);
    }

    public void testResponseWithoutDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextResponseMessage sentCreateCoordinationContextResponseMessage1    = new CreateCoordinationContextResponseMessage();
        CreateCoordinationContextResponseMessage sentCreateCoordinationContextResponseMessage2    = new CreateCoordinationContextResponseMessage();
        CreateCoordinationContextResponseMessage sentCreateCoordinationContextResponseMessage3    = new CreateCoordinationContextResponseMessage();
        CreateCoordinationContextResponseMessage sentCreateCoordinationContextResponseMessage4    = new CreateCoordinationContextResponseMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        sentCreateCoordinationContextResponseMessage1.requesterReference                      = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextResponseMessage1.coordinationContext                     = new CoordinationContext();
        sentCreateCoordinationContextResponseMessage1.coordinationContext.identifier          = "http://foo1.example.org/bar";
        sentCreateCoordinationContextResponseMessage1.coordinationContext.coordinationType    = "http://bar1.example.org/foo";
        sentCreateCoordinationContextResponseMessage1.coordinationContext.registrationService = new PortReference("http://www1.example.org/");
        sentCreateCoordinationContextResponseMessage1.dialogIdentifier                        = null;
        sentCreateCoordinationContextResponseMessage2.requesterReference                      = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextResponseMessage2.coordinationContext                     = new CoordinationContext();
        sentCreateCoordinationContextResponseMessage2.coordinationContext.identifier          = "http://foo2.example.org/bar";
        sentCreateCoordinationContextResponseMessage2.coordinationContext.coordinationType    = "http://bar2.example.org/foo";
        sentCreateCoordinationContextResponseMessage2.coordinationContext.registrationService = new PortReference("http://www2.example.org/");
        sentCreateCoordinationContextResponseMessage2.dialogIdentifier                        = null;
        sentCreateCoordinationContextResponseMessage3.requesterReference                      = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextResponseMessage3.coordinationContext                     = new CoordinationContext();
        sentCreateCoordinationContextResponseMessage3.coordinationContext.identifier          = "http://foo3.example.org/bar";
        sentCreateCoordinationContextResponseMessage3.coordinationContext.coordinationType    = "http://bar3.example.org/foo";
        sentCreateCoordinationContextResponseMessage3.coordinationContext.registrationService = new PortReference("http://www3.example.org/");
        sentCreateCoordinationContextResponseMessage3.dialogIdentifier                        = null;
        sentCreateCoordinationContextResponseMessage4.requesterReference                      = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextResponseMessage4.coordinationContext                     = new CoordinationContext();
        sentCreateCoordinationContextResponseMessage4.coordinationContext.identifier          = "http://foo4.example.org/bar";
        sentCreateCoordinationContextResponseMessage4.coordinationContext.coordinationType    = "http://bar4.example.org/foo";
        sentCreateCoordinationContextResponseMessage4.coordinationContext.registrationService = new PortReference("http://www4.example.org/");
        sentCreateCoordinationContextResponseMessage4.dialogIdentifier                        = null;

        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage1);
        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage2);
        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage3);
        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage4);

        receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage1.dialogIdentifier);

        assertEquals(sentCreateCoordinationContextResponseMessage1.requesterReference.getAddress(), receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage1.coordinationContext.identifier, receivedCreateCoordinationContextResponseMessage.coordinationContext.identifier);
        assertEquals(sentCreateCoordinationContextResponseMessage1.coordinationContext.coordinationType, receivedCreateCoordinationContextResponseMessage.coordinationContext.coordinationType);
        assertEquals(sentCreateCoordinationContextResponseMessage1.coordinationContext.registrationService.getAddress(), receivedCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress());
        assertNull(receivedCreateCoordinationContextResponseMessage.dialogIdentifier);

        receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage2.dialogIdentifier);

        assertEquals(sentCreateCoordinationContextResponseMessage2.requesterReference.getAddress(), receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage2.coordinationContext.identifier, receivedCreateCoordinationContextResponseMessage.coordinationContext.identifier);
        assertEquals(sentCreateCoordinationContextResponseMessage2.coordinationContext.coordinationType, receivedCreateCoordinationContextResponseMessage.coordinationContext.coordinationType);
        assertEquals(sentCreateCoordinationContextResponseMessage2.coordinationContext.registrationService.getAddress(), receivedCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress());
        assertNull(receivedCreateCoordinationContextResponseMessage.dialogIdentifier);

        receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage3.dialogIdentifier);

        assertEquals(sentCreateCoordinationContextResponseMessage3.requesterReference.getAddress(), receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage3.coordinationContext.identifier, receivedCreateCoordinationContextResponseMessage.coordinationContext.identifier);
        assertEquals(sentCreateCoordinationContextResponseMessage3.coordinationContext.coordinationType, receivedCreateCoordinationContextResponseMessage.coordinationContext.coordinationType);
        assertEquals(sentCreateCoordinationContextResponseMessage3.coordinationContext.registrationService.getAddress(), receivedCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress());
        assertNull(receivedCreateCoordinationContextResponseMessage.dialogIdentifier);

        receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage4.dialogIdentifier);

        assertEquals(sentCreateCoordinationContextResponseMessage4.requesterReference.getAddress(), receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage4.coordinationContext.identifier, receivedCreateCoordinationContextResponseMessage.coordinationContext.identifier);
        assertEquals(sentCreateCoordinationContextResponseMessage4.coordinationContext.coordinationType, receivedCreateCoordinationContextResponseMessage.coordinationContext.coordinationType);
        assertEquals(sentCreateCoordinationContextResponseMessage4.coordinationContext.registrationService.getAddress(), receivedCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress());
        assertNull(receivedCreateCoordinationContextResponseMessage.dialogIdentifier);
    }

    public void testErrorWithoutDialogIdentifier()
        throws Exception
    {
        ErrorMessage                             sentErrorMessage1                                = new ErrorMessage();
        ErrorMessage                             sentErrorMessage2                                = new ErrorMessage();
        ErrorMessage                             sentErrorMessage3                                = new ErrorMessage();
        ErrorMessage                             sentErrorMessage4                                = new ErrorMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        sentErrorMessage1.targetProtocolService = new PortReference(_activationRequesterServiceURI);
        sentErrorMessage1.errorCode             = new ErrorCode();
        sentErrorMessage1.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage1.errorCode.name        = XMLUtil.INVALIDADDRESS_ERROR_CODE;
        sentErrorMessage1.dialogIdentifier      = null;
        sentErrorMessage2.targetProtocolService = new PortReference(_activationRequesterServiceURI);
        sentErrorMessage2.errorCode             = new ErrorCode();
        sentErrorMessage2.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage2.errorCode.name        = XMLUtil.INVALIDCREATEPARAMETERS_ERROR_CODE;
        sentErrorMessage2.dialogIdentifier      = null;
        sentErrorMessage3.targetProtocolService = new PortReference(_activationRequesterServiceURI);
        sentErrorMessage3.errorCode             = new ErrorCode();
        sentErrorMessage3.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage3.errorCode.name        = XMLUtil.INVALIDPROTOCOL_ERROR_CODE;
        sentErrorMessage3.dialogIdentifier      = null;
        sentErrorMessage4.targetProtocolService = new PortReference(_activationRequesterServiceURI);
        sentErrorMessage4.errorCode             = new ErrorCode();
        sentErrorMessage4.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage4.errorCode.name        = XMLUtil.INVALIDSTATE_ERROR_CODE;
        sentErrorMessage4.dialogIdentifier      = null;

        _activationCoordinator.sendError(sentErrorMessage1);
        _activationCoordinator.sendError(sentErrorMessage2);
        _activationCoordinator.sendError(sentErrorMessage3);
        _activationCoordinator.sendError(sentErrorMessage4);

        try
        {
            receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentErrorMessage1.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage receivedErrorMessage = errorMessageException.getErrorMessage();

            assertEquals(sentErrorMessage1.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
            assertEquals(sentErrorMessage1.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
            assertEquals(sentErrorMessage1.errorCode.name, receivedErrorMessage.errorCode.name);
            assertNull(receivedErrorMessage.dialogIdentifier);
        }

        try
        {
            receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentErrorMessage2.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage receivedErrorMessage = errorMessageException.getErrorMessage();

            assertEquals(sentErrorMessage2.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
            assertEquals(sentErrorMessage2.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
            assertEquals(sentErrorMessage2.errorCode.name, receivedErrorMessage.errorCode.name);
            assertNull(receivedErrorMessage.dialogIdentifier);
        }

        try
        {
            receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentErrorMessage3.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage receivedErrorMessage = errorMessageException.getErrorMessage();

            assertEquals(sentErrorMessage3.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
            assertEquals(sentErrorMessage3.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
            assertEquals(sentErrorMessage3.errorCode.name, receivedErrorMessage.errorCode.name);
            assertNull(receivedErrorMessage.dialogIdentifier);
        }

        try
        {
            receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentErrorMessage4.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage receivedErrorMessage = errorMessageException.getErrorMessage();

            assertEquals(sentErrorMessage4.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
            assertEquals(sentErrorMessage4.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
            assertEquals(sentErrorMessage4.errorCode.name, receivedErrorMessage.errorCode.name);
            assertNull(receivedErrorMessage.dialogIdentifier);
        }
    }

    public void testRequestWithDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage1    = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage2    = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage3    = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage sentCreateCoordinationContextMessage4    = new CreateCoordinationContextMessage();
        CreateCoordinationContextMessage receivedCreateCoordinationContextMessage = null;

        sentCreateCoordinationContextMessage1.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage1.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage1.coordinationType   = "http://foo1.example.org/bar";
        sentCreateCoordinationContextMessage1.dialogIdentifier   = "111111";
        sentCreateCoordinationContextMessage2.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage2.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage2.coordinationType   = "http://foo2.example.org/bar";
        sentCreateCoordinationContextMessage2.dialogIdentifier   = "222222";
        sentCreateCoordinationContextMessage3.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage3.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage3.coordinationType   = "http://foo3.example.org/bar";
        sentCreateCoordinationContextMessage3.dialogIdentifier   = "333333";
        sentCreateCoordinationContextMessage4.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage4.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage4.coordinationType   = "http://foo4.example.org/bar";
        sentCreateCoordinationContextMessage4.dialogIdentifier   = "444444";

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage1);
        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage2);
        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage3);
        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage4);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        assertEquals(sentCreateCoordinationContextMessage1.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage1.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage1.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage1.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        assertEquals(sentCreateCoordinationContextMessage2.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage2.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage2.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage2.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        assertEquals(sentCreateCoordinationContextMessage3.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage3.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage3.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage3.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);

        receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

        assertEquals(sentCreateCoordinationContextMessage4.requesterReference.getAddress(), receivedCreateCoordinationContextMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextMessage4.activationService.getAddress(), receivedCreateCoordinationContextMessage.activationService.getAddress());
        assertEquals(sentCreateCoordinationContextMessage4.coordinationType, receivedCreateCoordinationContextMessage.coordinationType);
        assertEquals(sentCreateCoordinationContextMessage4.dialogIdentifier, receivedCreateCoordinationContextMessage.dialogIdentifier);
    }

    public void testResponseWithDialogIdentifier()
        throws Exception
    {
        CreateCoordinationContextResponseMessage sentCreateCoordinationContextResponseMessage1    = new CreateCoordinationContextResponseMessage();
        CreateCoordinationContextResponseMessage sentCreateCoordinationContextResponseMessage2    = new CreateCoordinationContextResponseMessage();
        CreateCoordinationContextResponseMessage sentCreateCoordinationContextResponseMessage3    = new CreateCoordinationContextResponseMessage();
        CreateCoordinationContextResponseMessage sentCreateCoordinationContextResponseMessage4    = new CreateCoordinationContextResponseMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        sentCreateCoordinationContextResponseMessage1.requesterReference                      = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextResponseMessage1.coordinationContext                     = new CoordinationContext();
        sentCreateCoordinationContextResponseMessage1.coordinationContext.identifier          = "http://foo1.example.org/bar";
        sentCreateCoordinationContextResponseMessage1.coordinationContext.coordinationType    = "http://bar1.example.org/foo";
        sentCreateCoordinationContextResponseMessage1.coordinationContext.registrationService = new PortReference("http://www1.example.org/");
        sentCreateCoordinationContextResponseMessage1.dialogIdentifier                        = "111111";
        sentCreateCoordinationContextResponseMessage2.requesterReference                      = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextResponseMessage2.coordinationContext                     = new CoordinationContext();
        sentCreateCoordinationContextResponseMessage2.coordinationContext.identifier          = "http://foo2.example.org/bar";
        sentCreateCoordinationContextResponseMessage2.coordinationContext.coordinationType    = "http://bar2.example.org/foo";
        sentCreateCoordinationContextResponseMessage2.coordinationContext.registrationService = new PortReference("http://www2.example.org/");
        sentCreateCoordinationContextResponseMessage2.dialogIdentifier                        = "222222";
        sentCreateCoordinationContextResponseMessage3.requesterReference                      = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextResponseMessage3.coordinationContext                     = new CoordinationContext();
        sentCreateCoordinationContextResponseMessage3.coordinationContext.identifier          = "http://foo3.example.org/bar";
        sentCreateCoordinationContextResponseMessage3.coordinationContext.coordinationType    = "http://bar3.example.org/foo";
        sentCreateCoordinationContextResponseMessage3.coordinationContext.registrationService = new PortReference("http://www3.example.org/");
        sentCreateCoordinationContextResponseMessage3.dialogIdentifier                        = "333333";
        sentCreateCoordinationContextResponseMessage4.requesterReference                      = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextResponseMessage4.coordinationContext                     = new CoordinationContext();
        sentCreateCoordinationContextResponseMessage4.coordinationContext.identifier          = "http://foo4.example.org/bar";
        sentCreateCoordinationContextResponseMessage4.coordinationContext.coordinationType    = "http://bar4.example.org/foo";
        sentCreateCoordinationContextResponseMessage4.coordinationContext.registrationService = new PortReference("http://www4.example.org/");
        sentCreateCoordinationContextResponseMessage4.dialogIdentifier                        = "444444";

        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage1);
        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage2);
        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage3);
        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage4);

        receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage4.dialogIdentifier);

        assertEquals(sentCreateCoordinationContextResponseMessage4.requesterReference.getAddress(), receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage4.coordinationContext.identifier, receivedCreateCoordinationContextResponseMessage.coordinationContext.identifier);
        assertEquals(sentCreateCoordinationContextResponseMessage4.coordinationContext.coordinationType, receivedCreateCoordinationContextResponseMessage.coordinationContext.coordinationType);
        assertEquals(sentCreateCoordinationContextResponseMessage4.coordinationContext.registrationService.getAddress(), receivedCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage4.dialogIdentifier, receivedCreateCoordinationContextResponseMessage.dialogIdentifier);

        receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage3.dialogIdentifier);

        assertEquals(sentCreateCoordinationContextResponseMessage3.requesterReference.getAddress(), receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage3.coordinationContext.identifier, receivedCreateCoordinationContextResponseMessage.coordinationContext.identifier);
        assertEquals(sentCreateCoordinationContextResponseMessage3.coordinationContext.coordinationType, receivedCreateCoordinationContextResponseMessage.coordinationContext.coordinationType);
        assertEquals(sentCreateCoordinationContextResponseMessage3.coordinationContext.registrationService.getAddress(), receivedCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage3.dialogIdentifier, receivedCreateCoordinationContextResponseMessage.dialogIdentifier);

        receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage2.dialogIdentifier);

        assertEquals(sentCreateCoordinationContextResponseMessage2.requesterReference.getAddress(), receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage2.coordinationContext.identifier, receivedCreateCoordinationContextResponseMessage.coordinationContext.identifier);
        assertEquals(sentCreateCoordinationContextResponseMessage2.coordinationContext.coordinationType, receivedCreateCoordinationContextResponseMessage.coordinationContext.coordinationType);
        assertEquals(sentCreateCoordinationContextResponseMessage2.coordinationContext.registrationService.getAddress(), receivedCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage2.dialogIdentifier, receivedCreateCoordinationContextResponseMessage.dialogIdentifier);

        receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage1.dialogIdentifier);

        assertEquals(sentCreateCoordinationContextResponseMessage1.requesterReference.getAddress(), receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage1.coordinationContext.identifier, receivedCreateCoordinationContextResponseMessage.coordinationContext.identifier);
        assertEquals(sentCreateCoordinationContextResponseMessage1.coordinationContext.coordinationType, receivedCreateCoordinationContextResponseMessage.coordinationContext.coordinationType);
        assertEquals(sentCreateCoordinationContextResponseMessage1.coordinationContext.registrationService.getAddress(), receivedCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage1.dialogIdentifier, receivedCreateCoordinationContextResponseMessage.dialogIdentifier);
    }

    public void testErrorWithDialogIdentifier()
        throws Exception
    {
        ErrorMessage                             sentErrorMessage1                                = new ErrorMessage();
        ErrorMessage                             sentErrorMessage2                                = new ErrorMessage();
        ErrorMessage                             sentErrorMessage3                                = new ErrorMessage();
        ErrorMessage                             sentErrorMessage4                                = new ErrorMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        sentErrorMessage1.targetProtocolService = new PortReference(_activationRequesterServiceURI);
        sentErrorMessage1.errorCode             = new ErrorCode();
        sentErrorMessage1.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage1.errorCode.name        = XMLUtil.INVALIDADDRESS_ERROR_CODE;
        sentErrorMessage1.dialogIdentifier      = "111111";
        sentErrorMessage2.targetProtocolService = new PortReference(_activationRequesterServiceURI);
        sentErrorMessage2.errorCode             = new ErrorCode();
        sentErrorMessage2.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage2.errorCode.name        = XMLUtil.INVALIDCREATEPARAMETERS_ERROR_CODE;
        sentErrorMessage2.dialogIdentifier      = "222222";
        sentErrorMessage3.targetProtocolService = new PortReference(_activationRequesterServiceURI);
        sentErrorMessage3.errorCode             = new ErrorCode();
        sentErrorMessage3.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage3.errorCode.name        = XMLUtil.INVALIDPROTOCOL_ERROR_CODE;
        sentErrorMessage3.dialogIdentifier      = "333333";
        sentErrorMessage4.targetProtocolService = new PortReference(_activationRequesterServiceURI);
        sentErrorMessage4.errorCode             = new ErrorCode();
        sentErrorMessage4.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage4.errorCode.name        = XMLUtil.INVALIDSTATE_ERROR_CODE;
        sentErrorMessage4.dialogIdentifier      = "444444";

        _activationCoordinator.sendError(sentErrorMessage1);
        _activationCoordinator.sendError(sentErrorMessage2);
        _activationCoordinator.sendError(sentErrorMessage3);
        _activationCoordinator.sendError(sentErrorMessage4);

        try
        {
            receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentErrorMessage4.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage receivedErrorMessage = errorMessageException.getErrorMessage();

            assertEquals(sentErrorMessage4.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
            assertEquals(sentErrorMessage4.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
            assertEquals(sentErrorMessage4.errorCode.name, receivedErrorMessage.errorCode.name);
            assertEquals(sentErrorMessage4.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
        }

        try
        {
            receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentErrorMessage3.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage receivedErrorMessage = errorMessageException.getErrorMessage();

            assertEquals(sentErrorMessage3.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
            assertEquals(sentErrorMessage3.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
            assertEquals(sentErrorMessage3.errorCode.name, receivedErrorMessage.errorCode.name);
            assertEquals(sentErrorMessage3.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
        }

        try
        {
            receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentErrorMessage2.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage receivedErrorMessage = errorMessageException.getErrorMessage();

            assertEquals(sentErrorMessage2.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
            assertEquals(sentErrorMessage2.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
            assertEquals(sentErrorMessage2.errorCode.name, receivedErrorMessage.errorCode.name);
            assertEquals(sentErrorMessage2.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
        }

        try
        {
            receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentErrorMessage1.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage receivedErrorMessage = errorMessageException.getErrorMessage();

            assertEquals(sentErrorMessage1.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
            assertEquals(sentErrorMessage1.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
            assertEquals(sentErrorMessage1.errorCode.name, receivedErrorMessage.errorCode.name);
            assertEquals(sentErrorMessage1.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
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
