/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryStatus.java,v 1.1.2.1.116.1 2002/11/14 13:28:58 nmcl Exp $
 */

/*
 * Status flags for recovered transactions.
 * <P>
 * @author Dave Ingham (dave@arjuna.com)
 * @version $Id: RecoveryStatus.java,v 1.1.2.1.116.1 2002/11/14 13:28:58 nmcl Exp $ */

package com.arjuna.CosRecovery.Transactions;

public class RecoveryStatus
{
    public static int NEW = 0;
    public static int ACTIVATED = 1;
    public static int ACTIVATE_FAILED = 2;
    public static int REPLAYING = 3;
    public static int REPLAYED = 4;
    public static int REPLAY_FAILED = 5;
}

