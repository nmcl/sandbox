/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * ContextFactory.java
 */

package com.arjuna.wsc;

import com.arjuna.wsc.messaging.CoordinationContext;
import com.arjuna.wsc.messaging.AttributedDateTime;

public interface ContextFactory
{
    /**
     * Called when a context factory is added to a context factory mapper. This method will be called multiple times
     * if the context factory is added to multiple context factory mappers or to the same context mapper with different
     * protocol identifiers.
     *
     * @param serviceAddress the address of the service
     * @param coordinationTypeURI the coordination type uri
     */
    public void install(String serviceAddress, String coordinationTypeURI);

    /**
     * Creates a coordination context.
     *
     * @param coordinationTypeURI the coordination type uri
     * @param expires the expire date/time for the returned context, can be null
     * @param currentContext the current coordination context, can be null
     *
     * @return the created coordination context
     *
     * @throws InvalidCreateParametersException if a parameter passed is invalid
     *         this activity identifier
     */
    public CoordinationContext create(String coordinationTypeURI, AttributedDateTime expires, CoordinationContext currentContext)
        throws InvalidCreateParametersException;

    /**
     * Called when a context factory is removed from a context factory mapper. This method will be called multiple
     * times if the context factory is removed from multiple context factory mappers or from the same context factory
     * mapper with different coordination type uris.
     *
     * @param serviceAddress the address of the service
     * @param coordinationTypeURI the coordination type uri
     */
    public void uninstall(String serviceAddress, String coordinationTypeURI);
}
