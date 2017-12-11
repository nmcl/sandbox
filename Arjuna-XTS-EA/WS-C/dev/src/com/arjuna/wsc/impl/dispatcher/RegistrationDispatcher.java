/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrationDispatcher.java
 */

package com.arjuna.wsc.impl.dispatcher;

import javax.naming.InitialContext;
import com.arjuna.wsc.*;
import com.arjuna.wsc.messaging.PortReference;
import com.arjuna.wsc.messaging.ErrorCode;
import com.arjuna.wsc.messaging.RegisterMessage;
import com.arjuna.wsc.messaging.RegisterResponseMessage;
import com.arjuna.wsc.messaging.ErrorMessage;
import com.arjuna.wsc.messaging.RegistrationCoordinator;
import com.arjuna.wsc.impl.messaging.XMLUtil;

public class RegistrationDispatcher
{
    public RegistrationDispatcher(String registrationCoordinationJNDIName, String registrarMapperJNDIName)
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        _registrationCoordinator = (RegistrationCoordinator) initialContext.lookup(registrationCoordinationJNDIName);
        _registrarMapper         = (RegistrarMapper)         initialContext.lookup(registrarMapperJNDIName);

        _dispatcher = new Dispatcher();
        _dispatcher.start();
    }

    public String getServiceURI()
    {
        return _registrationCoordinator.getServiceURI();
    }

    private class Dispatcher extends Thread
    {
        public void run()
	{
            while (true)
	    {
                try
		{
                    RegisterMessage registerMessage = _registrationCoordinator.recvRegister();

                    Registrar       registrar       = _registrarMapper.getRegistrar(registerMessage.protocolIdentifier);

                    if (registrar != null)
	            {
                        try
                        {
                            PortReference           coordinationProtocolService = registrar.register(registerMessage.participantProtocolService, registerMessage.protocolIdentifier, registerMessage.registrationService.getExtensionValue(XMLUtil.INSTANCE_IDENTIFIER_TAG));
                            RegisterResponseMessage registerResponseMessage     = new RegisterResponseMessage();

                            registerResponseMessage.requesterReference          = registerMessage.requesterReference;
                            registerResponseMessage.coordinationProtocolService = coordinationProtocolService;
                            registerResponseMessage.dialogIdentifier            = registerMessage.dialogIdentifier;

                            _registrationCoordinator.sendRegisterResponse(registerResponseMessage);
                        }
                        catch (AlreadyRegisteredException alreadyRegisteredException)
                        {
                            ErrorMessage errorMessage = new ErrorMessage();

                            errorMessage.targetProtocolService = registerMessage.requesterReference;
                            errorMessage.errorCode             = new ErrorCode();
                            errorMessage.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
                            errorMessage.errorCode.name        = XMLUtil.ALREADYREGISTERED_ERROR_CODE;
                            errorMessage.dialogIdentifier      = registerMessage.dialogIdentifier;

                            _registrationCoordinator.sendError(errorMessage);
                        }
                        catch (InvalidProtocolException invalidProtocolException)
                        {
                            ErrorMessage errorMessage = new ErrorMessage();

                            errorMessage.targetProtocolService = registerMessage.requesterReference;
                            errorMessage.errorCode             = new ErrorCode();
                            errorMessage.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
                            errorMessage.errorCode.name        = XMLUtil.INVALIDPROTOCOL_ERROR_CODE;
                            errorMessage.dialogIdentifier      = registerMessage.dialogIdentifier;

                            _registrationCoordinator.sendError(errorMessage);
                        }
                        catch (InvalidStateException InvalidStateException)
                        {
                            ErrorMessage errorMessage = new ErrorMessage();

                            errorMessage.targetProtocolService = registerMessage.requesterReference;
                            errorMessage.errorCode             = new ErrorCode();
                            errorMessage.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
                            errorMessage.errorCode.name        = XMLUtil.INVALIDSTATE_ERROR_CODE;
                            errorMessage.dialogIdentifier      = registerMessage.dialogIdentifier;

                            _registrationCoordinator.sendError(errorMessage);
                        }
                        catch (NoActivityException noActivityException)
                        {
                            ErrorMessage errorMessage = new ErrorMessage();

                            errorMessage.targetProtocolService = registerMessage.requesterReference;
                            errorMessage.errorCode             = new ErrorCode();
                            errorMessage.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
                            errorMessage.errorCode.name        = XMLUtil.NOACTIVITY_ERROR_CODE;
                            errorMessage.dialogIdentifier      = registerMessage.dialogIdentifier;

                            _registrationCoordinator.sendError(errorMessage);
                        }
                        catch (Throwable throwable)
                        {
                            System.err.println("Unexpected exception thrown from register: \"" + throwable + "\"");

                            ErrorMessage errorMessage = new ErrorMessage();

                            errorMessage.targetProtocolService = registerMessage.requesterReference;
                            errorMessage.errorCode             = new ErrorCode();
                            errorMessage.errorCode.namespace   = XMLUtil.WSARJCOOR_NAMESPACE_URI;
                            errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                            errorMessage.dialogIdentifier      = registerMessage.dialogIdentifier;

                            _registrationCoordinator.sendError(errorMessage);
                        }
		    }
                    else
	            {
                        System.err.println("Register called for unknown protocol identifier: \"" + registerMessage.protocolIdentifier + "\"");

                        ErrorMessage errorMessage = new ErrorMessage();

                        errorMessage.targetProtocolService = registerMessage.requesterReference;
                        errorMessage.errorCode             = new ErrorCode();
                        errorMessage.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
                        errorMessage.errorCode.name        = XMLUtil.INVALIDPROTOCOL_ERROR_CODE;
                        errorMessage.dialogIdentifier      = registerMessage.dialogIdentifier;

                        _registrationCoordinator.sendError(errorMessage);
	            }
		}
		catch (Throwable throwable)
	        {
                    throwable.printStackTrace(System.err);
		}
	    }
	}
    }

    private RegistrationCoordinator _registrationCoordinator = null;
    private RegistrarMapper         _registrarMapper         = null;
    private Dispatcher              _dispatcher              = null;
}
