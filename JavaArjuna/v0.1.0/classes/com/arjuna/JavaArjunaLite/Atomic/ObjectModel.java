/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectModel.java,v 1.3 1998/07/06 13:26:26 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Atomic;

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
