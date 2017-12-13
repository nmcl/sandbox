/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrationMarshalExceptionTestCase.java
 */

package com.arjuna.wsc.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.*;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.tests.TestUtil;

public class RegistrationMarshalExceptionTestCase extends TestCase
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

    public void testRegister()
        throws Exception
    {
        RegisterResponseMessage sentRegisterResponseMessage = new RegisterResponseMessage();
        RegisterMessage         receivedRegisterMessage     = null;

        sentRegisterResponseMessage.requesterReference          = new PortReference(_registrationCoordinatorServiceURI);
        sentRegisterResponseMessage.coordinationProtocolService = new PortReference(_registrationCoordinatorServiceURI);

        _registrationCoordinator.sendRegisterResponse(sentRegisterResponseMessage);

        try
        {
            receivedRegisterMessage = _registrationCoordinator.recvRegister();

            fail("Expected exception: MarshalException");
        }
        catch (MarshalException marshalException)
        {
        }
    }

    public void testRegisterResponse()
        throws Exception
    {
        RegisterMessage         sentRegisterMessage             = new RegisterMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterMessage.registrationService        = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.requesterReference         = new PortReference(_registrationRequesterServiceURI);
        sentRegisterMessage.protocolIdentifier         = TestUtil.PROTOCOL_IDENTIFIER;
        sentRegisterMessage.participantProtocolService = new PortReference(TestUtil.PROTOCOL_PARTICIPANT_SERVICE);

        _registrationRequester.sendRegister(sentRegisterMessage);

        try
        {
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(null);

            fail("Expected exception: MarshalException");
        }
        catch (MarshalException marshalException)
        {
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
