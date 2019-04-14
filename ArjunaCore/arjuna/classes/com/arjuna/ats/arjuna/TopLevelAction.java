/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TopLevelAction.java,v 1.1 2003/06/19 10:50:13 nmcl Exp $
 */

package com.arjuna.ats.arjuna;

import com.arjuna.ats.arjuna.coordinator.ActionType;

/**
 * This class provides a (nested) top-level transaction. So, no
 * matter how deeply nested a thread may be within a transaction
 * hierarchy, creating an instance of this class will always start
 * a new top-level transaction.
 *
 * Derived from AtomicAction so we can get the action-to-thread
 * tracking.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: TopLevelAction.java,v 1.1 2003/06/19 10:50:13 nmcl Exp $
 * @since JTS 1.0.
 */

public class TopLevelAction extends AtomicAction
{

public TopLevelAction ()
    {
	super(ActionType.TOP_LEVEL);
    }

}
