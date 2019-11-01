/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: AddOutcome.java,v 1.1.4.2.6.1.34.1 2001/06/15 12:06:48 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import java.io.PrintWriter;

/**
 * The possible outcomes when trying to add an AbstractRecord as
 * a participant within a transaction.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: AddOutcome.java,v 1.1.4.2.6.1.34.1 2001/06/15 12:06:48 nmcl Exp $
 * @since JTS 1.0.
 */

public class AddOutcome
{
    
public static final int AR_ADDED = 2;
public static final int AR_REJECTED = 3;
public static final int AR_DUPLICATE = 4;

    /**
     * @since JTS 2.1.2.
     */

public static String printString (int res)
    {
	switch (res)
	{
	case AR_ADDED:
	    return "AddOutcome.AR_ADDED";
	case AR_REJECTED:
	    return "AddOutcome.AR_REJECTED";
	case AR_DUPLICATE:
	    return "AddOutcome.AR_DUPLICATE";
	default:
	    return "Unknown";
	}
    }

public static void print (PrintWriter strm, int res)
    {
	strm.print(printString(res));
    }
    
}
