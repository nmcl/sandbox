/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: AsyncPrepare.java,v 1.1 2000/02/25 14:03:42 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import com.arjuna.ArjunaLite.Thread.*;
import java.lang.Thread;

class AsyncPrepare extends Thread
{

public static AsyncPrepare create (BasicAction act, boolean reportHeuristics,
				   AbstractRecord rec)
    {
	return new AsyncPrepare(act, reportHeuristics, rec);
    }
    
public void run ()
    {
	if (_theAction != null)
	{
	    /*
	     * This is a transient thread so we don't
	     * want to register it with the action it is
	     * preparing, only change its notion of the
	     * current transaction so that any abstract
	     * records that need that information can still
	     * have it.
	     */

	    ThreadActionData.pushAction(_theAction, false);
	    
	    _outcome = _theAction.doPrepare(_reportHeuristics, _theRecord);

	    ThreadActionData.popAction(false);
	}

	_theRecord = null;
	_theAction = null;
    }

public int outcome ()
    {
	return _outcome;
    }
    
protected AsyncPrepare (BasicAction act, boolean reportHeuristics, AbstractRecord rec)
    {
	_theAction = act;
	_outcome = PrepareOutcome.PREP_NOTOK;
	_reportHeuristics = reportHeuristics;
	_theRecord = rec;
    }

private BasicAction _theAction;
private int _outcome;
private boolean _reportHeuristics;
private AbstractRecord _theRecord;
    
};
