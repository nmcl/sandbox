/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: ObjectStatus.java,v 1.1 2003/06/19 10:50:13 nmcl Exp $
 */

package com.arjuna.ats.arjuna;

import java.io.PrintWriter;

/**
 * A transactional object may go through a number of different states
 * once it has been created.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ObjectStatus.java,v 1.1 2003/06/19 10:50:13 nmcl Exp $
 * @since JTS 1.0.
 */

public class ObjectStatus
{

public static final int PASSIVE = 0;
public static final int PASSIVE_NEW = 1;
public static final int ACTIVE = 2;
public static final int ACTIVE_NEW = 3;
public static final int DESTROYED = 4;
public static final int UNKNOWN_STATUS = 5;
    
public static void print (PrintWriter strm, int os)
    {
	switch (os)
	{
	case PASSIVE:
	    strm.print("PASSIVE");
	    break;
	case PASSIVE_NEW:
	    strm.print("PASSIVE_NEW");
	    break;
	case ACTIVE:
	    strm.print("ACTIVE");
	    break;
	case ACTIVE_NEW:
	    strm.print("ACTIVE_NEW");
	    break;
	case DESTROYED:
	    strm.print("DESTROYED");
	    break;
	default:
	    strm.print("UNKNOWN_STATUS");
	    break;
	}
    }
    
}
