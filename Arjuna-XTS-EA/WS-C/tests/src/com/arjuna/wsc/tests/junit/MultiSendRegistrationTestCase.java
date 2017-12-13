/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * MultiSendRegistrationTestCase.java
 */

package com.arjuna.wsc.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.*;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.impl.messaging.XMLUtil;

public class MultiSendRegistrationTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String registrationRequesterJNDIName   = "registrationrequester";
        String registrationCoordinatorJNDIName = "registrationcoordinator-msg";

        _registrationRequester             = (RegistrationRequester) initialContext.lookup(registrationRequesterJNDIName);
        _registrationRequesterServiceURI   = _registrationRequester.getServiceURI();
        _registrationCoordinator           = (RegistrationCoordinator) initialContext.lookup(registrationCoordinatorJNDIName);
        _registrationCoordinatorServiceURI = _registrationCoordinator.getServiceURI();
    }

    public void testRequestWithoutDialogIdentifierWithoutInstanceIdentifier()
        throws Exception
    {
        RegisterMessage sentRegisterMessage1    = new RegisterMessage();
        RegisterMessage sentRegisterMessage2    = new RegisterMessage();
        RegisterMessage sentRegisterMessage3    = new RegisterMessage();
        RegisterMessage sentRegisterMessage4    = new RegisterMessage();
        RegisterMessage receivedRegisterMessage = null;

        sentRegisterMessage1.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage1.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage1.protocolIdentifier         = "http://foo1.example.org/bar";
        sentRegisterMessage1.participantProtocolService = new PortReference("http://bar1.example.org/foo");
        sentRegisterMessage1.dialogIdentifier           = null;
        sentRegisterMessage2.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage2.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage2.protocolIdentifier         = "http://foo2.example.org/bar";
        sentRegisterMessage2.participantProtocolService = new PortReference("http://bar2.example.org/foo");
        sentRegisterMessage2.dialogIdentifier           = null;
        sentRegisterMessage3.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage3.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage3.protocolIdentifier         = "http://foo3.example.org/bar";
        sentRegisterMessage3.participantProtocolService = new PortReference("http://bar3.example.org/foo");
        sentRegisterMessage3.dialogIdentifier           = null;
        sentRegisterMessage4.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage4.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage4.protocolIdentifier         = "http://foo4.example.org/bar";
        sentRegisterMessage4.participantProtocolService = new PortReference("http://bar4.example.org/foo");
        sentRegisterMessage4.dialogIdentifier           = null;

        _registrationRequester.sendRegister(sentRegisterMessage1);
        _registrationRequester.sendRegister(sentRegisterMessage2);
        _registrationRequester.sendRegister(sentRegisterMessage3);
        _registrationRequester.sendRegister(sentRegisterMessage4);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage1.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage1.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage1.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage1.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage1.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage1.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage2.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage2.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage2.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage2.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage2.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage2.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage3.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage3.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage3.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage3.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage3.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage3.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage4.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage4.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage4.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage4.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage4.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage4.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);
    }

    public void testRequestWithoutDialogIdentifierWithInstanceIdentifier()
        throws Exception
    {
        RegisterMessage sentRegisterMessage1    = new RegisterMessage();
        RegisterMessage sentRegisterMessage2    = new RegisterMessage();
        RegisterMessage sentRegisterMessage3    = new RegisterMessage();
        RegisterMessage sentRegisterMessage4    = new RegisterMessage();
        RegisterMessage receivedRegisterMessage = null;

        sentRegisterMessage1.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage1.registrationService.addExtension(XMLUtil.INSTANCE_IDENTIFIER_TAG, "ABCDEF");
        sentRegisterMessage1.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage1.protocolIdentifier         = "http://foo1.example.org/bar";
        sentRegisterMessage1.participantProtocolService = new PortReference("http://bar1.example.org/foo");
        sentRegisterMessage1.dialogIdentifier           = null;
        sentRegisterMessage2.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage2.registrationService.addExtension(XMLUtil.INSTANCE_IDENTIFIER_TAG, "BCDEFA");
        sentRegisterMessage2.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage2.protocolIdentifier         = "http://foo2.example.org/bar";
        sentRegisterMessage2.participantProtocolService = new PortReference("http://bar2.example.org/foo");
        sentRegisterMessage2.dialogIdentifier           = null;
        sentRegisterMessage3.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage3.registrationService.addExtension(XMLUtil.INSTANCE_IDENTIFIER_TAG, "CDEFAB");
        sentRegisterMessage3.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage3.protocolIdentifier         = "http://foo3.example.org/bar";
        sentRegisterMessage3.participantProtocolService = new PortReference("http://bar3.example.org/foo");
        sentRegisterMessage3.dialogIdentifier           = null;
        sentRegisterMessage4.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage4.registrationService.addExtension(XMLUtil.INSTANCE_IDENTIFIER_TAG, "DEFABC");
        sentRegisterMessage4.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage4.protocolIdentifier         = "http://foo4.example.org/bar";
        sentRegisterMessage4.participantProtocolService = new PortReference("http://bar4.example.org/foo");
        sentRegisterMessage4.dialogIdentifier           = null;

        _registrationRequester.sendRegister(sentRegisterMessage1);
        _registrationRequester.sendRegister(sentRegisterMessage2);
        _registrationRequester.sendRegister(sentRegisterMessage3);
        _registrationRequester.sendRegister(sentRegisterMessage4);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage1.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage1.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage1.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage1.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage1.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage1.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage2.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage2.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage2.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage2.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage2.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage2.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage3.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage3.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage3.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage3.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage3.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage3.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage4.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage4.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage4.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage4.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage4.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage4.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);
    }

    public void testResponseWithoutDialogIdentifier()
        throws Exception
    {
        RegisterResponseMessage sentRegisterResponseMessage1    = new RegisterResponseMessage();
        RegisterResponseMessage sentRegisterResponseMessage2    = new RegisterResponseMessage();
        RegisterResponseMessage sentRegisterResponseMessage3    = new RegisterResponseMessage();
        RegisterResponseMessage sentRegisterResponseMessage4    = new RegisterResponseMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterResponseMessage1.requesterReference          = new PortReference(_registrationRequesterServiceURI);
        sentRegisterResponseMessage1.coordinationProtocolService = new PortReference("http://foo1.example.org/bar");
        sentRegisterResponseMessage1.dialogIdentifier            = null;
        sentRegisterResponseMessage2.requesterReference          = new PortReference(_registrationRequesterServiceURI);
        sentRegisterResponseMessage2.coordinationProtocolService = new PortReference("http://foo2.example.org/bar");
        sentRegisterResponseMessage2.dialogIdentifier            = null;
        sentRegisterResponseMessage3.requesterReference          = new PortReference(_registrationRequesterServiceURI);
        sentRegisterResponseMessage3.coordinationProtocolService = new PortReference("http://foo3.example.org/bar");
        sentRegisterResponseMessage3.dialogIdentifier            = null;
        sentRegisterResponseMessage4.requesterReference          = new PortReference(_registrationRequesterServiceURI);
        sentRegisterResponseMessage4.coordinationProtocolService = new PortReference("http://foo4.example.org/bar");
        sentRegisterResponseMessage4.dialogIdentifier            = null;

        _registrationCoordinator.sendRegisterResponse(sentRegisterResponseMessage1);
        _registrationCoordinator.sendRegisterResponse(sentRegisterResponseMessage2);
        _registrationCoordinator.sendRegisterResponse(sentRegisterResponseMessage3);
        _registrationCoordinator.sendRegisterResponse(sentRegisterResponseMessage4);

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterResponseMessage1.dialogIdentifier);

        assertEquals(sentRegisterResponseMessage1.requesterReference.getAddress(), receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(sentRegisterResponseMessage1.coordinationProtocolService.getAddress(), receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertNull(receivedRegisterResponseMessage.dialogIdentifier);

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterResponseMessage2.dialogIdentifier);

        assertEquals(sentRegisterResponseMessage2.requesterReference.getAddress(), receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(sentRegisterResponseMessage2.coordinationProtocolService.getAddress(), receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertNull(receivedRegisterResponseMessage.dialogIdentifier);

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterResponseMessage3.dialogIdentifier);

        assertEquals(sentRegisterResponseMessage3.requesterReference.getAddress(), receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(sentRegisterResponseMessage3.coordinationProtocolService.getAddress(), receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertNull(receivedRegisterResponseMessage.dialogIdentifier);

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterResponseMessage4.dialogIdentifier);

        assertEquals(sentRegisterResponseMessage4.requesterReference.getAddress(), receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(sentRegisterResponseMessage4.coordinationProtocolService.getAddress(), receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertNull(receivedRegisterResponseMessage.dialogIdentifier);
    }

    public void testErrorWithoutDialogIdentifier()
        throws Exception
    {
        ErrorMessage            sentErrorMessage1               = new ErrorMessage();
        ErrorMessage            sentErrorMessage2               = new ErrorMessage();
        ErrorMessage            sentErrorMessage3               = new ErrorMessage();
        ErrorMessage            sentErrorMessage4               = new ErrorMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentErrorMessage1.targetProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentErrorMessage1.errorCode             = new ErrorCode();
        sentErrorMessage1.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage1.errorCode.name        = XMLUtil.INVALIDADDRESS_ERROR_CODE;
        sentErrorMessage1.dialogIdentifier      = null;
        sentErrorMessage2.targetProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentErrorMessage2.errorCode             = new ErrorCode();
        sentErrorMessage2.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage2.errorCode.name        = XMLUtil.INVALIDCREATEPARAMETERS_ERROR_CODE;
        sentErrorMessage2.dialogIdentifier      = null;
        sentErrorMessage3.targetProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentErrorMessage3.errorCode             = new ErrorCode();
        sentErrorMessage3.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage3.errorCode.name        = XMLUtil.INVALIDPROTOCOL_ERROR_CODE;
        sentErrorMessage3.dialogIdentifier      = null;
        sentErrorMessage4.targetProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentErrorMessage4.errorCode             = new ErrorCode();
        sentErrorMessage4.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage4.errorCode.name        = XMLUtil.INVALIDSTATE_ERROR_CODE;
        sentErrorMessage4.dialogIdentifier      = null;

        _registrationCoordinator.sendError(sentErrorMessage1);
        _registrationCoordinator.sendError(sentErrorMessage2);
        _registrationCoordinator.sendError(sentErrorMessage3);
        _registrationCoordinator.sendError(sentErrorMessage4);

        try
        {
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentErrorMessage1.dialogIdentifier);

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
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentErrorMessage2.dialogIdentifier);

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
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentErrorMessage3.dialogIdentifier);

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
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentErrorMessage4.dialogIdentifier);

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

    public void testRequestWithDialogIdentifierWithoutInstanceIdentifier()
        throws Exception
    {
        RegisterMessage sentRegisterMessage1    = new RegisterMessage();
        RegisterMessage sentRegisterMessage2    = new RegisterMessage();
        RegisterMessage sentRegisterMessage3    = new RegisterMessage();
        RegisterMessage sentRegisterMessage4    = new RegisterMessage();
        RegisterMessage receivedRegisterMessage = null;

        sentRegisterMessage1.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage1.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage1.protocolIdentifier         = "http://foo1.example.org/bar";
        sentRegisterMessage1.participantProtocolService = new PortReference("http://bar1.example.org/foo");
        sentRegisterMessage1.dialogIdentifier           = "111111";
        sentRegisterMessage2.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage2.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage2.protocolIdentifier         = "http://foo2.example.org/bar";
        sentRegisterMessage2.participantProtocolService = new PortReference("http://bar2.example.org/foo");
        sentRegisterMessage2.dialogIdentifier           = "222222";
        sentRegisterMessage3.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage3.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage3.protocolIdentifier         = "http://foo3.example.org/bar";
        sentRegisterMessage3.participantProtocolService = new PortReference("http://bar3.example.org/foo");
        sentRegisterMessage3.dialogIdentifier           = "333333";
        sentRegisterMessage4.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage4.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage4.protocolIdentifier         = "http://foo4.example.org/bar";
        sentRegisterMessage4.participantProtocolService = new PortReference("http://bar4.example.org/foo");
        sentRegisterMessage4.dialogIdentifier           = "444444";

        _registrationRequester.sendRegister(sentRegisterMessage1);
        _registrationRequester.sendRegister(sentRegisterMessage2);
        _registrationRequester.sendRegister(sentRegisterMessage3);
        _registrationRequester.sendRegister(sentRegisterMessage4);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage1.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage1.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage1.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage1.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage1.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage1.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage2.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage2.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage2.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage2.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage2.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage2.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage3.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage3.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage3.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage3.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage3.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage3.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage4.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage4.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage4.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage4.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage4.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage4.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);
    }

    public void testRequestWithDialogIdentifierWithInstanceIdentifier()
        throws Exception
    {
        RegisterMessage sentRegisterMessage1    = new RegisterMessage();
        RegisterMessage sentRegisterMessage2    = new RegisterMessage();
        RegisterMessage sentRegisterMessage3    = new RegisterMessage();
        RegisterMessage sentRegisterMessage4    = new RegisterMessage();
        RegisterMessage receivedRegisterMessage = null;

        sentRegisterMessage1.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage1.registrationService.addExtension(XMLUtil.INSTANCE_IDENTIFIER_TAG, "ABCDEF");
        sentRegisterMessage1.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage1.protocolIdentifier         = "http://foo1.example.org/bar";
        sentRegisterMessage1.participantProtocolService = new PortReference("http://bar1.example.org/foo");
        sentRegisterMessage1.dialogIdentifier           = "111111";
        sentRegisterMessage2.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage2.registrationService.addExtension(XMLUtil.INSTANCE_IDENTIFIER_TAG, "BCDEFA");
        sentRegisterMessage2.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage2.protocolIdentifier         = "http://foo2.example.org/bar";
        sentRegisterMessage2.participantProtocolService = new PortReference("http://bar2.example.org/foo");
        sentRegisterMessage2.dialogIdentifier           = "222222";
        sentRegisterMessage3.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage3.registrationService.addExtension(XMLUtil.INSTANCE_IDENTIFIER_TAG, "CDEFAB");
        sentRegisterMessage3.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage3.protocolIdentifier         = "http://foo3.example.org/bar";
        sentRegisterMessage3.participantProtocolService = new PortReference("http://bar3.example.org/foo");
        sentRegisterMessage3.dialogIdentifier           = "333333";
        sentRegisterMessage4.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage4.registrationService.addExtension(XMLUtil.INSTANCE_IDENTIFIER_TAG, "DEFABC");
        sentRegisterMessage4.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage4.protocolIdentifier         = "http://foo4.example.org/bar";
        sentRegisterMessage4.participantProtocolService = new PortReference("http://bar4.example.org/foo");
        sentRegisterMessage4.dialogIdentifier           = "444444";

        _registrationRequester.sendRegister(sentRegisterMessage1);
        _registrationRequester.sendRegister(sentRegisterMessage2);
        _registrationRequester.sendRegister(sentRegisterMessage3);
        _registrationRequester.sendRegister(sentRegisterMessage4);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage1.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage1.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage1.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage1.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage1.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage1.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage2.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage2.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage2.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage2.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage2.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage2.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage3.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage3.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage3.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage3.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage3.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage3.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage4.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage4.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.requesterReference.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage4.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage4.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage4.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage4.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);
    }

    public void testResponseWithDialogIdentifier()
        throws Exception
    {
        RegisterResponseMessage sentRegisterResponseMessage1    = new RegisterResponseMessage();
        RegisterResponseMessage sentRegisterResponseMessage2    = new RegisterResponseMessage();
        RegisterResponseMessage sentRegisterResponseMessage3    = new RegisterResponseMessage();
        RegisterResponseMessage sentRegisterResponseMessage4    = new RegisterResponseMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterResponseMessage1.requesterReference          = new PortReference(_registrationRequesterServiceURI);
        sentRegisterResponseMessage1.coordinationProtocolService = new PortReference("http://foo1.example.org/bar");
        sentRegisterResponseMessage1.dialogIdentifier            = "111111";
        sentRegisterResponseMessage2.requesterReference          = new PortReference(_registrationRequesterServiceURI);
        sentRegisterResponseMessage2.coordinationProtocolService = new PortReference("http://foo2.example.org/bar");
        sentRegisterResponseMessage2.dialogIdentifier            = "222222";
        sentRegisterResponseMessage3.requesterReference          = new PortReference(_registrationRequesterServiceURI);
        sentRegisterResponseMessage3.coordinationProtocolService = new PortReference("http://foo3.example.org/bar");
        sentRegisterResponseMessage3.dialogIdentifier            = "333333";
        sentRegisterResponseMessage4.requesterReference          = new PortReference(_registrationRequesterServiceURI);
        sentRegisterResponseMessage4.coordinationProtocolService = new PortReference("http://foo4.example.org/bar");
        sentRegisterResponseMessage4.dialogIdentifier            = "444444";

        _registrationCoordinator.sendRegisterResponse(sentRegisterResponseMessage1);
        _registrationCoordinator.sendRegisterResponse(sentRegisterResponseMessage2);
        _registrationCoordinator.sendRegisterResponse(sentRegisterResponseMessage3);
        _registrationCoordinator.sendRegisterResponse(sentRegisterResponseMessage4);

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterResponseMessage4.dialogIdentifier);

        assertEquals(sentRegisterResponseMessage4.requesterReference.getAddress(), receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(sentRegisterResponseMessage4.coordinationProtocolService.getAddress(), receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertEquals(sentRegisterResponseMessage4.dialogIdentifier, receivedRegisterResponseMessage.dialogIdentifier);

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterResponseMessage3.dialogIdentifier);

        assertEquals(sentRegisterResponseMessage3.requesterReference.getAddress(), receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(sentRegisterResponseMessage3.coordinationProtocolService.getAddress(), receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertEquals(sentRegisterResponseMessage3.dialogIdentifier, receivedRegisterResponseMessage.dialogIdentifier);

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterResponseMessage2.dialogIdentifier);

        assertEquals(sentRegisterResponseMessage2.requesterReference.getAddress(), receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(sentRegisterResponseMessage2.coordinationProtocolService.getAddress(), receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertEquals(sentRegisterResponseMessage2.dialogIdentifier, receivedRegisterResponseMessage.dialogIdentifier);

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterResponseMessage1.dialogIdentifier);

        assertEquals(sentRegisterResponseMessage1.requesterReference.getAddress(), receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(sentRegisterResponseMessage1.coordinationProtocolService.getAddress(), receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertEquals(sentRegisterResponseMessage1.dialogIdentifier, receivedRegisterResponseMessage.dialogIdentifier);
    }

    public void testErrorWithDialogIdentifier()
        throws Exception
    {
        ErrorMessage            sentErrorMessage1               = new ErrorMessage();
        ErrorMessage            sentErrorMessage2               = new ErrorMessage();
        ErrorMessage            sentErrorMessage3               = new ErrorMessage();
        ErrorMessage            sentErrorMessage4               = new ErrorMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentErrorMessage1.targetProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentErrorMessage1.errorCode             = new ErrorCode();
        sentErrorMessage1.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage1.errorCode.name        = XMLUtil.INVALIDADDRESS_ERROR_CODE;
        sentErrorMessage1.dialogIdentifier      = "111111";
        sentErrorMessage2.targetProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentErrorMessage2.errorCode             = new ErrorCode();
        sentErrorMessage2.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage2.errorCode.name        = XMLUtil.INVALIDCREATEPARAMETERS_ERROR_CODE;
        sentErrorMessage2.dialogIdentifier      = "222222";
        sentErrorMessage3.targetProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentErrorMessage3.errorCode             = new ErrorCode();
        sentErrorMessage3.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage3.errorCode.name        = XMLUtil.INVALIDPROTOCOL_ERROR_CODE;
        sentErrorMessage3.dialogIdentifier      = "333333";
        sentErrorMessage4.targetProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentErrorMessage4.errorCode             = new ErrorCode();
        sentErrorMessage4.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
        sentErrorMessage4.errorCode.name        = XMLUtil.INVALIDSTATE_ERROR_CODE;
        sentErrorMessage4.dialogIdentifier      = "444444";

        _registrationCoordinator.sendError(sentErrorMessage1);
        _registrationCoordinator.sendError(sentErrorMessage2);
        _registrationCoordinator.sendError(sentErrorMessage3);
        _registrationCoordinator.sendError(sentErrorMessage4);

        try
        {
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentErrorMessage4.dialogIdentifier);

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
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentErrorMessage3.dialogIdentifier);

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
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentErrorMessage2.dialogIdentifier);

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
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentErrorMessage1.dialogIdentifier);

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

    private RegistrationRequester   _registrationRequester             = null;
    private String                  _registrationRequesterServiceURI   = null;
    private RegistrationCoordinator _registrationCoordinator           = null;
    private String                  _registrationCoordinatorServiceURI = null;
}
