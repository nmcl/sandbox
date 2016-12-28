/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockResult.java,v 1.1 2000/02/25 14:03:51 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import java.io.PrintStream;

public class LockResult
{
    
public static final int GRANTED = 0;
public static final int REFUSED = 1;
public static final int RELEASED = 2;

public static void print (PrintStream strm, int l)
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
	}
    }
	
};
