/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UnknownProtocolImplementationException.java,v 1.1 2002/11/25 10:56:34 nmcl Exp $
 */

package com.arjuna.mw.wscf.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * An attempt was made to operate on a protocol implementation that the
 * factory did not know about.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: UnknownProtocolImplementationException.java,v 1.1 2002/11/25 10:56:34 nmcl Exp $
 * @since 1.0.
 */

public class UnknownProtocolImplementationException extends WSASException
{

    public UnknownProtocolImplementationException ()
    {
	super();
    }

    public UnknownProtocolImplementationException (String s)
    {
	super(s);
    }

    public UnknownProtocolImplementationException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


