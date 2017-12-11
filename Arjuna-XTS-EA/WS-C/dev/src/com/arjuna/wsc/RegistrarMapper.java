/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * RegistrarMapper.java
 */

package com.arjuna.wsc;

public interface RegistrarMapper
{
    public String getServiceURI();

    public void addRegistrar(String protocolIdentifier, Registrar registrar);

    public void removeRegistrar(String protocolIdentifier);

    public Registrar getRegistrar(String protocolIdentifier);
}
