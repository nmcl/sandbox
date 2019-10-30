/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockMode.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:02 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import java.io.PrintWriter;

/**
 * The types of lock which can exist.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: LockMode.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:02 nmcl Exp $
 * @since JTS 1.0.
 */

public class LockMode
{

public static final int READ = 0;
public static final int WRITE = 1;
public static final int UPGRADE = 2;
public static final int INTENTION_READ = 3;
public static final int INTENTION_WRITE = 4;

    /**
     * Print a human-readable form of the lock type.
     */

public static String printString (int l)
    {
	switch (l)
	{
	case LockMode.READ:
	    return "LockMode.READ";
	case LockMode.WRITE:
	    return "LockMode.WRITE";
	case LockMode.UPGRADE:
	    return "LockMode.UPGRADE";
	case LockMode.INTENTION_READ:
	    return "LockMode.INTENTION_READ";
	case LockMode.INTENTION_WRITE:
	    return "LockMode.INTENTION_WRITE";
	default:
	    return "Unknown";
	}
    }

public static void print (PrintWriter strm, int l)
    {
	strm.print(LockMode.printString(l));
    }
    
}
