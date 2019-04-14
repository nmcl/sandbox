/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CheckedAction.java,v 1.1 2003/06/19 10:50:16 nmcl Exp $
 */

package com.arjuna.ats.arjuna.coordinator;

import com.arjuna.ats.arjuna.common.Uid;
import java.util.Hashtable;

import com.arjuna.ats.arjuna.logging.tsLogger;

/**
 * If an action attempts to terminate with threads still active we
 * call an instance of this class to determine what to do. The default
 * simply prints a warning and relies upon the outstanding threads to find
 * out the state of the action later. However, this can be overridden, e.g.,
 * the thread attempting to terminate the action may be made to block.
 *
 * WARNING: watch out for deadlock!
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: CheckedAction.java,v 1.1 2003/06/19 10:50:16 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class CheckedAction
{

public CheckedAction ()
    {
    }

    /**
     * Called during transaction termination if more than one thread
     * is associated with the transaction. The supplied information
     * should be sufficient for application specific implementations to
     * do useful work (such as synchronizing on the threads).
     *
     * @message com.arjuna.ats.arjuna.coordinator.CheckedAction_1 [com.arjuna.ats.arjuna.coordinator.CheckedAction_1] - CheckedAction::check - atomic action {0} commiting with {1} threads active!
     * @message com.arjuna.ats.arjuna.coordinator.CheckedAction_2 [com.arjuna.ats.arjuna.coordinator.CheckedAction_2] - CheckedAction::check - atomic action {0} aborting with {1} threads active!
     */

public synchronized void check (boolean isCommit, Uid actUid, Hashtable list)
    {
	if (tsLogger.arjLoggerI18N.isWarnEnabled())
	{
	    if (isCommit)
		tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.coordinator.CheckedAction_1", 
					    new Object[]{actUid,Integer.toString(list.size())});
	    else
		tsLogger.arjLoggerI18N.warn("com.arjuna.ats.arjuna.coordinator.CheckedAction_2", 
					    new Object[]{actUid,Integer.toString(list.size())});
	}
    }    

}
