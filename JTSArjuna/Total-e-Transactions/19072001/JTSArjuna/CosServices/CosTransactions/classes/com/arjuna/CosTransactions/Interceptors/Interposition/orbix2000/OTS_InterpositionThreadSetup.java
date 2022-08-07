/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_InterpositionThreadSetup.java,v 1.1.2.1.16.1 2001/05/15 11:19:27 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interceptors.Interposition.orbix2000;

import com.arjuna.ArjunaCore.Thread.ThreadActionData;
import com.arjuna.ArjunaCore.Thread.ThreadSetup;
import com.arjuna.CosTransactions.*;

class OTS_InterpositionThreadSetup implements ThreadSetup
{

public void setup ()
    {
	/*
	 * Simply getting (or trying to get) the current tx control
	 * will ensure that this thread is initialised properly. We
	 * have to do this because in a POA implementation the receiving
	 * thread may not be the same one which does the work, so we
	 * cannot do thread association at the interceptor level. We must
	 * do it when the invoked method actually gets called.
	 */

	OTS_Current curr = OTS.current();

	/*
	 * Probably separate the underlying work out so that we can
	 * call that directly. No real harm at present since the hard
	 * work represents most of the overhead and has to be done
	 * anyway.
	 */

	curr.contextManager().associate();
    }

OTS_InterpositionThreadSetup ()
    {
    }
    
}
