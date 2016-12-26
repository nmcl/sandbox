/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ThreadSetup.java,v 1.1.2.1 2001/02/14 10:42:40 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Thread;

/**
 * Implementations of this class can be registered with the low-level
 * transaction-threading system to set up any thread data that is needed
 * for thread-to-transaction tracking to be done correctly. This level of
 * indirection allows us to keep the core neutral to such issues (which tend
 * to be ORB specific), so that it can be used "raw".
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: ThreadSetup.java,v 1.1.2.1 2001/02/14 10:42:40 nmcl Exp $
 * @since JTS 2.1.
 */

public interface ThreadSetup
{

public void setup ();

}
