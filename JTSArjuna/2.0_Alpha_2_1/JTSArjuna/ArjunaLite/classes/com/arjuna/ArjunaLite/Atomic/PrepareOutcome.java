/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: PrepareOutcome.java,v 1.1.4.1 2000/04/25 08:35:59 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import java.io.*;

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

    /*
     * Eventually have a separate type for heuristic values.
     */
    
public static final int PREP_OK = 0;
public static final int PREP_NOTOK = 1;
public static final int PREP_READONLY = 2;
public static final int HEURISTIC_ROLLBACK = 3;
public static final int HEURISTIC_COMMIT = 4;
public static final int HEURISTIC_MIXED = 5;
public static final int HEURISTIC_HAZARD = 6;
public static final int FINISH_OK = 7;
public static final int FINISH_ERROR = 8;
public static final int NOT_PREPARED = 9;
public static final int ONE_PHASE_ERROR = 10;
public static final int INVALID_TRANSACTION = 11;
    	
private int _outcome;

public static void print (PrintStream strm, int res)
    {
	print(new PrintWriter(strm), res);
    }
    
public static void print (PrintWriter strm, int res)
    {
	switch (res)
	{
	case PREP_OK:
	    strm.print("PrepareOutcome.PREP_OK");
	    break;
	case PREP_NOTOK:
	    strm.print("PrepareOutcome.PREP_NOTOK");
	    break;
	case PREP_READONLY:
	    strm.print("PrepareOutcome.PREP_READONLY");
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
	case ONE_PHASE_ERROR:
	    strm.print("PrepareOutcome.ONE_PHASE_ERROR");
	    break;
	case INVALID_TRANSACTION:
	    strm.print("PrepareOutcome.INVALID_TRANSACTION");
	    break;
	}

	strm.flush();
    }
	
};
