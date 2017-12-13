/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * TestSuite.java
 */

package com.arjuna.wsc.tests.junit;

public class TestSuite extends junit.framework.TestSuite
{
    public TestSuite()
    {
        addTest(new junit.framework.TestSuite(com.arjuna.wsc.tests.junit.ActivationTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wsc.tests.junit.RegistrationTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wsc.tests.junit.MultiSendActivationTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wsc.tests.junit.MultiSendRegistrationTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wsc.tests.junit.ActivationMarshalExceptionTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wsc.tests.junit.RegistrationMarshalExceptionTestCase.class));

        addTest(new junit.framework.TestSuite(com.arjuna.wsc.tests.junit.ActivationServiceTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wsc.tests.junit.ActivationServiceExceptionTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wsc.tests.junit.RegistrationServiceTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wsc.tests.junit.RegistrationServiceExceptionTestCase.class));

        addTest(new junit.framework.TestSuite(com.arjuna.wsc.tests.junit.EnduranceTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wsc.tests.junit.ThreadedEnduranceTestCase.class));
    }
}