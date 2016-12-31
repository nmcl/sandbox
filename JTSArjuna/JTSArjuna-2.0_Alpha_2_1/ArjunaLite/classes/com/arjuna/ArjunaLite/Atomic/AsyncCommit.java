/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: AsyncCommit.java,v 1.1 2000/02/25 14:03:42 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import com.arjuna.ArjunaLite.Thread.*;
import java.lang.Thread;

class AsyncCommit extends Thread
{

public static AsyncCommit create (BasicAction toControl, boolean commit)
    {
	AsyncCommit c = new AsyncCommit(toControl, commit);

	c.start();

	Thread.yield();

	return c;
    }
    
public void run ()
    {
	if (_theAction != null)
	{
	    /*
	     * This is a transient thread so we don't
	     * want to register it with the action it is
	     * committing/aborting, only change its notion of the
	     * current transaction so that any abstract
	     * records that need that information can still
	     * have it.
	     */
	    
	    ThreadActionData.pushAction(_theAction, false);
	    
	    if (_commit)
		doPhase2Commit();
	    else
		doPhase2Abort();

	    ThreadActionData.popAction(false);
	}
    }

protected AsyncCommit (BasicAction toControl, boolean commit)
    {
	_theAction = toControl;
	_commit = commit;
    }

protected synchronized boolean doPhase2Commit ()
    {
	if (_theAction != null)
	{
	    /*
	     * Don't want heuristic information, otherwise would
	     * not be asynchronous.
	     */

	    _theAction.phase2Commit(false);
	    return true;
	}
	else
	    return false;
    }

protected boolean doPhase2Abort ()
    {
	if (_theAction != null)
	{
	    _theAction.phase2Abort(false);
	    return true;
	}
	else
	    return false;
    }

private BasicAction _theAction;
private boolean _commit;

};
