/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * Registrar.java
 */

package com.arjuna.wsc;

import com.arjuna.wsc.messaging.PortReference;

public interface Registrar
{
    /**
     * Called when a registrar is added to a register mapper. This method will be called multiple times if the
     * registrar is added to multiple register mappers or to the same register mapper with different protocol
     * identifiers.
     *
     * @param serviceAddress the address of the service
     * @param protocolIdentifier the protocol identifier
     */
    public void install(String serviceAddress, String protocolIdentifier);

    /**
     * Registers the interest of participant in a particular protocol.
     *
     * @param participantProtocolServiceAddress the port reference of the participant protocol service
     * @param protocolIdentifier the protocol identifier
     * @param instanceIdentifier the instance identifier, this may be null
     *
     * @return the port reference of the coordinator protocol service
     *
     * @throws AlreadyRegisteredException if the participant is already registered for this coordination protocol under
     *         this activity identifier
     * @throws InvalidProtocolException if the coordination protocol is not supported
     * @throws InvalidStateException if the state of the coordinator no longer allows registration for this
     *         coordination protocol
     * @throws NoActivityException if the actvity does not exist
     */
    public PortReference register(PortReference participantProtocolService, String protocolIdentifier, String instanceIdentifier)
        throws AlreadyRegisteredException, InvalidProtocolException, InvalidStateException, NoActivityException;

    /**
     * Called when a registrar is removed from a register mapper. This method will be called multiple times if the
     * registrar is removed from multiple register mappers or from the same register mapper with different protocol
     * identifiers.
     *
     * @param serviceAddress the address of the service
     * @param protocolIdentifier the protocol identifier
     */
    public void uninstall(String serviceAddress, String protocolIdentifier);
}
