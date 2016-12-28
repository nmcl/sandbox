/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockStatus.java,v 1.1 2000/02/25 14:03:51 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import java.io.PrintStream;

public class LockStatus
{

public static final int LOCKFREE = 0;
public static final int LOCKHELD = 1;
public static final int LOCKRETAINED = 2;

public static void print (PrintStream strm, int ls)
    {
	strm.print((ls == LOCKFREE) ? "LOCKFREE"
		                    : (ls == LOCKHELD ? "LOCKHELD"
				       : "LOCKRETAINED"));
    }
    
};
