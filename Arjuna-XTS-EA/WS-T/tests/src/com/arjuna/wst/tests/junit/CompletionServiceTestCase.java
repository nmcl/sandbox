/*
 * Copyright (c) 2003, Arjuna Technologies Limited.
 *
 * CompletionServiceTestCase.java
 */

package com.arjuna.wst.tests.junit;

import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wst.SystemException;
import com.arjuna.wst.TransactionRolledBackException;
import com.arjuna.wst.UnknownTransactionException;
import com.arjuna.wst.CompletionCoordinatorParticipant;
import com.arjuna.wst.messaging.*;
import com.arjuna.wst.stub.CompletionStub;
import com.arjuna.wst.impl.messaging.XMLUtil;
import com.arjuna.wst.tests.TestUtil;

public class CompletionServiceTestCase extends TestCase
{
    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String completionCoordinatorJNDIName = "completioncoordinator-srv";
        String completionParticipantJNDIName = "completionparticipant";

        CompletionCoordinator completionCoordinatorEndpoint = (CompletionCoordinator) initialContext.lookup(completionCoordinatorJNDIName);
        CompletionParticipant completionParticipantEndpoint = (CompletionParticipant) initialContext.lookup(completionParticipantJNDIName);

        PortReference noExceptionCompletionCoordinator = new PortReference(completionCoordinatorEndpoint.getServiceURI());
        noExceptionCompletionCoordinator.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, TestUtil.NOEXCEPTION_TRANSACTION_IDENTIFIER);

        PortReference transactionRolledBackExceptionCompletionCoordinator = new PortReference(completionCoordinatorEndpoint.getServiceURI());
        transactionRolledBackExceptionCompletionCoordinator.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, TestUtil.TRANSACTIONROLLEDBACKEXCEPTION_TRANSACTION_IDENTIFIER);

        PortReference unknownTransactionExceptionCompletionCoordinator = new PortReference(completionCoordinatorEndpoint.getServiceURI());
        unknownTransactionExceptionCompletionCoordinator.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, TestUtil.UNKNOWNTRANSACTIONEXCEPTION_TRANSACTION_IDENTIFIER);

        PortReference systemExceptionCompletionCoordinator = new PortReference(completionCoordinatorEndpoint.getServiceURI());
        systemExceptionCompletionCoordinator.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, TestUtil.SYSTEMEXCEPTION_TRANSACTION_IDENTIFIER);

        PortReference nonexistentCompletionCoordinator = new PortReference(completionCoordinatorEndpoint.getServiceURI());
        nonexistentCompletionCoordinator.addExtension(XMLUtil.TRANSACTION_IDENTIFIER_TAG, TestUtil.NONEXISTENT_TRANSACTION_IDENTIFIER);

        _noExceptionCompletionStub                    = new CompletionStub(completionParticipantEndpoint, noExceptionCompletionCoordinator);
        _transactionRolledBackExceptionCompletionStub = new CompletionStub(completionParticipantEndpoint, transactionRolledBackExceptionCompletionCoordinator);
        _unknownExceptionExceptionCompletionStub      = new CompletionStub(completionParticipantEndpoint, unknownTransactionExceptionCompletionCoordinator);
        _systemExceptionCompletionStub                = new CompletionStub(completionParticipantEndpoint, systemExceptionCompletionCoordinator);
        _nonexistentCompletionStub                    = new CompletionStub(completionParticipantEndpoint, nonexistentCompletionCoordinator);
    }

    public void testCommitWithNoException()
        throws Exception
    {
        _noExceptionCompletionStub.commit();
    }

    public void testRollbackWithNoException()
        throws Exception
    {
        _noExceptionCompletionStub.rollback();
    }

    public void testCommitWithTransactionRolledBackException()
        throws Exception
    {
        try
        {
            _transactionRolledBackExceptionCompletionStub.commit();
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
            _unknownExceptionExceptionCompletionStub.commit();
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
            _unknownExceptionExceptionCompletionStub.rollback();
            fail("Expected exception \"UnknownTransactionException\"");
        }
        catch (UnknownTransactionException unknownTransactionException)
        {
        }
    }

    public void testCommitWithSystemException()
        throws Exception
    {
        try
        {
            _systemExceptionCompletionStub.commit();
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
            _systemExceptionCompletionStub.rollback();
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
            _nonexistentCompletionStub.commit();
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
            _nonexistentCompletionStub.rollback();
            fail("Expected exception \"UnknownTransactionException\"");
        }
        catch (UnknownTransactionException unknownTransactionException)
        {
        }
    }

    protected void tearDown()
        throws Exception
    {
        _noExceptionCompletionStub                    = null;
        _transactionRolledBackExceptionCompletionStub = null;
        _unknownExceptionExceptionCompletionStub      = null;
        _systemExceptionCompletionStub                = null;
        _nonexistentCompletionStub                    = null;
    }

    private CompletionCoordinatorParticipant _noExceptionCompletionStub                    = null;
    private CompletionCoordinatorParticipant _transactionRolledBackExceptionCompletionStub = null;
    private CompletionCoordinatorParticipant _unknownExceptionExceptionCompletionStub      = null;
    private CompletionCoordinatorParticipant _systemExceptionCompletionStub                = null;
    private CompletionCoordinatorParticipant _nonexistentCompletionStub                    = null;
}
