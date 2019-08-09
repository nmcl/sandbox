/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectModel.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:03 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import java.io.PrintWriter;

/**
 * An enumeration of the types of object model supported.
 * Based upon the model type, certain optimisations may be
 * used.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ObjectModel.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:03 nmcl Exp $
 * @since JTS 1.0.
 */

public class ObjectModel
{

    /**
     * In the SINGLE model, it is assumed that only a single instance
     * of the object will exist within a single JVM.
     */

public static final int SINGLE = 0;

    /**
     * In the MULTIPLE model, it is assumed that multiple instances of
     * the object may exist in different JVMs concurrently.
     */

public static final int MULTIPLE = 1;

    /**
     * Print out a human-readable form of the model type.
     */

public static void print (PrintWriter strm, int os)
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
    
}
