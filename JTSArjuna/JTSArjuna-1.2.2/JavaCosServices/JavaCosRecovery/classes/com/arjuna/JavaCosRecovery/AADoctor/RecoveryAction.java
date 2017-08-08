/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecoveryAction.java,v 1.3 1998/07/06 10:49:44 nmcl Exp $
 */

package com.arjuna.JavaCosRecovery.AADoctor;

import com.arjuna.JavaArjunaLite.Atomic.BasicAction;
import com.arjuna.JavaGandiva.Common.Uid;

/*
 * Default visibility.
 */

class RecoveryAction extends BasicAction
{

public RecoveryAction (Uid actUid)
    {
	super(actUid);
    }

    /*
     * The following routine is used by crash recovery to commit an incorrectly
     * terminated action.
     */

public synchronized final void crashRecoveryCommit()
    {
	super.phase2Commit(true);

	/*
	 * May have heuritics, so deal with them!
	 */
    }

};
