/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ConflictType.java,v 1.1.4.1.2.1 2000/10/30 10:26:20 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

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

    /**
     * Print a human-readable form of the conflict type.
     */

public static void print (PrintWriter strm, int c)
    {
	switch (c)
	{
	case CONFLICT:
	    strm.print("ConflictType.CONFLICT");
	    break;
	case COMPATIBLE:
	    strm.print("ConflictType.COMPATIBLE");
	    break;
	case PRESENT:
	    strm.print("ConflictType.PRESENT");
	    break;
	}
    }
    
}
