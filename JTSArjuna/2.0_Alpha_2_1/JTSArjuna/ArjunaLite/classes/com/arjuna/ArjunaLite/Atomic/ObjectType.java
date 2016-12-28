/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectType.java,v 1.1 2000/02/25 14:03:53 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import java.io.PrintStream;

public class ObjectType
{

public static final int RECOVERABLE = 0;
public static final int ANDPERSISTENT = 1;
public static final int NEITHER = 2;

public static void print (PrintStream strm, int ot)
    {
	strm.print((ot == RECOVERABLE ? "RECOVERABLE"
                                      : (ot == ANDPERSISTENT ? "ANDPERSISTENT"
					                     : "NEITHER")));
    }

};
