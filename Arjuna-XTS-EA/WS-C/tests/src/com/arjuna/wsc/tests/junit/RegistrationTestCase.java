/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrationTestCase.java
 */

package com.arjuna.wsc.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.*;
import com.arjuna.wsc.impl.messaging.XMLUtil;
import com.arjuna.wsc.messaging.*;

public class RegistrationTestCase extends TestCase
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
        RegisterMessage sentRegisterMessage     = new RegisterMessage();
        RegisterMessage receivedRegisterMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = "http://foo.example.org/bar";
        sentRegisterMessage.participantProtocolService = new PortReference("http://bar.example.org/foo");
        sentRegisterMessage.dialogIdentifier           = null;

        _registrationRequester.sendRegister(sentRegisterMessage);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage.registrationService.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.registrationService.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);
    }

    public void testRequestWithoutDialogIdentifierWithInstanceIdentifier()
        throws Exception
    {
        RegisterMessage sentRegisterMessage     = new RegisterMessage();
        RegisterMessage receivedRegisterMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage.registrationService.addExtension(XMLUtil.INSTANCE_IDENTIFIER_TAG, "ABCDEF");
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = "http://foo.example.org/bar";
        sentRegisterMessage.participantProtocolService = new PortReference("http://bar.example.org/foo");
        sentRegisterMessage.dialogIdentifier           = null;

        _registrationRequester.sendRegister(sentRegisterMessage);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage.registrationService.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.registrationService.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);
    }

    public void testResponseWithoutDialogIdentifier()
        throws Exception
    {
        RegisterResponseMessage sentRegisterResponseMessage     = new RegisterResponseMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterResponseMessage.requesterReference          = new PortReference(_registrationRequesterServiceURI);
        sentRegisterResponseMessage.coordinationProtocolService = new PortReference("http://foo.example.org/bar");
        sentRegisterResponseMessage.dialogIdentifier            = null;

        _registrationCoordinator.sendRegisterResponse(sentRegisterResponseMessage);

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterResponseMessage.dialogIdentifier);

        assertEquals(sentRegisterResponseMessage.requesterReference.getAddress(), receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(sentRegisterResponseMessage.coordinationProtocolService.getAddress(), receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertEquals(sentRegisterResponseMessage.dialogIdentifier, receivedRegisterResponseMessage.dialogIdentifier);
    }

    public void testErrorWithoutDialogIdentifier()
        throws Exception
    {
        ErrorMessage            sentErrorMessage                = new ErrorMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJCOOR_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = null;

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

    public void testRequestWithDialogIdentifierWithoutInstanceIdentifier()
        throws Exception
    {
        RegisterMessage sentRegisterMessage     = new RegisterMessage();
        RegisterMessage receivedRegisterMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = "http://foo.example.org/bar";
        sentRegisterMessage.participantProtocolService = new PortReference("http://bar.example.org/foo");
        sentRegisterMessage.dialogIdentifier           = "123456";

        _registrationRequester.sendRegister(sentRegisterMessage);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage.registrationService.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.registrationService.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);
    }

    public void testRequestWithDialogIdentifierWithInstanceIdentifier()
        throws Exception
    {
        RegisterMessage sentRegisterMessage     = new RegisterMessage();
        RegisterMessage receivedRegisterMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage.registrationService.addExtension(XMLUtil.INSTANCE_IDENTIFIER_TAG, "ABCDEF");
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = "http://foo.example.org/bar";
        sentRegisterMessage.participantProtocolService = new PortReference("http://bar.example.org/foo");
        sentRegisterMessage.dialogIdentifier           = "123456";

        _registrationRequester.sendRegister(sentRegisterMessage);

        receivedRegisterMessage = _registrationCoordinator.recvRegister();

        assertEquals(sentRegisterMessage.registrationService.getAddress(), receivedRegisterMessage.registrationService.getAddress());
        assertEquals(sentRegisterMessage.registrationService.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG), receivedRegisterMessage.registrationService.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
        assertEquals(sentRegisterMessage.requesterReference.getAddress(), receivedRegisterMessage.requesterReference.getAddress());
        assertEquals(sentRegisterMessage.protocolIdentifier, receivedRegisterMessage.protocolIdentifier);
        assertEquals(sentRegisterMessage.participantProtocolService.getAddress(), receivedRegisterMessage.participantProtocolService.getAddress());
        assertEquals(sentRegisterMessage.dialogIdentifier, receivedRegisterMessage.dialogIdentifier);
    }

    public void testResponseWithDialogIdentifier()
        throws Exception
    {
        RegisterResponseMessage sentRegisterResponseMessage     = new RegisterResponseMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterResponseMessage.requesterReference          = new PortReference(_registrationRequesterServiceURI);
        sentRegisterResponseMessage.coordinationProtocolService = new PortReference("http://foo.example.org/bar");
        sentRegisterResponseMessage.dialogIdentifier            = "123456";

        _registrationCoordinator.sendRegisterResponse(sentRegisterResponseMessage);

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterResponseMessage.dialogIdentifier);

        assertEquals(sentRegisterResponseMessage.requesterReference.getAddress(), receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(sentRegisterResponseMessage.coordinationProtocolService.getAddress(), receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertEquals(sentRegisterResponseMessage.dialogIdentifier, receivedRegisterResponseMessage.dialogIdentifier);
    }

    public void testErrorWithDialogIdentifier()
        throws Exception
    {
        ErrorMessage            sentErrorMessage                = new ErrorMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJCOOR_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = "123456";

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

    private RegistrationRequester   _registrationRequester             = null;
    private String                  _registrationRequesterServiceURI   = null;
    private RegistrationCoordinator _registrationCoordinator           = null;
    private String                  _registrationCoordinatorServiceURI = null;
}
