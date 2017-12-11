/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ActivationDispatcher.java
 */

package com.arjuna.wsc.impl.dispatcher;

import javax.naming.InitialContext;
import com.arjuna.wsc.InvalidCreateParametersException;
import com.arjuna.wsc.ContextFactory;
import com.arjuna.wsc.ContextFactoryMapper;
import com.arjuna.wsc.messaging.CoordinationContext;
import com.arjuna.wsc.messaging.CreateCoordinationContextMessage;
import com.arjuna.wsc.messaging.CreateCoordinationContextResponseMessage;
import com.arjuna.wsc.messaging.ErrorCode;
import com.arjuna.wsc.messaging.ErrorMessage;
import com.arjuna.wsc.messaging.ActivationCoordinator;
import com.arjuna.wsc.impl.messaging.XMLUtil;

public class ActivationDispatcher
{
    public ActivationDispatcher(String activationCoordinationJNDIName, String contextFactoryMapperJNDIName)
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        _activationCoordinator = (ActivationCoordinator) initialContext.lookup(activationCoordinationJNDIName);
        _contextFactoryMapper  = (ContextFactoryMapper)  initialContext.lookup(contextFactoryMapperJNDIName);

        _dispatcher = new Dispatcher();
        _dispatcher.start();
    }

    public String getServiceURI()
    {
        return _activationCoordinator.getServiceURI();
    }

    private class Dispatcher extends Thread
    {
        public void run()
	{
            while (true)
	    {
                try
		{
                    CreateCoordinationContextMessage createCoordinationContextMessage = _activationCoordinator.recvCreateCoordinationContext();

                    ContextFactory contextFactory = _contextFactoryMapper.getContextFactory(createCoordinationContextMessage.coordinationType);

                    if (contextFactory != null)
	            {
                        try
                        {
                            CoordinationContext                      coordinationContext                      = contextFactory.create(createCoordinationContextMessage.coordinationType, createCoordinationContextMessage.expires, createCoordinationContextMessage.currentContext);
                            CreateCoordinationContextResponseMessage createCoordinationContextResponseMessage = new CreateCoordinationContextResponseMessage();

                            createCoordinationContextResponseMessage.requesterReference  = createCoordinationContextMessage.requesterReference;
                            createCoordinationContextResponseMessage.coordinationContext = coordinationContext;
                            createCoordinationContextResponseMessage.dialogIdentifier    = createCoordinationContextMessage.dialogIdentifier;

                            _activationCoordinator.sendCreateCoordinationContextResponse(createCoordinationContextResponseMessage);
                        }
                        catch (InvalidCreateParametersException invalidCreateParametersException)
                        {
                            ErrorMessage errorMessage = new ErrorMessage();

                            errorMessage.targetProtocolService = createCoordinationContextMessage.requesterReference;
                            errorMessage.errorCode             = new ErrorCode();
                            errorMessage.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
                            errorMessage.errorCode.name        = XMLUtil.INVALIDCREATEPARAMETERS_ERROR_CODE;
                            errorMessage.dialogIdentifier      = createCoordinationContextMessage.dialogIdentifier;

                            _activationCoordinator.sendError(errorMessage);
                        }
                        catch (Throwable throwable)
                        {
                            System.err.println("Unexpected exception thrown from create: \"" + throwable + "\"");

                            ErrorMessage errorMessage = new ErrorMessage();

                            errorMessage.targetProtocolService = createCoordinationContextMessage.requesterReference;
                            errorMessage.errorCode             = new ErrorCode();
                            errorMessage.errorCode.namespace   = XMLUtil.WSARJCOOR_NAMESPACE_URI;
                            errorMessage.errorCode.name        = XMLUtil.UNKNOWNERROR_ERROR_CODE;
                            errorMessage.dialogIdentifier      = createCoordinationContextMessage.dialogIdentifier;

                            _activationCoordinator.sendError(errorMessage);
                        }
		    }
                    else
	            {
                        System.err.println("CreateCoordinationContext called for unknown coordination type: \"" + createCoordinationContextMessage.coordinationType + "\"");

                        ErrorMessage errorMessage = new ErrorMessage();

                        errorMessage.targetProtocolService = createCoordinationContextMessage.requesterReference;
                        errorMessage.errorCode             = new ErrorCode();
                        errorMessage.errorCode.namespace   = XMLUtil.WSCOOR_NAMESPACE_URI;
                        errorMessage.errorCode.name        = XMLUtil.INVALIDCREATEPARAMETERS_ERROR_CODE;
                        errorMessage.dialogIdentifier      = createCoordinationContextMessage.dialogIdentifier;

                        _activationCoordinator.sendError(errorMessage);
	            }
		}
		catch (Throwable throwable)
	        {
                    throwable.printStackTrace(System.err);
		}
	    }
	}
    }

    private ActivationCoordinator _activationCoordinator = null;
    private ContextFactoryMapper  _contextFactoryMapper  = null;
    private Dispatcher            _dispatcher            = null;
}
