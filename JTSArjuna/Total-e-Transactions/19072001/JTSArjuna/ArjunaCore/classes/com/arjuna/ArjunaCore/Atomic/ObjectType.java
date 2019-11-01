/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectType.java,v 1.1.4.1.2.1.4.2 2001/01/04 12:34:54 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import java.io.PrintWriter;

/**
 * The various types of StateManager object which
 * can exist.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ObjectType.java,v 1.1.4.1.2.1.4.2 2001/01/04 12:34:54 nmcl Exp $
 * @since JTS 1.0.
 */

public class ObjectType
{

public static final int RECOVERABLE = 0;
public static final int ANDPERSISTENT = 1;
public static final int NEITHER = 2;
public static final int UNKNOWN_TYPE = 3;
    
    /**
     * Print a human-readable form of the object type.
     */

public static void print (PrintWriter strm, int ot)
    {
	switch (ot)
	{
	case RECOVERABLE:
	    strm.print("RECOVERABLE");
	    break;
	case ANDPERSISTENT:
	    strm.print("ANDPERSISTENT");
	    break;
	case NEITHER:
	    strm.print("NEITHER");
	    break;
	default:
	    strm.print("UNKNOWN_TYPE");
	    break;
	}
    }

}
