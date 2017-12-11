/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ContextFactoryMapper.java
 */

package com.arjuna.wsc;

public interface ContextFactoryMapper
{
    public String getServiceURI();

    public void addContextFactory(String coordinationTypeURI, ContextFactory contextFactory);

    public ContextFactory getContextFactory(String coordinationTypeURI);

    public void setSubordinateContextFactoryMapper(ContextFactoryMapper subordinateContextFactoryMapper);

    public ContextFactoryMapper getSubordinateContextFactoryMapper();

    public void setDefaultContextFactory(ContextFactory defaultContextFactory);

    public ContextFactory getDefaultContextFactory();

    public void removeContextFactory(String coordinationTypeURI);
}
