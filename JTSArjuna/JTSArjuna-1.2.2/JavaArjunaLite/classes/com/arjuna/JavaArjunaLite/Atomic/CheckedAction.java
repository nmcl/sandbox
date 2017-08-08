/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CheckedAction.java,v 1.10 1998/07/06 13:26:18 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Atomic;

import com.arjuna.JavaGandiva.Common.Uid;
import com.arjuna.JavaGandiva.Template.BasicList;

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

public synchronized void check (boolean isCommit, Uid actUid, BasicList list)
    {
	System.err.print("\nWARNING CheckedAction::check - atomic action "+actUid);
	
	if (isCommit)
	    System.err.print(" committing ");
	else
	    System.err.print(" aborting ");

	System.err.println("with "+list.size()+" threads active!\n");
    }    
    
}
