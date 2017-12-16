/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionWithAckServiceTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.SystemException;
import com.arjuna.wst.TransactionRolledBackException;
import com.arjuna.wst.UnknownTransactionException;
import com.arjuna.wst.CompletionWithAckCoordinatorParticipant;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.stub.CompletionWithAckStub;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class CompletionWithAckServiceTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String completionWithAckParticipantJNDIName = "completionwithackparticipant";
        String completionWithAckCoordinatorJNDIName = "completionwithackcoordinator-srv";

        CompletionWithAckParticipant completionWithAckParticipantEndpoint = (CompletionWithAckParticipant) initialContext.lookup(completionWithAckParticipantJNDIName);
        CompletionWithAckCoordinator completionWithAckCoordinatorEndpoint = (CompletionWithAckCoordinator) initialContext.lookup(completionWithAckCoordinatorJNDIName);

        PortReference noExceptionCompletionWithAckCoordinator = new PortReference(completionWithAckCoordinatorEndpoint.getServiceURI());
        noExceptionCompletionWithAckCoordinator.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, TestUtil.NOEXCEPTION_TRANSACTION_IDENTIFIER);

        PortReference transactionRolledBackExceptionCompletionWithAckCoordinator = new PortReference(completionWithAckCoordinatorEndpoint.getServiceURI());
        transactionRolledBackExceptionCompletionWithAckCoordinator.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, TestUtil.TRANSACTIONROLLEDBACKEXCEPTION_TRANSACTION_IDENTIFIER);

        PortReference unknownTransactionExceptionCompletionWithAckCoordinator = new PortReference(completionWithAckCoordinatorEndpoint.getServiceURI());
        unknownTransactionExceptionCompletionWithAckCoordinator.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, TestUtil.UNKNOWNTRANSACTIONEXCEPTION_TRANSACTION_IDENTIFIER);

        PortReference systemExceptionCompletionWithAckCoordinator = new PortReference(completionWithAckCoordinatorEndpoint.getServiceURI());
        systemExceptionCompletionWithAckCoordinator.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, TestUtil.SYSTEMEXCEPTION_TRANSACTION_IDENTIFIER);

        PortReference nonexistentCompletionWithAckCoordinator = new PortReference(completionWithAckCoordinatorEndpoint.getServiceURI());
        nonexistentCompletionWithAckCoordinator.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, TestUtil.NONEXISTENT_TRANSACTION_IDENTIFIER);

        _noExceptionCompletionWithAckStub                    = new CompletionWithAckStub(completionWithAckParticipantEndpoint, noExceptionCompletionWithAckCoordinator);
        _transactionRolledBackExceptionCompletionWithAckStub = new CompletionWithAckStub(completionWithAckParticipantEndpoint, transactionRolledBackExceptionCompletionWithAckCoordinator);
        _unknownExceptionExceptionCompletionWithAckStub      = new CompletionWithAckStub(completionWithAckParticipantEndpoint, unknownTransactionExceptionCompletionWithAckCoordinator);
        _systemExceptionCompletionWithAckStub                = new CompletionWithAckStub(completionWithAckParticipantEndpoint, systemExceptionCompletionWithAckCoordinator);
        _nonexistentCompletionWithAckStub                    = new CompletionWithAckStub(completionWithAckParticipantEndpoint, nonexistentCompletionWithAckCoordinator);
    }

    public void testCommitWithNoException()
        throws Exception
    {
        _noExceptionCompletionWithAckStub.commit();
    }

    public void testRollbackWithNoException()
        throws Exception
    {
        _noExceptionCompletionWithAckStub.rollback();
    }

    public void testNotifiedWithNoException()
        throws Exception
    {
        _noExceptionCompletionWithAckStub.notified();
    }

    public void testCommitWithTransactionRolledBackException()
        throws Exception
    {
        try
        {
            _transactionRolledBackExceptionCompletionWithAckStub.commit();
            fail("Expected exception \"TransactionRolledBackException\"");
        }
        catch (TransactionRolledBackException transactionRolledBackException)
        {
        }
    }

    public void testCommitWithUnknownTransactionException()
        throws Exception
    {
        try
        {
            _unknownExceptionExceptionCompletionWithAckStub.commit();
            fail("Expected exception \"UnknownTransactionException\"");
        }
        catch (UnknownTransactionException unknownTransactionException)
        {
        }
    }

    public void testRollbackWithUnknownTransactionException()
        throws Exception
    {
        try
        {
            _unknownExceptionExceptionCompletionWithAckStub.rollback();
            fail("Expected exception \"UnknownTransactionException\"");
        }
        catch (UnknownTransactionException unknownTransactionException)
        {
        }
    }

    public void testNotifiedWithUnknownTransactionException()
        throws Exception
    {
        _unknownExceptionExceptionCompletionWithAckStub.notified();
    }

    public void testCommitWithSystemException()
        throws Exception
    {
        try
        {
            _systemExceptionCompletionWithAckStub.commit();
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
            _systemExceptionCompletionWithAckStub.rollback();
            fail("Expected exception \"SystemException\"");
        }
        catch (SystemException systemException)
        {
        }
    }

    public void testNotifiedWithSystemException()
        throws Exception
    {
        _systemExceptionCompletionWithAckStub.notified();
    }

    public void testCommitWithNonExistent()
        throws Exception
    {
        try
        {
            _nonexistentCompletionWithAckStub.commit();
            fail("Expected exception \"UnknownTransactionException\"");
        }
        catch (UnknownTransactionException unknownTransactionException)
        {
        }
    }

    public void testRollbackWithNonExistent()
        throws Exception
    {
        try
        {
            _nonexistentCompletionWithAckStub.rollback();
            fail("Expected exception \"UnknownTransactionException\"");
        }
        catch (UnknownTransactionException unknownTransactionException)
        {
        }
    }

    public void testNotifiedWithNonExistent()
        throws Exception
    {
        _nonexistentCompletionWithAckStub.notified();
    }

    protected void tearDown()
        throws Exception
    {
        _noExceptionCompletionWithAckStub                    = null;
        _transactionRolledBackExceptionCompletionWithAckStub = null;
        _unknownExceptionExceptionCompletionWithAckStub      = null;
        _systemExceptionCompletionWithAckStub                = null;
        _nonexistentCompletionWithAckStub                    = null;
    }

    private CompletionWithAckCoordinatorParticipant _noExceptionCompletionWithAckStub                    = null;
    private CompletionWithAckCoordinatorParticipant _transactionRolledBackExceptionCompletionWithAckStub = null;
    private CompletionWithAckCoordinatorParticipant _unknownExceptionExceptionCompletionWithAckStub      = null;
    private CompletionWithAckCoordinatorParticipant _systemExceptionCompletionWithAckStub                = null;
    private CompletionWithAckCoordinatorParticipant _nonexistentCompletionWithAckStub                    = null;
}
