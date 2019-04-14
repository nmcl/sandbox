/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: AsyncPrepare.java,v 1.1 2003/06/19 10:50:16 nmcl Exp $
 */

package com.arjuna.ats.arjuna.coordinator;

import com.arjuna.ats.internal.arjuna.thread.*;
import java.lang.Thread;

/**
 * Instances of this class are responsible for performing asynchronous
 * prepare on a specific AbstractRecord associated with a transaction.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: AsyncPrepare.java,v 1.1 2003/06/19 10:50:16 nmcl Exp $
 * @since JTS 1.2.4.
 */

/*
 * Default visibility.
 */

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
	_outcome = TwoPhaseOutcome.PREPARE_NOTOK;
	_reportHeuristics = reportHeuristics;
	_theRecord = rec;
    }

private BasicAction    _theAction;
private int            _outcome;
private boolean        _reportHeuristics;
private AbstractRecord _theRecord;
    
};
