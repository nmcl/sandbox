/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockStatus.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:03 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import java.io.PrintWriter;

/**
 * Essentially an enumeration of the status a lock may
 * be in.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: LockStatus.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:03 nmcl Exp $
 * @since JTS 1.0.
 */

public class LockStatus
{

public static final int LOCKFREE = 0;
public static final int LOCKHELD = 1;
public static final int LOCKRETAINED = 2;

    /**
     * Print a human-readable form.
     */

public static String printString (int ls)
    {
	switch (ls)
	{
	case LOCKFREE:
	    return "LockStatus.LOCKFREE";
	case LOCKHELD:
	    return "LockStatus.LOCKHELD";
	case LOCKRETAINED:
	    return "LockStatus.LOCKRETAINED";
	default:
	    return "Unknown";
	}
    }

public static void print (PrintWriter strm, int ls)
    {
	strm.print(LockStatus.printString(ls));
    }
    
};
