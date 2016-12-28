/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ActionType.java,v 1.1 2000/02/25 14:03:40 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import java.io.*;

public class ActionType
{
    
public static final int TOP_LEVEL = 0;
public static final int NESTED = 1;

public static void print (PrintStream strm, int res)
    {
	switch (res)
	{
	case TOP_LEVEL:
	    strm.print("ActionType.TOP_LEVEL");
	    break;
	case NESTED:
	    strm.print("ActionType.NESTED");
	}
    }
    
}
