/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CheckedAction.java,v 1.1 2000/02/25 14:03:45 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

import com.arjuna.ArjunaCommon.Common.Uid;
import java.util.Hashtable;

/**
 * If an action attempts to terminate with threads still active we
 * call an instance of this class to determine what to do. The default
 * simply prints a warning and relies upon the outstanding threads to find
 * out the state of the action later. However, this can be overridden, e.g.,
 * the thread attempting to terminate the action may be made to block.
 *
 * WARNING: watch out for deadlock!
 */

public class CheckedAction
{

public CheckedAction ()
    {
    }

public synchronized void check (boolean isCommit, Uid actUid, Hashtable list)
    {
	System.err.print("\nWARNING CheckedAction::check - atomic action "+actUid);
	
	if (isCommit)
	    System.err.print(" committing ");
	else
	    System.err.print(" aborting ");

	System.err.println("with "+list.size()+" threads active!\n");
    }    
    
}
