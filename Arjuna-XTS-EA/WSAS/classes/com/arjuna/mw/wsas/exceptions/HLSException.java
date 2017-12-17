/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: HLSException.java,v 1.1 2002/11/25 10:51:43 nmcl Exp $
 */

package com.arjuna.mw.wsas.exceptions;

/**
 * Thrown the HLS completion fails. Allows the actual exception to be
 * passed as an outcome.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: HLSException.java,v 1.1 2002/11/25 10:51:43 nmcl Exp $
 * @since 1.0.
 */

public class HLSException extends SystemException
{

    public HLSException ()
    {
	super();
    }

    public HLSException (String reason)
    {
	super(reason);
    }

    public HLSException (String reason, int errorcode)
    {
	super(reason, errorcode);
    }

    public HLSException (String reason, SystemException obj)
    {
	super(reason, obj);
    }
    
    public HLSException (SystemException ex)
    {
	super(ex);
    }
    
}


