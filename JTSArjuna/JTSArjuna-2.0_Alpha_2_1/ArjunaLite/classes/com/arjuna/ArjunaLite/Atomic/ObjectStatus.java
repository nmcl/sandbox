/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStatus.java,v 1.1 2000/02/25 14:03:52 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

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
