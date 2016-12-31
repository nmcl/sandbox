/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TopLevelAction.java,v 1.1 2000/02/25 14:03:57 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Atomic;

/*
 * Derived from AtomicAction so we can get the action-to-thread
 * tracking.
 */

public class TopLevelAction extends AtomicAction
{

public TopLevelAction ()
    {
	super(ActionType.TOP_LEVEL);
    }

};
