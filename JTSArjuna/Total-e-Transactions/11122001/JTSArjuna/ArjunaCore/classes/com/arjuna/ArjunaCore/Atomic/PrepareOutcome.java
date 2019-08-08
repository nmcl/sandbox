/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: PrepareOutcome.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:04 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import java.io.PrintWriter;

/*
 * If Java had proper reference parameter passing and/or allowed
 * the wrappers for basic types to modify the contents, then we
 * would not have to do this! This class should only be an "enum".
 */

/**
 * The outcomes which can be generated when a transaction
 * attempts to prepare/commit/rollback.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: PrepareOutcome.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:04 nmcl Exp $
 * @since JTS 1.0.
 */

public class PrepareOutcome
{

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

    /**
     * Print a human-readable version of the outcome.
     */

public static String printString (int res)
    {
	switch (res)
	{
	case PREP_OK:
	    return "PrepareOutcome.PREP_OK";
	case PREP_NOTOK:
	    return "PrepareOutcome.PREP_NOTOK";
	case PREP_READONLY:
	    return "PrepareOutcome.PREP_READONLY";
	case HEURISTIC_ROLLBACK:
	    return "PrepareOutcome.HEURISTIC_ROLLBACK";
	case HEURISTIC_COMMIT:
	    return "PrepareOutcome.HEURISTIC_COMMIT";
	case HEURISTIC_MIXED:
	    return "PrepareOutcome.HEURISTIC_MIXED";
	case HEURISTIC_HAZARD:
	    return "PrepareOutcome.HEURISTIC_HAZARD";
	case FINISH_OK:
	    return "PrepareOutcome.FINISH_OK";
	case FINISH_ERROR:
	    return "PrepareOutcome.FINISH_ERROR";
	case NOT_PREPARED:
	    return "PrepareOutcome.NOT_PREPARED";
	case ONE_PHASE_ERROR:
	    return "PrepareOutcome.ONE_PHASE_ERROR";
	case INVALID_TRANSACTION:
	    return "PrepareOutcome.INVALID_TRANSACTION";
	default:
	    return "Unknown";
	}
    }
    
public static void print (PrintWriter strm, int res)
    {
	strm.print(PrepareOutcome.printString(res));
	strm.flush();
    }

private int _outcome;
	
};
