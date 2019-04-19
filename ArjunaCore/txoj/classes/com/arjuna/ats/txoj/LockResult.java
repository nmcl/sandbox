/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockResult.java,v 1.1 2003/06/19 11:56:11 nmcl Exp $
 */

package com.arjuna.ats.txoj;

import java.io.PrintWriter;

/**
 * The various results which can occur when setting a lock.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: LockResult.java,v 1.1 2003/06/19 11:56:11 nmcl Exp $
 * @since JTS 1.0.
 */

public class LockResult
{
    
    public static final int GRANTED = 0;
    public static final int REFUSED = 1;
    public static final int RELEASED = 2;

    public static String stringForm (int l)
    {
	switch (l)
	{
	case GRANTED:
	    return "LockResult.GRANTED";
	case REFUSED:
	    return "LockResult.REFUSED";
	case RELEASED:
	    return "LockResult.RELEASED";
	default:
	    return "Unknown";
	}
    }
    
    /**
     * Print a human-readable form of the lock result.
     */

    public static void print (PrintWriter strm, int l)
    {
	strm.print(stringForm(l));
    }
	
}
