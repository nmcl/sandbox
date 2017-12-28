/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ProtocolLocationException.java,v 1.1 2002/11/25 10:56:36 nmcl Exp $
 */

package com.arjuna.mwlabs.wscf.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * When a coordination protocol implementation is imported to the registry,
 * the XML protocol document must be loaded that corresponds to the protocol
 * it implements. If that document cannot be located then this exception
 * will be thrown.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ProtocolLocationException.java,v 1.1 2002/11/25 10:56:36 nmcl Exp $
 * @since 1.0.
 */

public class ProtocolLocationException extends WSASException
{

    public ProtocolLocationException ()
    {
	super();
    }

    public ProtocolLocationException (String s)
    {
	super(s);
    }

    public ProtocolLocationException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


