/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockStatus.java,v 1.5 1998/07/06 13:26:25 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

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
