/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * TestDeploymentServlet.java
 */

package com.arjuna.wst.tests.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wst.CompletionCoordinatorParticipant;
import com.arjuna.wst.CompletionWithAckCoordinatorParticipant;
import com.arjuna.wst.PhaseZeroParticipant;
import com.arjuna.wst.TwoPCParticipant;
import com.arjuna.wst.OutcomeNotificationParticipant;
import com.arjuna.wst.dispatcher.CompletionDispatcher;
import com.arjuna.wst.dispatcher.CompletionWithAckDispatcher;
import com.arjuna.wst.dispatcher.PhaseZeroDispatcher;
import com.arjuna.wst.dispatcher.TwoPCDispatcher;
import com.arjuna.wst.dispatcher.OutcomeNotificationDispatcher;
import com.arjuna.wst.tests.*;

public class TestDeploymentServlet extends HttpServlet
{
    public void init(ServletConfig config)
        throws ServletException
    {
        super.init(config);

        try
        {
            InitialContext initialContext = new InitialContext();

            String                        completionDispatcherJNDIName          = config.getInitParameter("CompletionDispatcherJNDIName");
            CompletionDispatcher          completionDispatcher                  = (CompletionDispatcher) initialContext.lookup(completionDispatcherJNDIName);
            String                        completionWithAckDispatcherJNDIName   = config.getInitParameter("CompletionWithAckDispatcherJNDIName");
            CompletionWithAckDispatcher   completionWithAckDispatcher           = (CompletionWithAckDispatcher) initialContext.lookup(completionWithAckDispatcherJNDIName);
            String                        phaseZeroDispatcherJNDIName           = config.getInitParameter("PhaseZeroDispatcherJNDIName");
            PhaseZeroDispatcher           phaseZeroDispatcher                   = (PhaseZeroDispatcher) initialContext.lookup(phaseZeroDispatcherJNDIName);
            String                        twoPCDispatcherJNDIName               = config.getInitParameter("TwoPCDispatcherJNDIName");
            TwoPCDispatcher               twoPCDispatcher                       = (TwoPCDispatcher) initialContext.lookup(twoPCDispatcherJNDIName);
            String                        outcomeNotificationDispatcherJNDIName = config.getInitParameter("OutcomeNotificationDispatcherJNDIName");
            OutcomeNotificationDispatcher outcomeNotificationDispatcher         = (OutcomeNotificationDispatcher) initialContext.lookup(outcomeNotificationDispatcherJNDIName);

            CompletionCoordinatorParticipant testNoExceptionCompletionCoordinatorParticipant                 = new TestNoExceptionCompletionCoordinatorParticipant();
            CompletionCoordinatorParticipant testTransactionRolledExceptionCompletionCoordinatorParticipant  = new TestTransactionRolledBackExceptionCompletionCoordinatorParticipant();
            CompletionCoordinatorParticipant testUnknownTransactionExceptionCompletionCoordinatorParticipant = new TestUnknownTransactionExceptionCompletionCoordinatorParticipant();
            CompletionCoordinatorParticipant testSystemExceptionCompletionCoordinatorParticipant             = new TestSystemExceptionCompletionCoordinatorParticipant();

            completionDispatcher.activateObject(testNoExceptionCompletionCoordinatorParticipant, TestUtil.NOEXCEPTION_TRANSACTION_IDENTIFIER);
            completionDispatcher.activateObject(testTransactionRolledExceptionCompletionCoordinatorParticipant, TestUtil.TRANSACTIONROLLEDBACKEXCEPTION_TRANSACTION_IDENTIFIER);
            completionDispatcher.activateObject(testUnknownTransactionExceptionCompletionCoordinatorParticipant, TestUtil.UNKNOWNTRANSACTIONEXCEPTION_TRANSACTION_IDENTIFIER);
            completionDispatcher.activateObject(testSystemExceptionCompletionCoordinatorParticipant, TestUtil.SYSTEMEXCEPTION_TRANSACTION_IDENTIFIER);

            CompletionWithAckCoordinatorParticipant testNoExceptionCompletionWithAckCoordinatorParticipant                 = new TestNoExceptionCompletionWithAckCoordinatorParticipant();
            CompletionWithAckCoordinatorParticipant testTransactionRolledExceptionCompletionWithAckCoordinatorParticipant  = new TestTransactionRolledBackExceptionCompletionWithAckCoordinatorParticipant();
            CompletionWithAckCoordinatorParticipant testUnknownTransactionExceptionCompletionWithAckCoordinatorParticipant = new TestUnknownTransactionExceptionCompletionWithAckCoordinatorParticipant();
            CompletionWithAckCoordinatorParticipant testSystemExceptionCompletionWithAckCoordinatorParticipant             = new TestSystemExceptionCompletionWithAckCoordinatorParticipant();

            completionWithAckDispatcher.activateObject(testNoExceptionCompletionWithAckCoordinatorParticipant, TestUtil.NOEXCEPTION_TRANSACTION_IDENTIFIER);
            completionWithAckDispatcher.activateObject(testTransactionRolledExceptionCompletionWithAckCoordinatorParticipant, TestUtil.TRANSACTIONROLLEDBACKEXCEPTION_TRANSACTION_IDENTIFIER);
            completionWithAckDispatcher.activateObject(testUnknownTransactionExceptionCompletionWithAckCoordinatorParticipant, TestUtil.UNKNOWNTRANSACTIONEXCEPTION_TRANSACTION_IDENTIFIER);
            completionWithAckDispatcher.activateObject(testSystemExceptionCompletionWithAckCoordinatorParticipant, TestUtil.SYSTEMEXCEPTION_TRANSACTION_IDENTIFIER);

            PhaseZeroParticipant testNoExceptionPhaseZeroParticipant     = new TestNoExceptionPhaseZeroParticipant();
            PhaseZeroParticipant testSystemExceptionPhaseZeroParticipant = new TestSystemExceptionPhaseZeroParticipant();

            phaseZeroDispatcher.activateObject(testNoExceptionPhaseZeroParticipant, TestUtil.NOEXCEPTION_PARTICIPANT_IDENTIFIER);
            phaseZeroDispatcher.activateObject(testSystemExceptionPhaseZeroParticipant, TestUtil.SYSTEMEXCEPTION_PARTICIPANT_IDENTIFIER);

            TwoPCParticipant testPreparedVoteTwoPCParticipant                   = new TestPreparedVoteTwoPCParticipant();
            TwoPCParticipant testAbortedVoteTwoPCParticipant                    = new TestAbortedVoteTwoPCParticipant();
            TwoPCParticipant testReadOnlyTwoPCParticipant                       = new TestReadOnlyVoteTwoPCParticipant();
            TwoPCParticipant testNoExceptionTwoPCParticipant                    = new TestNoExceptionTwoPCParticipant();
            TwoPCParticipant testTransactionRolledBackExceptionTwoPCParticipant = new TestTransactionRolledBackExceptionTwoPCParticipant();
            TwoPCParticipant testWrongStateExceptionTwoPCParticipant            = new TestWrongStateExceptionTwoPCParticipant();
            TwoPCParticipant testSystemExceptionTwoPCParticipant                = new TestSystemExceptionTwoPCParticipant();

            twoPCDispatcher.activateObject(testPreparedVoteTwoPCParticipant, TestUtil.PREPAREDVOTE_PARTICIPANT_IDENTIFIER);
            twoPCDispatcher.activateObject(testAbortedVoteTwoPCParticipant, TestUtil.ABORTEDVOTE_PARTICIPANT_IDENTIFIER);
            twoPCDispatcher.activateObject(testReadOnlyTwoPCParticipant, TestUtil.READONLYVOTE_PARTICIPANT_IDENTIFIER);
            twoPCDispatcher.activateObject(testNoExceptionTwoPCParticipant, TestUtil.NOEXCEPTION_PARTICIPANT_IDENTIFIER);
            twoPCDispatcher.activateObject(testTransactionRolledBackExceptionTwoPCParticipant, TestUtil.TRANSACTIONROLLEDBACKEXCEPTION_PARTICIPANT_IDENTIFIER);
            twoPCDispatcher.activateObject(testWrongStateExceptionTwoPCParticipant, TestUtil.WRONGSTATEEXCEPTION_PARTICIPANT_IDENTIFIER);
            twoPCDispatcher.activateObject(testSystemExceptionTwoPCParticipant, TestUtil.SYSTEMEXCEPTION_PARTICIPANT_IDENTIFIER);

            OutcomeNotificationParticipant testNoExceptionOutcomeNotificationParticipant     = new TestNoExceptionOutcomeNotificationParticipant();
            OutcomeNotificationParticipant testSystemExceptionOutcomeNotificationParticipant = new TestSystemExceptionOutcomeNotificationParticipant();

            outcomeNotificationDispatcher.activateObject(testNoExceptionOutcomeNotificationParticipant, TestUtil.NOEXCEPTION_PARTICIPANT_IDENTIFIER);
            outcomeNotificationDispatcher.activateObject(testSystemExceptionOutcomeNotificationParticipant, TestUtil.SYSTEMEXCEPTION_PARTICIPANT_IDENTIFIER);

            log("Test Deployment: done");
        }
        catch (Exception exception)
        {
            log("Test Deployment: failed", exception);

            throw new ServletException("Test Deployment Servlet: init failed", exception);
        }
        catch (Error error)
        {
            log("Test Deployment: failed", error);

            throw new ServletException("Test Deployment Servlet: init failed", error);
        }
    }

    public void destroy()
    {
        log("Test Deployment: undone");

        super.destroy();
    }
}
