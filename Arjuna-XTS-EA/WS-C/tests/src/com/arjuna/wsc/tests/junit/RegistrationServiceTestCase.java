/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrationServiceTestCase.java
 */

package com.arjuna.wsc.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.*;
import com.arjuna.wsc.impl.messaging.XMLUtil;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.tests.TestUtil;

public class RegistrationServiceTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String registrationRequesterJNDIName   = "registrationrequester";

        _registrationRequester             = (RegistrationRequester) initialContext.lookup(registrationRequesterJNDIName);
        _registrationRequesterServiceURI   = _registrationRequester.getServiceURI();
        _registrationCoordinatorServiceURI = TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL;
    }

    public void testKnownCoordinationTypeWithoutDialogIdentifier()
        throws Exception
    {
        RegisterMessage         sentRegisterMessage             = new RegisterMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = TestUtil.PROTOCOL_IDENTIFIER;
        sentRegisterMessage.participantProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.dialogIdentifier           = null;

        _registrationRequester.sendRegister(sentRegisterMessage);

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterMessage.dialogIdentifier);

        assertEquals(_registrationRequesterServiceURI, receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(TestUtil.PROTOCOL_COORDINATOR_SERVICE, receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertEquals(sentRegisterMessage.dialogIdentifier, receivedRegisterResponseMessage.dialogIdentifier);
    }

    public void testUnknownCoordinationTypeWithoutDialogIdentifier()
        throws Exception
    {
        RegisterMessage         sentRegisterMessage             = new RegisterMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = TestUtil.UNKNOWN_PROTOCOL_IDENTIFIER;
        sentRegisterMessage.participantProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.dialogIdentifier           = null;

        _registrationRequester.sendRegister(sentRegisterMessage);

        try
	{
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterMessage.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage errorMessage = errorMessageException.getErrorMessage();

            assertEquals(_registrationRequesterServiceURI, errorMessage.targetProtocolService.getAddress());
            assertEquals(XMLUtil.WSCOOR_NAMESPACE_URI, errorMessage.errorCode.namespace);
            assertEquals(XMLUtil.INVALIDPROTOCOL_ERROR_CODE, errorMessage.errorCode.name);
            assertEquals(sentRegisterMessage.dialogIdentifier, errorMessage.dialogIdentifier);
        }
    }

    public void testKnownCoordinationTypeWithDialogIdentifier()
        throws Exception
    {
        RegisterMessage         sentRegisterMessage             = new RegisterMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = TestUtil.PROTOCOL_IDENTIFIER;
        sentRegisterMessage.participantProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.dialogIdentifier           = "123456";

        _registrationRequester.sendRegister(sentRegisterMessage);

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterMessage.dialogIdentifier);

        assertEquals(_registrationRequesterServiceURI, receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(TestUtil.PROTOCOL_COORDINATOR_SERVICE, receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertEquals(sentRegisterMessage.dialogIdentifier, receivedRegisterResponseMessage.dialogIdentifier);
    }

    public void testUnknownCoordinationTypeWithDialogIdentifier()
        throws Exception
    {
        RegisterMessage         sentRegisterMessage             = new RegisterMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = TestUtil.UNKNOWN_PROTOCOL_IDENTIFIER;
        sentRegisterMessage.participantProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.dialogIdentifier           = "123456";

        _registrationRequester.sendRegister(sentRegisterMessage);

        try
	{
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterMessage.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage errorMessage = errorMessageException.getErrorMessage();

            assertEquals(_registrationRequesterServiceURI, errorMessage.targetProtocolService.getAddress());
            assertEquals(XMLUtil.WSCOOR_NAMESPACE_URI, errorMessage.errorCode.namespace);
            assertEquals(XMLUtil.INVALIDPROTOCOL_ERROR_CODE, errorMessage.errorCode.name);
            assertEquals(sentRegisterMessage.dialogIdentifier, errorMessage.dialogIdentifier);
        }
    }

    protected void tearDown()
        throws Exception
    {
    }

    private RegistrationRequester _registrationRequester             = null;
    private String                _registrationRequesterServiceURI   = null;
    private String                _registrationCoordinatorServiceURI = null;
}
