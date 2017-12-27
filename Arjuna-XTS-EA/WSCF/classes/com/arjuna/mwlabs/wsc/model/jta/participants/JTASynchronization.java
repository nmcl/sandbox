/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTASynchronization.java,v 1.2 2003/02/24 13:21:33 nmcl Exp $
 */

package com.arjuna.mwlabs.wsc.model.jta.participants;

import com.arjuna.wsc.messaging.PortReference;

import javax.transaction.xa.*;

public class JTASynchronization implements javax.transaction.Synchronization
{

    public JTASynchronization (PortReference address)
    {
	_address = address;
    }
    
    public void beforeCompletion ()
    {
	System.out.println("beforeCompletion");
    }

    public void afterCompletion (int status)
    {
	System.out.println("afterCompletion :"+status);
    }

    private PortReference _address;
    
}
