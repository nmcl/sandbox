/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CoordinationResult.java,v 1.1 2003/01/07 10:33:44 nmcl Exp $
 */

package com.arjuna.mw.wscf.model.twophase.common;

/**
 * The final coordination result.
 *
 * @author Mark Little (mark.little@arjuna.com)
 * @version $Id: CoordinationResult.java,v 1.1 2003/01/07 10:33:44 nmcl Exp $
 * @since 1.0.
 */

public class CoordinationResult
{

    public static final int CONFIRMED = 0;
    public static final int CANCELLED = 1;
    
    /**
     * @return a human-readable version of the outcome.
     */

    public static String stringForm (int res)
    {
	switch (res)
	{
	case CANCELLED:
	    return "CoordinationResult.CANCELLED";
	case CONFIRMED:
	    return "CoordinationResult.CONFIRMED";
	default:
	    return "Unknown - "+res;
	}
    }
	
}
