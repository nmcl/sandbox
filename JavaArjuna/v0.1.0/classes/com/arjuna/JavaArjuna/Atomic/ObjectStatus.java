/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectStatus.java,v 1.5 1998/07/06 13:26:27 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Atomic;

import java.io.PrintStream;

public class ObjectStatus
{

public static final int PASSIVE = 0;
public static final int PASSIVE_NEW = 1;
public static final int ACTIVE = 2;
public static final int ACTIVE_NEW = 3;

public static void print (PrintStream strm, int os)
    {
	strm.print((os == PASSIVE ? "PASSIVE"
                                  : (os == PASSIVE_NEW ? "PASSIVE_NEW"
                                                       : (os == ACTIVE ? "ACTIVE"
							               : "ACTIVE_NEW"))));
    }
    
};
