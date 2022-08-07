/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryStatus.java,v 1.1.2.1 2001/01/29 18:38:09 pfurniss Exp $
 */

/*
 * Status flags for recovered transactions.
 * <P>
 * @author Dave Ingham (dave@arjuna.com)
 * @version $Id: RecoveryStatus.java,v 1.1.2.1 2001/01/29 18:38:09 pfurniss Exp $ */

package com.arjuna.CosRecovery.Transactions;

public class RecoveryStatus
{
    public static int NEW = 0;
    public static int ACTIVATED = 1;
    public static int ACTIVATE_FAILED = 2;
    public static int REPLAYING = 3;
    public static int REPLAYED = 4;
}

