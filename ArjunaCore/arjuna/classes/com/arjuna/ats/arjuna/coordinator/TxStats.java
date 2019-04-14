/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TxStats.java,v 1.1 2003/06/19 10:50:17 nmcl Exp $
 */

package com.arjuna.ats.arjuna.coordinator;

/**
 * This class is used to maintain statistics on transactions that
 * have been created. This includes the number of transactions, their
 * termination status (committed or rolled back), ...
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: TxStats.java,v 1.1 2003/06/19 10:50:17 nmcl Exp $
 * @since JTS 2.1.
 */

public class TxStats
{

    /**
     * @return the number of transactions (top-level and nested)
     * created so far.
     */

public static int numberOfTransactions ()
    {
	synchronized (_ntxLock)
	    {
		return _numberOfTransactions;
	    }
    }

    /**
     * @return the number of nested (sub) transactions created so far.
     */

public static int numberOfNestedTransactions ()
    {
	synchronized (_nntxLock)
	    {
		return _numberOfNestedTransactions;
	    }
    }
    
    /**
     * @return the number of transactions which have terminated with
     * heuristic outcomes.
     */

public static int numberOfHeuristics ()
    {
	synchronized (_nhLock)
	    {
		return _numberOfHeuristics;
	    }
    }

    /**
     * @return the number of committed transactions.
     */

public static int numberOfCommittedTransactions ()
    {
	synchronized (_ncmLock)
	    {
		return _numberOfCommittedTransactions;
	    }
    }

    /**
     * @return the number of transactions which have rolled back.
     */

public static int numberOfAbortedTransactions ()
    {
	synchronized (_nabLock)
	    {
		return _numberOfAbortedTransactions;
	    }
    }
    
static void incrementTransactions ()
    {
	synchronized (_ntxLock)
	    {
		_numberOfTransactions++;
	    }
    }

static void incrementNestedTransactions ()
    {
	synchronized (_nntxLock)
	    {
		_numberOfNestedTransactions++;
	    }
    }

static void incrementAbortedTransactions ()
    {
	synchronized (_nabLock)
	    {
		_numberOfAbortedTransactions++;
	    }
    }

static void incrementCommittedTransactions ()
    {
	synchronized (_ncmLock)
	    {
		_numberOfCommittedTransactions++;
	    }
    }

static void incrementHeuristics ()
    {
	synchronized (_nhLock)
	    {
		_numberOfHeuristics++;
	    }
    }
    
private static int _numberOfTransactions = 0;
private static java.lang.Object _ntxLock = new Object();
private static int _numberOfNestedTransactions = 0;
private static java.lang.Object _nntxLock = new Object();
private static int _numberOfCommittedTransactions = 0;
private static java.lang.Object _ncmLock = new Object();
private static int _numberOfAbortedTransactions = 0;
private static java.lang.Object _nabLock = new Object();
private static int _numberOfHeuristics = 0;
private static java.lang.Object _nhLock = new Object();
    
}
