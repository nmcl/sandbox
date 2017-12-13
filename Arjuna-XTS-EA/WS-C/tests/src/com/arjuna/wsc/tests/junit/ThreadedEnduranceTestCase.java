/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ThreadedEnduranceTestCase.java
 */

package com.arjuna.wsc.tests.junit;

import java.util.Random;
import javax.naming.InitialContext;
import junit.framework.*;
import com.arjuna.wsc.*;
import com.arjuna.wsc.impl.messaging.XMLUtil;
import com.arjuna.wsc.messaging.*;
import com.arjuna.wsc.tests.TestUtil;
import com.arjuna.wsc.tests.TestCoordinationContext;

public class ThreadedEnduranceTestCase extends TestCase
{
    private static final long TEST_DURATION = 60 * 1000;
    private static final int  TEST_THREADS  = 4;

    protected void setUp()
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        String activationRequesterJNDIName     = "activationrequester";
        String activationCoordinatorJNDIName   = "activationcoordinator-msg";
        String registrationRequesterJNDIName   = "registrationrequester";
        String registrationCoordinatorJNDIName = "registrationcoordinator-msg";

        _activationRequester               =(ActivationRequester) initialContext.lookup(activationRequesterJNDIName);
        _activationRequesterServiceURI     = _activationRequester.getServiceURI();
        _activationCoordinator             = (ActivationCoordinator) initialContext.lookup(activationCoordinatorJNDIName);
        _activationCoordinatorServiceURI   = _activationCoordinator.getServiceURI();
        _registrationRequester             = (RegistrationRequester) initialContext.lookup(registrationRequesterJNDIName);
        _registrationRequesterServiceURI   = _registrationRequester.getServiceURI();
        _registrationCoordinator           = (RegistrationCoordinator) initialContext.lookup(registrationCoordinatorJNDIName);
        _registrationCoordinatorServiceURI = _registrationCoordinator.getServiceURI();
    }

    public void testCreateCoordinationContextResponseWithoutDialogIdentifier()
        throws Exception
    {
        invokeWithoutDialogIdentifier(0);
    }

    public void testCreateCoordinationContextErrorWithoutDialogIdentifier()
        throws Exception
    {
        invokeWithoutDialogIdentifier(1);
    }

    public void testRegisterResponseWithoutDialogIdentifier()
        throws Exception
    {
        invokeWithoutDialogIdentifier(2);
    }

    public void testRegisterErrorWithoutDialogIdentifier()
        throws Exception
    {
        invokeWithoutDialogIdentifier(3);
    }

    public void testCreateCoordinationContextResponseWithDialogIdentifier()
        throws Exception
    {
        invokeWithDialogIdentifier(0);
    }

    public void testCreateCoordinationContextErrorWithDialogIdentifier()
        throws Exception
    {
        invokeWithDialogIdentifier(1);
    }

    public void testRegisterResponseWithDialogIdentifier()
        throws Exception
    {
        invokeWithDialogIdentifier(2);
    }

    public void testRegisterErrorWithDialogIdentifier()
        throws Exception
    {
        invokeWithDialogIdentifier(3);
    }

    public void invokeWithoutDialogIdentifier(int operation)
        throws Exception
    {
        InvokerThread[] threads = new InvokerThread[TEST_THREADS];

        for (int threadIndex = 0; threadIndex < TEST_THREADS; threadIndex++)
        {
            threads[threadIndex] = new InvokerThread(operation, null);
            threads[threadIndex].start();
        }

        for (int threadIndex = 0; threadIndex < TEST_THREADS; threadIndex++)
            threads[threadIndex].join();

        for (int threadIndex = 0; threadIndex < TEST_THREADS; threadIndex++)
            assertFalse(threads[threadIndex].isFailed());
    }

    public void invokeWithDialogIdentifier(int operation)
        throws Exception
    {
        InvokerThread[] threads = new InvokerThread[TEST_THREADS];

        for (int threadIndex = 0; threadIndex < TEST_THREADS; threadIndex++)
        {
            threads[threadIndex] = new InvokerThread(operation, Integer.toString(threadIndex));
            threads[threadIndex].start();
        }

        for (int threadIndex = 0; threadIndex < TEST_THREADS; threadIndex++)
            threads[threadIndex].join();

        for (int threadIndex = 0; threadIndex < TEST_THREADS; threadIndex++)
            assertFalse(threads[threadIndex].isFailed());
    }

    private class InvokerThread extends Thread
    {
        public InvokerThread(int operation, String dialogIdentifier)
        {
            _operation        = operation;
            _dialogIdentifier = dialogIdentifier;
        }

        public void run()
        {
            try
            {
                long startTime = System.currentTimeMillis();

                if (_operation == 0)
                {
                    while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
                        doCreateCoordinationContextResponse(_dialogIdentifier);
                }
                else if (_operation == 1)
                {
                    while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
                        doCreateCoordinationContextError(_dialogIdentifier);
                }
                else if (_operation == 2)
                {
                    while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
                        doRegisterResponse(_dialogIdentifier);
                }
                else
                {
                    while ((System.currentTimeMillis() - startTime) < TEST_DURATION)
                        doRegisterError(_dialogIdentifier);
                }
            }
            catch (Exception exception)
            {
                System.out.print("Exception: ");
                exception.printStackTrace(System.out);
                _failed = true;
            }
            catch (Error error)
            {
                System.out.print("Error: ");
                error.printStackTrace(System.out);
                _failed = true;
            }
        }

        public boolean isFailed()
        {
            return _failed;
        }

        private boolean _failed           = false;
        private int     _operation        = 0;
        private String  _dialogIdentifier = null;
    }

    public void doCreateCoordinationContextResponse(String dialogIdentifier)
        throws Exception
    {
        CreateCoordinationContextResponseMessage sentCreateCoordinationContextResponseMessage     = new CreateCoordinationContextResponseMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        TestCoordinationContext sentTestCoordinationContext                                  = new TestCoordinationContext();
        sentCreateCoordinationContextResponseMessage.requesterReference                      = new PortReference(_activationRequesterServiceURI);
        sentCreateCoordinationContextResponseMessage.coordinationContext                     = sentTestCoordinationContext;
        sentCreateCoordinationContextResponseMessage.coordinationContext.identifier          = TestUtil.PROTOCOL_IDENTIFIER;
        sentCreateCoordinationContextResponseMessage.coordinationContext.coordinationType    = TestUtil.COORDINATION_TYPE;
        sentCreateCoordinationContextResponseMessage.coordinationContext.registrationService = new PortReference(TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL);
        sentTestCoordinationContext.extensionValue                                           = TestUtil.TEST_EXTENSION_VALUE;
        sentCreateCoordinationContextResponseMessage.dialogIdentifier                        = dialogIdentifier;

        _activationCoordinator.sendCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage);

        Thread.sleep(_random.nextInt(6));

        receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentCreateCoordinationContextResponseMessage.dialogIdentifier);

        TestCoordinationContext receivedTestCoordinationContext = (TestCoordinationContext) receivedCreateCoordinationContextResponseMessage.coordinationContext;
        assertEquals(sentCreateCoordinationContextResponseMessage.requesterReference.getAddress(), receivedCreateCoordinationContextResponseMessage.requesterReference.getAddress());
        assertEquals(sentCreateCoordinationContextResponseMessage.coordinationContext.identifier, receivedCreateCoordinationContextResponseMessage.coordinationContext.identifier);
        assertEquals(sentCreateCoordinationContextResponseMessage.coordinationContext.coordinationType, receivedCreateCoordinationContextResponseMessage.coordinationContext.coordinationType);
        assertEquals(sentCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress(), receivedCreateCoordinationContextResponseMessage.coordinationContext.registrationService.getAddress());
        assertEquals(sentTestCoordinationContext.extensionValue, receivedTestCoordinationContext.extensionValue);
        assertEquals(sentCreateCoordinationContextResponseMessage.dialogIdentifier, receivedCreateCoordinationContextResponseMessage.dialogIdentifier);
    }

    public void doCreateCoordinationContextError(String dialogIdentifier)
        throws Exception
    {
        ErrorMessage                             sentErrorMessage                                 = new ErrorMessage();
        CreateCoordinationContextResponseMessage receivedCreateCoordinationContextResponseMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_activationRequesterServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJCOOR_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = dialogIdentifier;

        _activationCoordinator.sendError(sentErrorMessage);

        Thread.sleep(_random.nextInt(6));

        try
        {
            receivedCreateCoordinationContextResponseMessage = _activationRequester.recvCreateCoordinationContextResponse(sentErrorMessage.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage receivedErrorMessage = errorMessageException.getErrorMessage();

            assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
            assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
            assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
            assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
        }
    }

    public void doRegisterResponse(String dialogIdentifier)
        throws Exception
    {
        RegisterResponseMessage sentRegisterResponseMessage     = new RegisterResponseMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentRegisterResponseMessage.requesterReference          = new PortReference(_registrationRequesterServiceURI);
        sentRegisterResponseMessage.coordinationProtocolService = new PortReference(TestUtil.PROTOCOL_COORDINATOR_SERVICE);
        sentRegisterResponseMessage.dialogIdentifier            = dialogIdentifier;

        _registrationCoordinator.sendRegisterResponse(sentRegisterResponseMessage);

        Thread.sleep(_random.nextInt(6));

        receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentRegisterResponseMessage.dialogIdentifier);

        assertEquals(sentRegisterResponseMessage.requesterReference.getAddress(), receivedRegisterResponseMessage.requesterReference.getAddress());
        assertEquals(sentRegisterResponseMessage.coordinationProtocolService.getAddress(), receivedRegisterResponseMessage.coordinationProtocolService.getAddress());
        assertEquals(sentRegisterResponseMessage.dialogIdentifier, receivedRegisterResponseMessage.dialogIdentifier);
    }

    public void doRegisterError(String dialogIdentifier)
        throws Exception
    {
        ErrorMessage            sentErrorMessage                = new ErrorMessage();
        RegisterResponseMessage receivedRegisterResponseMessage = null;

        sentErrorMessage.targetProtocolService = new PortReference(_registrationRequesterServiceURI);
        sentErrorMessage.errorCode             = new ErrorCode();
        sentErrorMessage.errorCode.namespace   = XMLUtil.WSARJCOOR_NAMESPACE_URI;
        sentErrorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
        sentErrorMessage.dialogIdentifier      = dialogIdentifier;

        _registrationCoordinator.sendError(sentErrorMessage);

        Thread.sleep(_random.nextInt(6));

        try
        {
            receivedRegisterResponseMessage = _registrationRequester.recvRegisterResponse(sentErrorMessage.dialogIdentifier);

            fail("Expected exception: ErrorMessageException");
        }
        catch (ErrorMessageException errorMessageException)
        {
            ErrorMessage receivedErrorMessage = errorMessageException.getErrorMessage();

            assertEquals(sentErrorMessage.targetProtocolService.getAddress(), receivedErrorMessage.targetProtocolService.getAddress());
            assertEquals(sentErrorMessage.errorCode.namespace, receivedErrorMessage.errorCode.namespace);
            assertEquals(sentErrorMessage.errorCode.name, receivedErrorMessage.errorCode.name);
            assertEquals(sentErrorMessage.dialogIdentifier, receivedErrorMessage.dialogIdentifier);
        }
    }

    protected void tearDown()
        throws Exception
    {
    }

    private ActivationRequester     _activationRequester               = null;
    private String                  _activationRequesterServiceURI     = null;
    private ActivationCoordinator   _activationCoordinator             = null;
    private String                  _activationCoordinatorServiceURI   = null;
    private RegistrationRequester   _registrationRequester             = null;
    private String                  _registrationRequesterServiceURI   = null;
    private RegistrationCoordinator _registrationCoordinator           = null;
    private String                  _registrationCoordinatorServiceURI = null;

    private Random                  _random                            = new Random();
}
