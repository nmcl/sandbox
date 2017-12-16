/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * OutcomeNotificationServiceTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.SystemException;
import com.arjuna.wst.OutcomeNotificationParticipant;
import com.arjuna.wst.stub.OutcomeNotificationStub;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class OutcomeNotificationServiceTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String outcomeNotificationParticipantJNDIName = "outcomenotificationparticipant-srv";
        String outcomeNotificationCoordinatorJNDIName = "outcomenotificationcoordinator";

        com.arjuna.wst.messaging.OutcomeNotificationParticipant outcomeNotificationParticipantEndpoint = (com.arjuna.wst.messaging.OutcomeNotificationParticipant) initialContext.lookup(outcomeNotificationParticipantJNDIName);
        com.arjuna.wst.messaging.OutcomeNotificationCoordinator outcomeNotificationCoordinatorEndpoint = (com.arjuna.wst.messaging.OutcomeNotificationCoordinator) initialContext.lookup(outcomeNotificationCoordinatorJNDIName);

        PortReference noExceptionOutcomeNotificationCoordinator = new PortReference(outcomeNotificationParticipantEndpoint.getServiceURI());
        noExceptionOutcomeNotificationCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.NOEXCEPTION_PARTICIPANT_IDENTIFIER);

        PortReference systemExceptionOutcomeNotificationCoordinator = new PortReference(outcomeNotificationParticipantEndpoint.getServiceURI());
        systemExceptionOutcomeNotificationCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.SYSTEMEXCEPTION_PARTICIPANT_IDENTIFIER);

        PortReference nonexistentOutcomeNotificationCoordinator = new PortReference(outcomeNotificationParticipantEndpoint.getServiceURI());
        nonexistentOutcomeNotificationCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.NONEXISTENT_PARTICIPANT_IDENTIFIER);

        _noExceptionOutcomeNotificationStub     = new OutcomeNotificationStub(outcomeNotificationCoordinatorEndpoint, noExceptionOutcomeNotificationCoordinator);
        _systemExceptionOutcomeNotificationStub = new OutcomeNotificationStub(outcomeNotificationCoordinatorEndpoint, systemExceptionOutcomeNotificationCoordinator);
        _nonexistentOutcomeNotificationStub     = new OutcomeNotificationStub(outcomeNotificationCoordinatorEndpoint, nonexistentOutcomeNotificationCoordinator);
    }

    public void testCommittedWithNoException()
        throws Exception
    {
        _noExceptionOutcomeNotificationStub.committed();
    }

    public void testAbortedWithNoException()
        throws Exception
    {
        _noExceptionOutcomeNotificationStub.aborted();
    }

    public void testUnknownWithNoException()
        throws Exception
    {
        _noExceptionOutcomeNotificationStub.unknown();
    }

    public void testErrorWithNoException()
        throws Exception
    {
        _noExceptionOutcomeNotificationStub.error();
    }

    public void testCommitedWithSystemException()
        throws Exception
    {
        try
        {
            _systemExceptionOutcomeNotificationStub.committed();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testAbortedWithSystemException()
        throws Exception
    {
        try
        {
            _systemExceptionOutcomeNotificationStub.aborted();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testUnknownWithSystemException()
        throws Exception
    {
        _systemExceptionOutcomeNotificationStub.unknown();
    }

    public void testErrorWithSystemException()
        throws Exception
    {
        _systemExceptionOutcomeNotificationStub.error();
    }

    public void testCommitedWithNonExistent()
        throws Exception
    {
        try
        {
            _nonexistentOutcomeNotificationStub.committed();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testAbortedWithNonExistent()
        throws Exception
    {
        try
        {
            _nonexistentOutcomeNotificationStub.aborted();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testUnknownWithNonExistent()
        throws Exception
    {
        _nonexistentOutcomeNotificationStub.unknown();
    }

    public void testErrorWithNonExistent()
        throws Exception
    {
        _nonexistentOutcomeNotificationStub.error();
    }

    protected void tearDown()
        throws Exception
    {
        _noExceptionOutcomeNotificationStub     = null;
        _systemExceptionOutcomeNotificationStub = null;
        _nonexistentOutcomeNotificationStub     = null;
    }

    private OutcomeNotificationParticipant _noExceptionOutcomeNotificationStub     = null;
    private OutcomeNotificationParticipant _systemExceptionOutcomeNotificationStub = null;
    private OutcomeNotificationParticipant _nonexistentOutcomeNotificationStub     = null;
}
