/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ActivationMarshalExceptionTestCase.java
 */

package com.arjuna.wsc.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.*;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.tests.TestUtil;

public class ActivationMarshalExceptionTestCase extends TestCase
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

    public void testCreateCoordinationContext()
        throws Exception
    {
        CreateCoordinationContextResponseMessage sentCreateCoordinationContextResponseMessage = new CreateCoordinationContextResponseMessage();
        CreateCoordinationContextMessage         receivedCreateCoordinationContextMessage     = null;

        sentCreateCoordinationContextResponseMessage.requesterReference  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextResponseMessage.coordinationContext = null;

        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage);

        try
        {
            receivedCreateCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

            fail("Expected exception: MarshalException");
        }
        catch (MarshalException marshalException)
        {
        }
    }

    public void testCreateCoordinationContextResponse()
        throws Exception
    {
        CreateCoordinationContextMessage         sentCreateCoordinationContextMessage             = new CreateCoordinationContextMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        sentCreateCoordinationContextMessage.activationService  = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType   = TestUtil.COORDINATION_TYPE;

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        try
        {
            receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(null);

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

    private ActivationRequester   _activationRequester             = null;
    private String                _activationRequesterServiceURI   = null;
    private ActivationCoordinator _activationCoordinator           = null;
    private String                _activationCoordinatorServiceURI = null;
}
