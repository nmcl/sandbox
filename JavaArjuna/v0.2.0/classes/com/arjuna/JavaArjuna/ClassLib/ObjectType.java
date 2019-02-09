/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectType.java,v 1.5 1998/07/06 13:26:27 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

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
