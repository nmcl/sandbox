/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: InitAction.java,v 1.1.2.2.2.2.34.1.4.4 2001/07/20 12:56:25 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Atomic;

import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.ArjunaCommon.Common.ClassName;
import com.arjuna.ArjunaCore.Common.ArjunaCoreEnvironment;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.ArjunaCoreNames;

/*
 * Transaction configuration object. We now have a separate object for
 * this so that other classes can enquire of (and use) this information.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: InitAction.java,v 1.1.2.2.2.2.34.1.4.4 2001/07/20 12:56:25 nmcl Exp $
 * @since JTS 2.1.
 */

public class InitAction
{

public static ClassName actionStoreType = null;

    /**
     * @return the <code>ObjectStore</code> implementation which the
     * transaction coordinator will use.
     * @see com.arjuna.ArjunaCore.Interface.ObjectStore
     */

public static final ObjectStore getStore ()
    {
	/*
	 * Check for action store once per application. The second
	 * parameter is the default value, which is returned if no
	 * other value is specified.
	 */
	    
	if (InitAction.actionStoreType == null)
	{
	    String useLog = PropertyManager.getProperty(ArjunaCoreEnvironment.TRANSACTION_LOG, "OFF");

	    if (useLog.equals("ON"))
		InitAction.actionStoreType = new ClassName(ArjunaCoreNames.Implementation_ObjectStore_ActionLogStore());
	    else
		InitAction.actionStoreType = new ClassName(PropertyManager.getProperty(ArjunaCoreEnvironment.ACTION_STORE, ArjunaCoreNames.Implementation_ObjectStore_defaultActionStore().stringForm()));

	    String sharedLog = PropertyManager.getProperty(ArjunaCoreEnvironment.SHARED_TRANSACTION_LOG, "NO");
	    
	    if (sharedLog.equals("YES"))
		sharedTransactionLog = true;
	}

	/*
	 * Defaults to ObjectStore.OS_UNSHARED
	 */
	
	if (sharedTransactionLog)
	    return new ObjectStore(actionStoreType, ObjectStore.OS_SHARED);
	else
	    return new ObjectStore(actionStoreType);
    }

static boolean maintainHeuristics = false;
static boolean asyncCommit = false;
static boolean asyncPrepare = false;
static boolean onePhase = true;
static boolean readonlyOptimisation = true;
static boolean onePhaseRestriction = false;
static boolean restrictedNumberOfTwoPhaseTransactions = false;
static boolean restrictedNumberOfOnePhaseTransactions = false;
static boolean enableStatistics = false;
static boolean sharedTransactionLog = false;
static int     numberOfTransactions = 100;
static boolean enable = true;
    
    static
    {
	String env = PropertyManager.getProperty(ArjunaCoreEnvironment.MAINTAIN_HEURISTICS);
	
	if (env != null)
	{
	    if (env.compareTo("YES") == 0)
		InitAction.maintainHeuristics = true;
	}

	env = PropertyManager.getProperty(ArjunaCoreEnvironment.ASYNC_COMMIT);
	    
	if (env != null)
	{
	    if (env.compareTo("YES") == 0)
		InitAction.asyncCommit = true;
	}

	env = PropertyManager.getProperty(ArjunaCoreEnvironment.ASYNC_PREPARE);
	    
	if (env != null)
	{
	    if (env.compareTo("YES") == 0)
		InitAction.asyncPrepare = true;
	}

	env = PropertyManager.getProperty(ArjunaCoreEnvironment.COMMIT_ONE_PHASE);
	    
	if (env != null)
	{
	    if (env.compareTo("NO") == 0)
		InitAction.onePhase = false;
	}

	env = PropertyManager.getProperty(ArjunaCoreEnvironment.READONLY_OPTIMISATION);
	    
	if (env != null)
	{
	    if (env.compareTo("NO") == 0)
		InitAction.readonlyOptimisation = false;
	}

	env = PropertyManager.getProperty(ArjunaCoreEnvironment.ENABLE_STATISTICS);
	    
	if (env != null)
	{
	    if (env.compareTo("YES") == 0)
		InitAction.enableStatistics = true;
	}

	env = PropertyManager.getProperty(ArjunaCoreEnvironment.START_DISABLED);
	    
	if (env != null)
	{
	    if (env.compareTo("YES") == 0)
		InitAction.enable = false;
	}

	if (com.arjuna.ArjunaCommon.Core.Lector.confirm(com.arjuna.ArjunaCommon.Core.LicenceType.NUMBER_TRANSACTIONS_100))
	{
	    numberOfTransactions = 100;
	}
	else
	{
	    if (com.arjuna.ArjunaCommon.Core.Lector.confirm(com.arjuna.ArjunaCommon.Core.LicenceType.NUMBER_TRANSACTIONS_1000))
	    {
		numberOfTransactions = 1000;
	    }
	    else
	    {
		if (com.arjuna.ArjunaCommon.Core.Lector.confirm(com.arjuna.ArjunaCommon.Core.LicenceType.NUMBER_TRANSACTIONS_10000))
		{
		    numberOfTransactions = 10000;
		}
		else
		{
		    if (com.arjuna.ArjunaCommon.Core.Lector.confirm(com.arjuna.ArjunaCommon.Core.LicenceType.NUMBER_TRANSACTIONS_100000))
		    {
			numberOfTransactions = 100000;
		    }
		}
	    }
	}
    }

}
