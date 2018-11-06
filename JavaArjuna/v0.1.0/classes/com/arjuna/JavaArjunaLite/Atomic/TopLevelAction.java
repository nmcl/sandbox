/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: TopLevelAction.java,v 1.5 1998/07/06 13:26:33 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Atomic;

/*
 * Derived from AtomicAction so we can get the action-to-thread
 * tracking.
 */

public class TopLevelAction extends AtomicAction
{

public TopLevelAction ()
    {
	super(BasicAction.TOP_LEVEL);
    }

};
