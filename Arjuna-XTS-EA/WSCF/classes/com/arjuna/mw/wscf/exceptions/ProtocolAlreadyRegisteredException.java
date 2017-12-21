/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ProtocolAlreadyRegisteredException.java,v 1.1 2002/11/25 10:56:34 nmcl Exp $
 */

package com.arjuna.mw.wscf.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * An attempt has been made to register the same coordination protocol
 * with the protocol factory.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: ProtocolAlreadyRegisteredException.java,v 1.1 2002/11/25 10:56:34 nmcl Exp $
 * @since 1.0.
 */

public class ProtocolAlreadyRegisteredException extends WSASException
{

    public ProtocolAlreadyRegisteredException ()
    {
	super();
    }

    public ProtocolAlreadyRegisteredException (String s)
    {
	super(s);
    }

    public ProtocolAlreadyRegisteredException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


