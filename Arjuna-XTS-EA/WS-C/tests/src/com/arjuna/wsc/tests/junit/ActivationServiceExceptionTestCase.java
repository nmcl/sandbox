/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ActivationServiceExceptionTestCase.java
 */

package com.arjuna.wsc.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.*;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.impl.messaging.XMLUtil;
import com.arjuna.wsc.tests.TestUtil;

public class ActivationServiceExceptionTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String activationRequesterJNDIName = "activationrequester";

        _activationRequester             = (ActivationRequester) initialContext.lookup(activationRequesterJNDIName);
        _activationRequesterServiceURI   = _activationRequester.getServiceURI();
        _activationCoordinatorServiceURI = TestUtil.ACTIVATION_COORDINATOR_SERVICE_URL;
    }

    public void testInvalidCreateParametersException()
        throws Exception
    {
        CreateCoordinationContextMessage         sentCreateCoordinationContextMessage             = new CreateCoordinationContextMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        sentCreateCoordinationContextMessage.activationService  = new PortReference(_activationCoordinatorServiceURI);
        sentCreateCoordinationContextMessage.requesterReference = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextMessage.coordinationType   = TestUtil.INVALID_CREATE_PARAMETERS_COORDINATION_TYPE;

        _activationRequester.sendCreateCoordinationContext(sentCreateCoordinationContextMessage);

        try
	{
            receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(null);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage errorMessage = errorMessageException.getErrorMessage();

            assertEquals(_activationRequesterServiceURI, errorMessage.targetProtocolService.getAddress());
            assertEquals(XMLUtil.WSCOOR_NAMESPACE_URI, errorMessage.errorCode.namespace);
            assertEquals(XMLUtil.INVALIDCREATEPARAMETERS_ERROR_CODE, errorMessage.errorCode.name);
        }
    }

    protected void tearDown()
        throws Exception
    {
    }

    private ActivationRequester   _activationRequester             = null;
    private String                _activationRequesterServiceURI   = null;
    private String                _activationCoordinatorServiceURI = null;
}
