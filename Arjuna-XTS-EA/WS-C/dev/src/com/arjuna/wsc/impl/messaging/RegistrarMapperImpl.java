/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrarMapperImpl.java
 */

package com.arjuna.wsc.impl.messaging;

import com.arjuna.wsc.RegistrarMapper;
import com.arjuna.wsc.Registrar;

import java.util.Map;
import java.util.HashMap;
import javax.naming.InitialContext;

public class RegistrarMapperImpl implements RegistrarMapper, java.io.Serializable
{
    public RegistrarMapperImpl(String jndiName, String serviceURI)
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

    public void addRegistrar(String protocolIdentifier, Registrar registrar)
    {
        _mapping.put(protocolIdentifier, registrar);

        registrar.install(_serviceURI, protocolIdentifier);
    }

    public void removeRegistrar(String protocolIdentifier)
    {
        Registrar registrar = (Registrar) _mapping.remove(protocolIdentifier);

        registrar.uninstall(_serviceURI, protocolIdentifier);
    }

    public Registrar getRegistrar(String protocolIdentifier)
    {
        return (Registrar) _mapping.get(protocolIdentifier);
    }

    private void writeObject(java.io.ObjectOutputStream objectOutputStream )
        throws java.io.IOException
    {
    }

    private void readObject(java.io.ObjectInputStream objectInputStream)
        throws java.io.IOException, ClassNotFoundException
    {
    }

    private String _serviceURI = null;
    private Map    _mapping    = new HashMap();
}
