/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActionStatus.java,v 1.5 1998/07/15 15:34:12 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Atomic;

import java.io.PrintStream;

public class ActionStatus
{

public static final int RUNNING = 0;
public static final int PREPARING = 1;
public static final int ABORTING = 2;
public static final int ABORT_ONLY = 3;
public static final int ABORTED = 4;
public static final int PREPARED = 5;
public static final int COMMITTING = 6;
public static final int COMMITTED = 7;
public static final int CREATED = 8;
public static final int INVALID = 9;
public static final int CLEANUP = 10;
public static final int H_ROLLBACK = 11;
public static final int H_COMMIT = 12;
public static final int H_MIXED = 13;
public static final int H_HAZARD = 14;

public static void print (PrintStream strm, int res)
    {
	switch (res)
	{
	case RUNNING:
	    strm.println("ActionStatus.RUNNING");
	    break;
	case PREPARING:
	    strm.println("ActionStatus.PREPARING");
	    break;
	case ABORTING:
	    strm.println("ActionStatus.ABORTING");
	    break;
	case ABORT_ONLY:
	    strm.println("ActionStatus.ABORT_ONLY");
	    break;
	case ABORTED:
	    strm.println("ActionStatus.ABORTED");
	    break;
	case PREPARED:
	    strm.println("ActionStatus.PREPARED");
	    break;
	case COMMITTING:
	    strm.println("ActionStatus.COMMITTING");
	    break;
	case COMMITTED:
	    strm.println("ActionStatus.COMMITTED");
	    break;
	case CREATED:
	    strm.println("ActionStatus.CREATED");
	    break;
	case INVALID:
	    strm.println("ActionStatus.INVALID");
	    break;
	case CLEANUP:
	    strm.println("ActionStatus.CLEANUP");
	    break;
	case H_ROLLBACK:
	    strm.println("ActionStatus.H_ROLLBACK");
	    break;
	case H_COMMIT:
	    strm.println("ActionStatus.H_COMMIT");
	    break;
	case H_MIXED:
	    strm.println("ActionStatus.H_MIXED");
	    break;
	case H_HAZARD:
	    strm.println("ActionStatus.H_HAZARD");
	    break;	    
	}
    }

};

