/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: AddOutcome.java,v 1.1 2000/02/25 14:03:41 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import java.io.*;

public class AddOutcome
{
    
public static final int AR_ADDED = 2;
public static final int AR_REJECTED = 3;
public static final int AR_DUPLICATE = 4;

public static void print (PrintStream strm, int res)
    {
	switch (res)
	{
	case AR_ADDED:
	    strm.print("AddOutcome.AR_ADDED");
	    break;
	case AR_REJECTED:
	    strm.print("AddOutcome.AR_REJECTED");
	    break;
	case AR_DUPLICATE:
	    strm.print("AddOutcome.AR_DUPLICATE");
	    break;
	}
    }
    
}
