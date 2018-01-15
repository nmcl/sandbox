/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: TxContextImple.java,v 1.7 2003/04/02 10:38:30 nmcl Exp $
 */

package com.arjuna.mwlabs.wst.at.context;

import com.arjuna.wsc.messaging.CoordinationContext;

import com.arjuna.mwlabs.wst.at.ContextImple;

import com.arjuna.mw.wst.*;

import com.arjuna.mw.wsas.exceptions.SystemException;

/**
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: TxContextImple.java,v 1.7 2003/04/02 10:38:30 nmcl Exp $
 */

public class TxContextImple implements TxContext
{

    public TxContextImple (CoordinationContext ctx)
    {
	_context = new ContextImple(ctx);
    }

    public TxContextImple (com.arjuna.mw.wsc.context.Context context)
    {
	_context = context;
    }
    
    public boolean valid ()
    {
	if (_context != null)
	    return true;
	else
	    return false;
    }
    
    public boolean equals (Object obj)
    {
	if (obj instanceof TxContextImple)
	{
	    TxContextImple compare = (TxContextImple) obj;
	    
	    return compare.context().equals(_context);
	}
	else
	    return false;
    }
    
    public final String identifier ()
    {
	return _context.getCoordinationContext().identifier;
    }
    
    public final com.arjuna.mw.wsc.context.Context context ()
    {
	return _context;
    }

    public String toString ()
    {
	return _context.toString();
    }

    private com.arjuna.mw.wsc.context.Context _context;
    
}
