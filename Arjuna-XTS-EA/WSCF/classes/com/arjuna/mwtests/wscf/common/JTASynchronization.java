/*
 * Copyright (C) 2002,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JTASynchronization.java,v 1.1 2003/01/07 10:37:17 nmcl Exp $
 */

package com.arjuna.mwtests.wscf.common;

import javax.transaction.xa.*;

public class JTASynchronization implements javax.transaction.Synchronization
{

    public void beforeCompletion ()
    {
	System.out.println("beforeCompletion");
    }

    public void afterCompletion (int status)
    {
	System.out.println("afterCompletion :"+status);
    }
 
}
