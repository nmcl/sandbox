/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ConflictType.java,v 1.1 2003/06/19 11:56:10 nmcl Exp $
 */

package com.arjuna.ats.txoj;

import java.io.PrintWriter;

/**
 * The various types of lock conflict that can occur when
 * trying to set a lock.
 */

class ConflictType
{

    public static final int CONFLICT = 0;
    public static final int COMPATIBLE = 1;
    public static final int PRESENT = 2;

    public static String stringForm (int c)
    {
	switch (c)
	{
	case CONFLICT:
	    return "ConflictType.CONFLICT";
	case COMPATIBLE:
	    return "ConflictType.COMPATIBLE";
	case PRESENT:
	    return "ConflictType.PRESENT";
	default:
	    return "Unknown";
	}
    }
    
    /**
     * Print a human-readable form of the conflict type.
     */

    public static void print (PrintWriter strm, int c)
    {
	strm.print(c);
    }
    
}
