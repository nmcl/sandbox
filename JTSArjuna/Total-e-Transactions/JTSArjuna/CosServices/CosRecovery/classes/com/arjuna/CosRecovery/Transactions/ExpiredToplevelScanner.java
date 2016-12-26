/*
 * Copyright (C) 2001
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ExpiredToplevelScanner.java,v 1.1.2.2 2001/02/15 21:40:09 pfurniss Exp $
 */

package com.arjuna.CosRecovery.Transactions;

import org.omg.CosTransactions.*;

import java.util.*;
import java.io.PrintWriter;
import java.text.*;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Atomic.InitAction;
import com.arjuna.CosTransactions.*;
import com.arjuna.CosRecovery.*;

/**
 * Refinement of the expired assumed scanner for toplevel transactions.
 * @see AssumedCompleteTransaction
 */

public class ExpiredToplevelScanner extends ExpiredAssumedCompleteScanner
{
    /**
     * Construction is caused by presence of class name as property value.
     * @see com.arjuna.CosRecovery.ExpiredEntryMonitor
     */
    public ExpiredToplevelScanner ()
    {
	super(AssumedCompleteTransaction.typeName(),InitAction.getStore());
    
    }
}