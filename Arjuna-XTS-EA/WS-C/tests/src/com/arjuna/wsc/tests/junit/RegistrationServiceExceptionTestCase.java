/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrationServiceExceptionTestCase.java
 */

package com.arjuna.wsc.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.*;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.impl.messaging.XMLUtil;
import com.arjuna.wsc.tests.TestUtil;

public class RegistrationServiceExceptionTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String registrationRequesterJNDIName = "registrationrequester";

        _registrationRequester             = (RegistrationRequester) initialContext.lookup(registrationRequesterJNDIName);
        _registrationRequesterServiceURI   = _registrationRequester.getServiceURI();
        _registrationCoordinatorServiceURI = TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL;
    }

    public void testAlreadyRegisteredProtocolIdentifierException()
        throws Exception
    {
        RegisterMessage         sentRegisterMessage             = new RegisterMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = TestUtil.ALREADY_REGISTERED_PROTOCOL_IDENTIFIER;
        sentRegisterMessage.participantProtocolService = new PortReference(_registrationRequesterServiceURI);

        _registrationRequester.sendRegister(sentRegisterMessage);

        try
	{
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(null);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage errorMessage = errorMessageException.getErrorMessage();

            assertEquals(_registrationRequesterServiceURI, errorMessage.targetProtocolService.getAddress());
            assertEquals(XMLUtil.WSCOOR_NAMESPACE_URI, errorMessage.errorCode.namespace);
            assertEquals(XMLUtil.ALREADYREGISTERED_ERROR_CODE, errorMessage.errorCode.name);
        }
    }

    public void testInvalidProtocolProtocolIdentifierException()
        throws Exception
    {
        RegisterMessage         sentRegisterMessage             = new RegisterMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = TestUtil.INVALID_PROTOCOL_PROTOCOL_IDENTIFIER;
        sentRegisterMessage.participantProtocolService = new PortReference(_registrationRequesterServiceURI);

        _registrationRequester.sendRegister(sentRegisterMessage);

        try
	{
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(null);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage errorMessage = errorMessageException.getErrorMessage();

            assertEquals(_registrationRequesterServiceURI, errorMessage.targetProtocolService.getAddress());
            assertEquals(XMLUtil.WSCOOR_NAMESPACE_URI, errorMessage.errorCode.namespace);
            assertEquals(XMLUtil.INVALIDPROTOCOL_ERROR_CODE, errorMessage.errorCode.name);
        }
    }

    public void testInvalidStateProtocolIdentifierException()
        throws Exception
    {
        RegisterMessage         sentRegisterMessage             = new RegisterMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = TestUtil.INVALID_STATE_PROTOCOL_IDENTIFIER;
        sentRegisterMessage.participantProtocolService = new PortReference(_registrationRequesterServiceURI);

        _registrationRequester.sendRegister(sentRegisterMessage);

        try
	{
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(null);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage errorMessage = errorMessageException.getErrorMessage();

            assertEquals(_registrationRequesterServiceURI, errorMessage.targetProtocolService.getAddress());
            assertEquals(XMLUtil.WSCOOR_NAMESPACE_URI, errorMessage.errorCode.namespace);
            assertEquals(XMLUtil.INVALIDSTATE_ERROR_CODE, errorMessage.errorCode.name);
        }
    }

    public void testNoActivityProtocolIdentifierException()
        throws Exception
    {
        RegisterMessage         sentRegisterMessage             = new RegisterMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = TestUtil.NO_ACTIVITY_PROTOCOL_IDENTIFIER;
        sentRegisterMessage.participantProtocolService = new PortReference(_registrationRequesterServiceURI);

        _registrationRequester.sendRegister(sentRegisterMessage);

        try
	{
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(null);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage errorMessage = errorMessageException.getErrorMessage();

            assertEquals(_registrationRequesterServiceURI, errorMessage.targetProtocolService.getAddress());
            assertEquals(XMLUtil.WSCOOR_NAMESPACE_URI, errorMessage.errorCode.namespace);
            assertEquals(XMLUtil.NOACTIVITY_ERROR_CODE, errorMessage.errorCode.name);
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
