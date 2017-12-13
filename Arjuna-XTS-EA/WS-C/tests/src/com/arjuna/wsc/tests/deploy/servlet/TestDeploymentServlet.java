/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * TestDeploymentServlet.java
 */

package com.arjuna.wsc.tests.deploy.servlet;

import javax.naming.InitialContext;
import javax.servlet.ServletException;
import javax.servlet.ServletConfig;
import javax.servlet.http.HttpServlet;
import com.arjuna.wsc.ContextFactoryMapper;
import com.arjuna.wsc.RegistrarMapper;
import com.arjuna.wsc.messaging.saaj.CoordinationContextHelper;
import com.arjuna.wsc.tests.*;

public class TestDeploymentServlet extends HttpServlet
{
    public void init(ServletConfig config)
        throws ServletException
    {
        super.init(config);

        try
        {
            InitialContext initialContext = new InitialContext();

            String contextFactoryMapperJNDIName = config.getInitParameter("ContextFactoryMapperJNDIName");
            String registrarMapperJNDIName      = config.getInitParameter("RegistrarMapperJNDIName");

            TestUtil.ACTIVATION_REQUESTER_URL             = config.getInitParameter("ActivationRequesterURI");
            TestUtil.ACTIVATION_COORDINATOR_SERVICE_URL   = config.getInitParameter("ActivationCoordinatorServiceURI");
            TestUtil.REGISTRATION_REQUESTER_URL           = config.getInitParameter("RegistrationRequesterURI");
            TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL = config.getInitParameter("RegistrationCoordinatorServiceURI");

            _contextFactoryMapper = (ContextFactoryMapper) initialContext.lookup(contextFactoryMapperJNDIName);
            _registrarMapper      = (RegistrarMapper)      initialContext.lookup(registrarMapperJNDIName);

            TestContextFactory testContextFactory = new TestContextFactory(TestUtil.COORDINATION_TYPE, TestUtil.REGISTRATION_COORDINATOR_SERVICE_URL);
            TestRegistrar      testRegistrar      = new TestRegistrar();

            _contextFactoryMapper.addContextFactory(TestUtil.COORDINATION_TYPE, testContextFactory);
            _contextFactoryMapper.addContextFactory(TestUtil.INVALID_CREATE_PARAMETERS_COORDINATION_TYPE, testContextFactory);
            _registrarMapper.addRegistrar(TestUtil.PROTOCOL_IDENTIFIER, testRegistrar);
            _registrarMapper.addRegistrar(TestUtil.ALREADY_REGISTERED_PROTOCOL_IDENTIFIER, testRegistrar);
            _registrarMapper.addRegistrar(TestUtil.INVALID_PROTOCOL_PROTOCOL_IDENTIFIER, testRegistrar);
            _registrarMapper.addRegistrar(TestUtil.INVALID_STATE_PROTOCOL_IDENTIFIER, testRegistrar);
            _registrarMapper.addRegistrar(TestUtil.NO_ACTIVITY_PROTOCOL_IDENTIFIER, testRegistrar);

            CoordinationContextHelper.addCoordinationContextExtensionHelper(TestUtil.COORDINATION_TYPE, new TestCoordinationContextExtensionHelper());

            log("Test Deployment: done, context factory mapper is \"" + contextFactoryMapperJNDIName + "\", registrar mapper is \"" + registrarMapperJNDIName + "\"");
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

        _contextFactoryMapper.removeContextFactory(TestUtil.COORDINATION_TYPE);
        _registrarMapper.removeRegistrar(TestUtil.COORDINATION_TYPE);

        super.destroy();
    }

    private ContextFactoryMapper _contextFactoryMapper = null;
    private RegistrarMapper      _registrarMapper      = null;
}
