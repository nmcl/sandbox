/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TwoPCServiceTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.*;
import com.arjuna.wst.stub.TwoPCStub;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class TwoPCServiceTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String twoPCParticipantJNDIName = "twopcparticipant-srv";
        String twoPCCoordinatorJNDIName = "twopccoordinator";

        com.arjuna.wst.messaging.TwoPCParticipant twoPCParticipantEndpoint = (com.arjuna.wst.messaging.TwoPCParticipant) initialContext.lookup(twoPCParticipantJNDIName);
        com.arjuna.wst.messaging.TwoPCCoordinator twoPCCoordinatorEndpoint = (com.arjuna.wst.messaging.TwoPCCoordinator) initialContext.lookup(twoPCCoordinatorJNDIName);

        PortReference preparedVoteTwoPCCoordinator = new PortReference(twoPCParticipantEndpoint.getServiceURI());
        preparedVoteTwoPCCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.PREPAREDVOTE_PARTICIPANT_IDENTIFIER);

        PortReference abortedVoteTwoPCCoordinator = new PortReference(twoPCParticipantEndpoint.getServiceURI());
        abortedVoteTwoPCCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.ABORTEDVOTE_PARTICIPANT_IDENTIFIER);

        PortReference readOnlyVoteTwoPCCoordinator = new PortReference(twoPCParticipantEndpoint.getServiceURI());
        readOnlyVoteTwoPCCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.READONLYVOTE_PARTICIPANT_IDENTIFIER);

        PortReference noExceptionTwoPCCoordinator = new PortReference(twoPCParticipantEndpoint.getServiceURI());
        noExceptionTwoPCCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.NOEXCEPTION_PARTICIPANT_IDENTIFIER);

        PortReference transactionRolledBackExceptionTwoPCCoordinator = new PortReference(twoPCParticipantEndpoint.getServiceURI());
        transactionRolledBackExceptionTwoPCCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.TRANSACTIONROLLEDBACKEXCEPTION_PARTICIPANT_IDENTIFIER);

        PortReference wrongStateExceptionTwoPCCoordinator = new PortReference(twoPCParticipantEndpoint.getServiceURI());
        wrongStateExceptionTwoPCCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.WRONGSTATEEXCEPTION_PARTICIPANT_IDENTIFIER);

        PortReference systemExceptionTwoPCCoordinator = new PortReference(twoPCParticipantEndpoint.getServiceURI());
        systemExceptionTwoPCCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.SYSTEMEXCEPTION_PARTICIPANT_IDENTIFIER);

        PortReference nonexistentTwoPCCoordinator = new PortReference(twoPCParticipantEndpoint.getServiceURI());
        nonexistentTwoPCCoordinator.addExtension(XMLUtil.PARTICIPANT_IDENTIFIER_TAG, TestUtil.NONEXISTENT_PARTICIPANT_IDENTIFIER);

        _preparedVoteTwoPCStub                   = new TwoPCStub(twoPCCoordinatorEndpoint, preparedVoteTwoPCCoordinator);
        _abortedVoteTwoPCStub                    = new TwoPCStub(twoPCCoordinatorEndpoint, abortedVoteTwoPCCoordinator);
        _readOnlyVoteTwoPCStub                   = new TwoPCStub(twoPCCoordinatorEndpoint, readOnlyVoteTwoPCCoordinator);
        _noExceptionTwoPCStub                    = new TwoPCStub(twoPCCoordinatorEndpoint, noExceptionTwoPCCoordinator);
        _transactionRolledBackExceptionTwoPCStub = new TwoPCStub(twoPCCoordinatorEndpoint, transactionRolledBackExceptionTwoPCCoordinator);
        _wrongStateExceptionTwoPCStub            = new TwoPCStub(twoPCCoordinatorEndpoint, wrongStateExceptionTwoPCCoordinator);
        _systemExceptionTwoPCStub                = new TwoPCStub(twoPCCoordinatorEndpoint, systemExceptionTwoPCCoordinator);
        _nonexistentTwoPCStub                    = new TwoPCStub(twoPCCoordinatorEndpoint, nonexistentTwoPCCoordinator);
    }

    public void testPrepareWithPreparedVote()
        throws Exception
    {
        Vote vote = _preparedVoteTwoPCStub.prepare();

        assertNotNull(vote);
        assertTrue("Expected vote \"Prepared\" got \"" + vote.getClass().getName() + "\"", vote instanceof com.arjuna.wst.Prepared);
    }

    public void testPrepareWithAbortedVote()
        throws Exception
    {
        Vote vote = _abortedVoteTwoPCStub.prepare();

        assertNotNull(vote);
        assertTrue("Expected vote \"Aborted\" got \"" + vote.getClass().getName() + "\"", vote instanceof com.arjuna.wst.Aborted);
    }

    public void testPrepareWithReadOnlyVote()
        throws Exception
    {
        Vote vote = _readOnlyVoteTwoPCStub.prepare();

        assertNotNull(vote);
        assertTrue("Expected vote \"ReadOnly\" got \"" + vote.getClass().getName() + "\"", vote instanceof com.arjuna.wst.ReadOnly);
    }

    public void testCommitWithNoException()
        throws Exception
    {
        _noExceptionTwoPCStub.commit();
    }

    public void testRollbackWithNoException()
        throws Exception
    {
        _noExceptionTwoPCStub.rollback();
    }

    public void testCommitOnePhaseWithNoException()
        throws Exception
    {
        _noExceptionTwoPCStub.commitOnePhase();
    }

    public void testUnknownWithNoException()
        throws Exception
    {
        _noExceptionTwoPCStub.unknown();
    }

    public void testErrorWithNoException()
        throws Exception
    {
        _noExceptionTwoPCStub.error();
    }

    public void testCommitOnePhaseWithTransactionRolledBackException()
        throws Exception
    {
        try
        {
            _transactionRolledBackExceptionTwoPCStub.commitOnePhase();
            fail("Expected exception \"TransactionRolledBackException\"");
        }
        catch (TransactionRolledBackException transactionRolledBackException)
        {
        }
    }

    public void testPrepareWithWrongStateException()
        throws Exception
    {
        try
        {
            _wrongStateExceptionTwoPCStub.prepare();
            fail("Expected exception \"WrongStateException\"");
        }
        catch (WrongStateException wrongStateException)
        {
        }
    }

    public void testCommitWithWrongStateException()
        throws Exception
    {
        try
        {
            _wrongStateExceptionTwoPCStub.commit();
            fail("Expected exception \"WrongStateException\"");
        }
        catch (WrongStateException wrongStateException)
        {
        }
    }

    public void testRollbackWithWrongStateException()
        throws Exception
    {
        try
        {
            _wrongStateExceptionTwoPCStub.rollback();
            fail("Expected exception \"WrongStateException\"");
        }
        catch (WrongStateException wrongStateException)
        {
        }
    }

    public void testCommitOnePhaseWithWrongStateException()
        throws Exception
    {
        try
        {
            _wrongStateExceptionTwoPCStub.commitOnePhase();
            fail("Expected exception \"WrongStateException\"");
        }
        catch (WrongStateException wrongStateException)
        {
        }
    }

    public void testPrepareWithSystemException()
        throws Exception
    {
        try
        {
            _systemExceptionTwoPCStub.prepare();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testCommitWithSystemException()
        throws Exception
    {
        try
        {
            _systemExceptionTwoPCStub.commit();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testRollbackWithSystemException()
        throws Exception
    {
        try
        {
            _systemExceptionTwoPCStub.rollback();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testCommitOnePhaseWithSystemException()
        throws Exception
    {
        try
        {
            _systemExceptionTwoPCStub.commitOnePhase();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testUnknownWithSystemException()
        throws Exception
    {
        _systemExceptionTwoPCStub.unknown();
    }

    public void testErrorWithSystemException()
        throws Exception
    {
        _systemExceptionTwoPCStub.error();
    }

    public void testPrepareWithNonExistent()
        throws Exception
    {
        try
        {
            _nonexistentTwoPCStub.prepare();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testCommitWithNonExistent()
        throws Exception
    {
        try
        {
            _nonexistentTwoPCStub.commit();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testRollbackWithNonExistent()
        throws Exception
    {
        try
        {
            _nonexistentTwoPCStub.rollback();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testCommitOnePhaseWithNonExistent()
        throws Exception
    {
        try
        {
            _nonexistentTwoPCStub.commitOnePhase();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testUnknownWithNonExistent()
        throws Exception
    {
        _nonexistentTwoPCStub.unknown();
    }

    public void testErrorWithNonExistent()
        throws Exception
    {
        _nonexistentTwoPCStub.error();
    }

    protected void tearDown()
        throws Exception
    {
        _preparedVoteTwoPCStub                   = null;
        _abortedVoteTwoPCStub                    = null;
        _readOnlyVoteTwoPCStub                   = null;
        _noExceptionTwoPCStub                    = null;
        _transactionRolledBackExceptionTwoPCStub = null;
        _wrongStateExceptionTwoPCStub            = null;
        _systemExceptionTwoPCStub                = null;
        _nonexistentTwoPCStub                    = null;
    }

    private TwoPCParticipant _preparedVoteTwoPCStub                   = null;
    private TwoPCParticipant _abortedVoteTwoPCStub                    = null;
    private TwoPCParticipant _readOnlyVoteTwoPCStub                   = null;
    private TwoPCParticipant _noExceptionTwoPCStub                    = null;
    private TwoPCParticipant _transactionRolledBackExceptionTwoPCStub = null;
    private TwoPCParticipant _wrongStateExceptionTwoPCStub            = null;
    private TwoPCParticipant _systemExceptionTwoPCStub                = null;
    private TwoPCParticipant _nonexistentTwoPCStub                    = null;
}
