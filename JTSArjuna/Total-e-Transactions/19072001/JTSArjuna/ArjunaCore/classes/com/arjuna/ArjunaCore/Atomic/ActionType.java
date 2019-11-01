/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ActionType.java,v 1.1.4.2.2.1.4.1 2000/12/21 11:20:59 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import java.io.PrintWriter;

/**
 * The two types of transactions, nested, and top-level.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ActionType.java,v 1.1.4.2.2.1.4.1 2000/12/21 11:20:59 nmcl Exp $
 * @since JTS 1.0.
 */

public class ActionType
{
    
public static final int TOP_LEVEL = 0;
public static final int NESTED = 1;

    /**
     * Print a human-reabable version of the type.
     */

public static void print (PrintWriter strm, int res)
    {
	switch (res)
	{
	case TOP_LEVEL:
	    strm.print("ActionType.TOP_LEVEL");
	    break;
	case NESTED:
	    strm.print("ActionType.NESTED");
	    break;
	default:
	    strm.print("Unknown");
	    break;
	}
    }
    
}
