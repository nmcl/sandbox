/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * PhaseZeroServiceTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.SystemException;
import com.arjuna.wst.PhaseZeroParticipant;
import com.arjuna.wst.stub.PhaseZeroStub;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class PhaseZeroServiceTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String phaseZeroParticipantJNDIName = "phasezeroparticipant-srv";
        String phaseZeroCoordinatorJNDIName = "phasezerocoordinator";

        com.arjuna.wst.messaging.PhaseZeroParticipant phaseZeroParticipantEndpoint = (com.arjuna.wst.messaging.PhaseZeroParticipant) initialContext.lookup(phaseZeroParticipantJNDIName);
        com.arjuna.wst.messaging.PhaseZeroCoordinator phaseZeroCoordinatorEndpoint = (com.arjuna.wst.messaging.PhaseZeroCoordinator) initialContext.lookup(phaseZeroCoordinatorJNDIName);

        PortReference noExceptionPhaseZeroCoordinator = new PortReference(phaseZeroParticipantEndpoint.getServiceURI());
        noExceptionPhaseZeroCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.NOEXCEPTION_PARTICIPANT_IDENTIFIER);

        PortReference systemExceptionPhaseZeroCoordinator = new PortReference(phaseZeroParticipantEndpoint.getServiceURI());
        systemExceptionPhaseZeroCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.SYSTEMEXCEPTION_PARTICIPANT_IDENTIFIER);

        PortReference nonexistentPhaseZeroCoordinator = new PortReference(phaseZeroParticipantEndpoint.getServiceURI());
        nonexistentPhaseZeroCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.NONEXISTENT_PARTICIPANT_IDENTIFIER);

        _noExceptionPhaseZeroStub     = new PhaseZeroStub(phaseZeroCoordinatorEndpoint, noExceptionPhaseZeroCoordinator);
        _systemExceptionPhaseZeroStub = new PhaseZeroStub(phaseZeroCoordinatorEndpoint, systemExceptionPhaseZeroCoordinator);
        _nonexistentPhaseZeroStub     = new PhaseZeroStub(phaseZeroCoordinatorEndpoint, nonexistentPhaseZeroCoordinator);
    }

    public void testPhaseZeroWithNoException()
        throws Exception
    {
        _noExceptionPhaseZeroStub.phaseZero();
    }

    public void testErrorWithNoException()
        throws Exception
    {
        _noExceptionPhaseZeroStub.error();
    }

    public void testPhaseZeroWithSystemException()
        throws Exception
    {
        try
        {
            _systemExceptionPhaseZeroStub.phaseZero();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testErrorWithSystemException()
        throws Exception
    {
        _systemExceptionPhaseZeroStub.error();
    }

    public void testPhaseZeroWithNonExistent()
        throws Exception
    {
        try
        {
            _nonexistentPhaseZeroStub.phaseZero();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testErrorWithNonExistent()
        throws Exception
    {
        _nonexistentPhaseZeroStub.error();
    }

    protected void tearDown()
        throws Exception
    {
        _noExceptionPhaseZeroStub     = null;
        _systemExceptionPhaseZeroStub = null;
        _nonexistentPhaseZeroStub     = null;
    }

    private PhaseZeroParticipant _noExceptionPhaseZeroStub     = null;
    private PhaseZeroParticipant _systemExceptionPhaseZeroStub = null;
    private PhaseZeroParticipant _nonexistentPhaseZeroStub     = null;
}
