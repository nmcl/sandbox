/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: DuplicateSynchronizationException.java,v 1.1 2003/01/07 10:33:44 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.twophase.exceptions;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * There is no coordinator associated with the target.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: DuplicateSynchronizationException.java,v 1.1 2003/01/07 10:33:44 nmcl Exp $
 * @since 1.0.
 */

public class DuplicateSynchronizationException extends SystemException
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


