/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ActionType.java,v 1.1 2003/06/19 10:50:15 nmcl Exp $
 */

package com.arjuna.ats.arjuna.coordinator;

import java.io.PrintWriter;

/**
 * The two types of transactions, nested, and top-level.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ActionType.java,v 1.1 2003/06/19 10:50:15 nmcl Exp $
 * @since 1.0.
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
