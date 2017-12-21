/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: HeuristicCancelException.java,v 1.1 2003/01/07 10:33:44 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.twophase.exceptions;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * There is no coordinator associated with the target.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: HeuristicCancelException.java,v 1.1 2003/01/07 10:33:44 nmcl Exp $
 * @since 1.0.
 */

public class HeuristicCancelException extends SystemException
{

    public HeuristicCancelException ()
    {
	super();
    }

    public HeuristicCancelException (String s)
    {
	super(s);
    }

    public HeuristicCancelException (String s, int errorcode)
    {
	super(s, errorcode);
    }
    
}


