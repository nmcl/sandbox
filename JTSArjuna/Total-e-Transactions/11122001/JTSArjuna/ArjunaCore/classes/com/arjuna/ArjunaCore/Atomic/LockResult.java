/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockResult.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:03 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import java.io.PrintWriter;

/**
 * The various results which can occur when setting a lock.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: LockResult.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:03 nmcl Exp $
 * @since JTS 1.0.
 */

public class LockResult
{
    
public static final int GRANTED = 0;
public static final int REFUSED = 1;
public static final int RELEASED = 2;

    /**
     * Print a human-readable form of the lock result.
     */

public static void print (PrintWriter strm, int l)
    {
	switch (l)
	{
	case GRANTED:
	    strm.print("LockResult.GRANTED");
	    break;
	case REFUSED:
	    strm.print("LockResult.REFUSED");
	    break;
	case RELEASED:
	    strm.print("LockResult.RELEASED");
	    break;
	default:
	    strm.print("Unknown");
	    break;
	}
    }
	
}
