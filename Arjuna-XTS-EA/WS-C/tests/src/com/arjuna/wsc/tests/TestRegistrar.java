/*
 * Copyright (c) 2002, 2003, Arjuna Technologies Limited.
 *
 * TestRegistrar.java
 */

package com.arjuna.wsc.tests;

import com.arjuna.wsc.AlreadyRegisteredException;
import com.arjuna.wsc.InvalidProtocolException;
import com.arjuna.wsc.InvalidStateException;
import com.arjuna.wsc.NoActivityException;
import com.arjuna.wsc.Registrar;
import com.arjuna.wsc.messaging.PortReference;

public class TestRegistrar implements Registrar
{
    public void install(String serviceAddress, String protocolIdentifier)
    {
    }

    public PortReference register(PortReference participantProtocolService, String protocolIdentifier, String instanceIdentifier)
        throws AlreadyRegisteredException, InvalidProtocolException, InvalidStateException, NoActivityException
    {
        if (protocolIdentifier.equals(TestUtil.ALREADY_REGISTERED_PROTOCOL_IDENTIFIER))
            throw new AlreadyRegisteredException();
        else if (protocolIdentifier.equals(TestUtil.INVALID_PROTOCOL_PROTOCOL_IDENTIFIER))
            throw new InvalidProtocolException();
        else if (protocolIdentifier.equals(TestUtil.INVALID_STATE_PROTOCOL_IDENTIFIER))
            throw new InvalidStateException();
        else if (protocolIdentifier.equals(TestUtil.NO_ACTIVITY_PROTOCOL_IDENTIFIER))
            throw new NoActivityException();

        return new PortReference(TestUtil.PROTOCOL_COORDINATOR_SERVICE);
    }

    public void uninstall(String serviceAddress, String protocolIdentifier)
    {
    }
}
