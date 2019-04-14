/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TwoPhaseOutcome.java,v 1.1 2003/06/19 10:50:17 nmcl Exp $
 */

package com.arjuna.ats.arjuna.coordinator;

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
 * @version $Id: TwoPhaseOutcome.java,v 1.1 2003/06/19 10:50:17 nmcl Exp $
 * @since JTS 1.0.
 */

public class TwoPhaseOutcome
{

    public static final int PREPARE_OK = 0;
    public static final int PREPARE_NOTOK = 1;
    public static final int PREPARE_READONLY = 2;
    public static final int HEURISTIC_ROLLBACK = 3;
    public static final int HEURISTIC_COMMIT = 4;
    public static final int HEURISTIC_MIXED = 5;
    public static final int HEURISTIC_HAZARD = 6;
    public static final int FINISH_OK = 7;
    public static final int FINISH_ERROR = 8;
    public static final int NOT_PREPARED = 9;
    public static final int ONE_PHASE_ERROR = 10;
    public static final int INVALID_TRANSACTION = 11;

    public TwoPhaseOutcome (int outcome)
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
     * @return <code>String</code> representation of the status.
     */

    public static String stringForm (int res)
    {
	return printString(res);
    }

    /**
     * Print a human-readable version of the outcome.
     * @deprecated Use stringForm
     */

    public static String printString (int res)
    {
	switch (res)
	{
	case PREPARE_OK:
	    return "TwoPhaseOutcome.PREPARE_OK";
	case PREPARE_NOTOK:
	    return "TwoPhaseOutcome.PREPARE_NOTOK";
	case PREPARE_READONLY:
	    return "TwoPhaseOutcome.PREPARE_READONLY";
	case HEURISTIC_ROLLBACK:
	    return "TwoPhaseOutcome.HEURISTIC_ROLLBACK";
	case HEURISTIC_COMMIT:
	    return "TwoPhaseOutcome.HEURISTIC_COMMIT";
	case HEURISTIC_MIXED:
	    return "TwoPhaseOutcome.HEURISTIC_MIXED";
	case HEURISTIC_HAZARD:
	    return "TwoPhaseOutcome.HEURISTIC_HAZARD";
	case FINISH_OK:
	    return "TwoPhaseOutcome.FINISH_OK";
	case FINISH_ERROR:
	    return "TwoPhaseOutcome.FINISH_ERROR";
	case NOT_PREPARED:
	    return "TwoPhaseOutcome.NOT_PREPARED";
	case ONE_PHASE_ERROR:
	    return "TwoPhaseOutcome.ONE_PHASE_ERROR";
	case INVALID_TRANSACTION:
	    return "TwoPhaseOutcome.INVALID_TRANSACTION";
	default:
	    return "Unknown";
	}
    }
    
    public static void print (PrintWriter strm, int res)
    {
	strm.print(TwoPhaseOutcome.printString(res));
	strm.flush();
    }

    private int _outcome;
	
}
