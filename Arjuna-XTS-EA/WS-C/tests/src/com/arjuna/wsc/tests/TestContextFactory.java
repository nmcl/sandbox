/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * TestContextFactory.java
 */

package com.arjuna.wsc.tests;

import com.arjuna.wsc.InvalidCreateParametersException;
import com.arjuna.wsc.ContextFactory;
import com.arjuna.wsc.messaging.AttributedDateTime;
import com.arjuna.wsc.messaging.CoordinationContext;
import com.arjuna.wsc.messaging.PortReference;

public class TestContextFactory implements ContextFactory
{
    public TestContextFactory(String coordinationType, String registrationServiceAddress)
    {
        _identifier          = 0;
        _coordinationType    = coordinationType;
        _registrationService = new PortReference(registrationServiceAddress);
    }

    public void install(String serviceAddress, String coordinationTypeURI)
    {
    }

    public CoordinationContext create(String coordinationTypeURI, AttributedDateTime expires, CoordinationContext currentContext)
        throws InvalidCreateParametersException
    {
        if (coordinationTypeURI.equals(TestUtil.INVALID_CREATE_PARAMETERS_COORDINATION_TYPE))
            throw new InvalidCreateParametersException();

        TestCoordinationContext testCoordinationContext = new TestCoordinationContext();

        testCoordinationContext.identifier          = String.valueOf(_identifier);
        testCoordinationContext.coordinationType    = _coordinationType;
        testCoordinationContext.registrationService = _registrationService;
        testCoordinationContext.extensionValue      = TestUtil.TEST_EXTENSION_VALUE;
        _identifier++;

        return testCoordinationContext;
    }

    public void uninstall(String serviceAddress, String coordinationTypeURI)
    {
    }

    private int           _identifier          = 0;
    private String        _coordinationType    = null;
    private PortReference _registrationService = null;
}
