/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DuplicateSynchronizationException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 */

package com.arjuna.mw.wstx.exceptions;

import com.arjuna.mw.wsas.exceptions.WSASException;

/**
 * Thrown if an enrol attempt is made for a synchronization with an identifier
 * that has already been used to enrol.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: DuplicateSynchronizationException.java,v 1.1 2002/11/25 11:00:52 nmcl Exp $
 * @since 1.0.
 */

public class DuplicateSynchronizationException extends WSASException
{
    
    public DuplicateSynchronizationException ()
    {
	super();
    }

    public DuplicateSynchronizationException (String s)
    {
	super(s);
    }

    public DuplicateSynchronizationException (String s, int errorcode)
    {
	super(s, errorcode);
    }
 
}

