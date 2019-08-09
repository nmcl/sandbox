/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CheckedAction.java,v 1.1.4.1.2.1.4.2 2001/01/04 12:36:13 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import com.arjuna.ArjunaCommon.Common.Uid;
import com.arjuna.ArjunaCommon.Common.ErrorStream;
import java.util.Hashtable;

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
 * @version $Id: CheckedAction.java,v 1.1.4.1.2.1.4.2 2001/01/04 12:36:13 nmcl Exp $
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
     */

public synchronized void check (boolean isCommit, Uid actUid, Hashtable list)
    {
	ErrorStream.stream("\n", ErrorStream.WARNING).print("CheckedAction::check - atomic action "+actUid);
	
	if (isCommit)
	    ErrorStream.stream().print(" committing ");
	else
	    ErrorStream.stream().print(" aborting ");

	ErrorStream.stream().println("with "+list.size()+" threads active!");
    }    
    
}
