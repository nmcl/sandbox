/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectModel.java,v 1.1 2000/02/25 14:03:52 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import java.io.PrintStream;

public class ObjectModel
{

public static final int SINGLE = 0;
public static final int MULTIPLE = 1;

public static void print (PrintStream strm, int os)
    {
	switch (os)
	{
	case SINGLE:
	    strm.print("SINGLE");
	    break;
	case MULTIPLE:
	    strm.print("MULTIPLE");
	    break;
	}
    }
    
};
