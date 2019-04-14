/*
 * Copyright (C) 2001,
 *
 * Hewlett-Packard Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TwoPhaseCoordinator.java,v 1.3 2003/07/03 14:10:17 nmcl Exp $
 */

package com.arjuna.ats.arjuna.coordinator;

import com.arjuna.ats.arjuna.common.Uid;

import com.arjuna.ats.arjuna.logging.tsLogger;

import com.arjuna.ats.internal.arjuna.template.*;

/**
 * Adds support for synchronizations to BasicAction. It does not change
 * thread associations either. It also allows any thread to terminate
 * a transaction, even if it is not the transaction that is marked as current
 * for the thread (unlike the BasicAction default).
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: TwoPhaseCoordinator.java,v 1.3 2003/07/03 14:10:17 nmcl Exp $
 * @since JTS 3.0.
 */

public class TwoPhaseCoordinator extends BasicAction implements Reapable
{
    
    public TwoPhaseCoordinator ()
    {
    }

    public TwoPhaseCoordinator (Uid id)
    {
	super(id);
    }

    public int start ()
    {
	return start(BasicAction.Current());
    }

    public int start (BasicAction parentAction)
    {
	int status = ActionStatus.INVALID;

	if (parentAction != null)
	    parentAction.addChildAction(this);
	
	return super.Begin(parentAction);
    }

    public int end (boolean report_heuristics)
    {
	if (parent() != null)
	    parent().removeChildAction(this);

	if (beforeCompletion())
	{
	    int outcome = super.End(report_heuristics);

	    afterCompletion(outcome);

	    return outcome;
	}
	else
	    return Abort();
    }

    public int cancel ()
    {
	if (parent() != null)
	    parent().removeChildAction(this);

	beforeCompletion();
	
	int outcome = super.Abort();

	afterCompletion(outcome);

	return outcome;
    }
    
    public int addSynchronization (SynchronizationRecord sr)
    {
	if (sr == null)
	    return AddOutcome.AR_REJECTED;

	int result = AddOutcome.AR_REJECTED;
	
	if (parent() != null)
	    return AddOutcome.AR_REJECTED;

	switch (status())
	{
	case ActionStatus.RUNNING:
	    {
		if (_synchs == null)
		    _synchs = new HashList(10);

		if (_synchs.add(sr))
		    result = AddOutcome.AR_ADDED;
	    }
	    break;
	default:
	    break;
	}

	return result;
    }

    /**
     * @return <code>true</code> if the transaction is running,
     * <code>false</code> otherwise.
     */

    public boolean running ()
    {
	return (boolean) (status() == ActionStatus.RUNNING);
    }

    /**
     * Overloads BasicAction.type()
     */

    public String type ()
    {
	return "/StateManager/BasicAction/AtomicAction/TwoPhaseCoordinator";
    }

    protected TwoPhaseCoordinator (int at)
    {
	super(at);
    }

    protected TwoPhaseCoordinator (Uid u, int at)
    {
	super(u, at);
    }

    /**
     * @message com.arjuna.ats.arjuna.coordinator.TwoPhaseCoordinator_1 [com.arjuna.ats.arjuna.coordinator.TwoPhaseCoordinator_1] TwoPhaseCoordinator.beforeCompletion - attempted rollback_only failed!
     */

    protected boolean beforeCompletion ()
    {
	boolean problem = false;

	/*
	 * If we have a synchronization list then we must be top-level.
	 */

	if (_synchs != null)
	{
	    HashListIterator iterator = new HashListIterator(_synchs);
	    SynchronizationRecord record = (SynchronizationRecord) iterator.iterate();

	    while ((record != null) && !problem)
	    {
		problem = !record.beforeCompletion();
		    
		record = (SynchronizationRecord) iterator.iterate();
	    }

	    if (problem)
	    {
		if (!preventCommit())
		{
		    /*
		     * This should not happen. If it does, continue with
		     * commit to tidy-up.
		     */
		
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.coordinator.TwoPhaseCoordinator_1");
		}
	    }
	}

	return !problem;
    }

    /**
     * @message com.arjuna.ats.arjuna.coordinator.TwoPhaseCoordinator_2 [com.arjuna.ats.arjuna.coordinator.TwoPhaseCoordinator_2] TwoPhaseCoordinator.beforeCompletion TwoPhaseCoordinator.afterCompletion called on still running transaction!
     * @message com.arjuna.ats.arjuna.coordinator.TwoPhaseCoordinator_3 [com.arjuna.ats.arjuna.coordinator.TwoPhaseCoordinator_3] TwoPhaseCoordinator.afterCompletion - failed for {0}
     */

    protected boolean afterCompletion (int myStatus)
    {
	if (myStatus == ActionStatus.RUNNING)
	{
	    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.coordinator.TwoPhaseCoordinator_2");

	    return false;
	}

	boolean problem = false;
	
	if (_synchs != null)
	{
	    HashListIterator iterator = new HashListIterator(_synchs);
	    SynchronizationRecord record = (SynchronizationRecord) iterator.iterate();
		
	    /*
	     * Regardless of failures, we must tell all synchronizations
	     * what happened.
	     */
	
	    while (record != null)
	    {
		if (!record.afterCompletion(myStatus))
		{
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.coordinator.TwoPhaseCoordinator_3",
					    new Object[]{record});
		   
		    problem = true;
		}

		record = (SynchronizationRecord) iterator.iterate();
	    }

	    _synchs = null;
	}

	return !problem;
    }
    
    private HashList    _synchs;

}
