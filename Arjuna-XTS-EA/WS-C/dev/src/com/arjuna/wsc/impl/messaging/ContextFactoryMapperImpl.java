/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ContextFactoryMapperImpl.java
 */

package com.arjuna.wsc.impl.messaging;

import com.arjuna.wsc.ContextFactoryMapper;
import com.arjuna.wsc.ContextFactory;

import java.util.Map;
import java.util.HashMap;
import javax.naming.InitialContext;

public class ContextFactoryMapperImpl implements ContextFactoryMapper, java.io.Serializable
{
    public ContextFactoryMapperImpl(String jndiName, String serviceURI)
        throws Exception
    {
        InitialContext initialContext = new InitialContext();

        initialContext.bind(jndiName, this);

        _serviceURI = serviceURI;
    }

    public String getServiceURI()
    {
        return _serviceURI;
    }

    public void addContextFactory(String coordinationTypeURI, ContextFactory contextFactory)
    {
        _mapping.put(coordinationTypeURI, contextFactory);

        contextFactory.install(_serviceURI, coordinationTypeURI);
    }

    public ContextFactory getContextFactory(String coordinationTypeURI)
    {
        ContextFactory contextFactory = (ContextFactory) _mapping.get(coordinationTypeURI);

        if ((contextFactory == null) && (_subordinateContextFactoryMapper != null))
            contextFactory = _subordinateContextFactoryMapper.getContextFactory(coordinationTypeURI);

        if (contextFactory == null)
            contextFactory = _defaultContextFactory;

        return contextFactory;
    }

    public void setSubordinateContextFactoryMapper(ContextFactoryMapper subordinateContextFactoryMapper)
    {
        _subordinateContextFactoryMapper = subordinateContextFactoryMapper;
    }

    public ContextFactoryMapper getSubordinateContextFactoryMapper()
    {
        return _subordinateContextFactoryMapper;
    }


    public void setDefaultContextFactory(ContextFactory defaultContextFactory)
    {
        _defaultContextFactory = defaultContextFactory;
    }

    public ContextFactory getDefaultContextFactory()
    {
        return _defaultContextFactory;
    }

    public void removeContextFactory(String coordinationTypeURI)
    {
        ContextFactory contextFactory = (ContextFactory) _mapping.remove(coordinationTypeURI);

        contextFactory.uninstall(_serviceURI, coordinationTypeURI);
    }

    private void writeObject(java.io.ObjectOutputStream objectOutputStream )
        throws java.io.IOException
    {
    }

    private void readObject(java.io.ObjectInputStream objectInputStream)
        throws java.io.IOException, ClassNotFoundException
    {
    }
 
    private String               _serviceURI                      = null;
    private ContextFactoryMapper _subordinateContextFactoryMapper = null;
    private ContextFactory       _defaultContextFactory           = null;
    private Map                  _mapping                         = new HashMap();
}
