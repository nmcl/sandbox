/*
 * Copyright (C) 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PrepareOutcome.java,v 1.5 1998/08/13 14:31:36 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Atomic;

import java.io.PrintStream;

/*
 * If Java had proper reference parameter passing and/or allowed
 * the wrappers for basic types to modify the contents, then we
 * would not have to do this! This class should only be an "enum".
 */

public class PrepareOutcome
{

public PrepareOutcome (int outcome)
    {
	_outcome = outcome;
    }

public void setOutcome (int outcome)
    {
	_outcome = outcome;
    }

public int getOutcome ()
    {
	return _outcome;
    }
    
public static final int PREP_OK = 0;
public static final int PREP_NOTOK = 1;
public static final int READONLY = 2;
public static final int HEURISTIC_ROLLBACK = 3;
public static final int HEURISTIC_COMMIT = 4;
public static final int HEURISTIC_MIXED = 5;
public static final int HEURISTIC_HAZARD = 6;
public static final int FINISH_OK = 7;
public static final int FINISH_ERROR = 8;
public static final int NOT_PREPARED = 9;
    	
private int _outcome;

public static void print (PrintStream strm, int res)
    {
	switch (res)
	{
	case PREP_OK:
	    strm.print("PrepareOutcome.PREP_OK");
	    break;
	case PREP_NOTOK:
	    strm.print("PrepareOutcome.PREP_NOTOK");
	    break;
	case READONLY:
	    strm.print("PrepareOutcome.READONLY");
	    break;
	case HEURISTIC_ROLLBACK:
	    strm.print("PrepareOutcome.HEURISTIC_ROLLBACK");
	    break;
	case HEURISTIC_COMMIT:
	    strm.print("PrepareOutcome.HEURISTIC_COMMIT");
	    break;
	case HEURISTIC_MIXED:
	    strm.print("PrepareOutcome.HEURISTIC_MIXED");
	    break;
	case HEURISTIC_HAZARD:
	    strm.print("PrepareOutcome.HEURISTIC_HAZARD");
	    break;
	case FINISH_OK:
	    strm.print("PrepareOutcome.FINISH_OK");
	    break;
	case FINISH_ERROR:
	    strm.print("PrepareOutcome.FINISH_ERROR");
	    break;
	case NOT_PREPARED:
	    strm.print("PrepareOutcome.NOT_PREPARED");
	    break;	    
	}
    }
	
};
