/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TestSuite.java
 */

package com.arjuna.wst.tests.junit;

public class TestSuite extends junit.framework.TestSuite
{
    public TestSuite()
    {
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.CompletionParticipantTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.CompletionCoordinatorTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.CompletionWithAckParticipantTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.CompletionWithAckCoordinatorTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.PhaseZeroParticipantTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.PhaseZeroCoordinatorTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.TwoPCParticipantTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.TwoPCCoordinatorTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.OutcomeNotificationParticipantTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.OutcomeNotificationCoordinatorTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.CompletionServiceTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.CompletionWithAckServiceTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.PhaseZeroServiceTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.TwoPCServiceTestCase.class));
        addTest(new junit.framework.TestSuite(com.arjuna.wst.tests.junit.OutcomeNotificationServiceTestCase.class));
    }
}
