/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockMode.java,v 1.4 1998/07/06 13:26:23 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Atomic;

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
