/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ConflictType.java,v 1.1 2000/02/25 14:03:46 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import java.io.PrintStream;

class ConflictType
{

public static final int CONFLICT = 0;
public static final int COMPATIBLE = 1;
public static final int PRESENT = 2;

public static void print (PrintStream strm, int c)
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
    
};
