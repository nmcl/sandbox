/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockMode.java,v 1.1 2000/02/25 14:03:50 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import java.io.PrintStream;

public class LockMode
{

public static final int READ = 0;
public static final int WRITE = 1;
public static final int UPGRADE = 2;
public static final int INTENTION_READ = 3;
public static final int INTENTION_WRITE = 4;

public static void print (PrintStream strm, int l)
    {
	switch (l)
	{
	case LockMode.READ:
	    strm.print("LockMode.READ");
	    break;
	case LockMode.WRITE:
	    strm.print("LockMode.WRITE");
	    break;
	case LockMode.UPGRADE:
	    strm.print("LockMode.UPGRADE");
	    break;
	case LockMode.INTENTION_READ:
	    strm.print("LockMode.INTENTION_READ");
	    break;
	case LockMode.INTENTION_WRITE:
	    strm.print("LockMode.INTENTION_WRITE");
	    break;
	}
    }
};
