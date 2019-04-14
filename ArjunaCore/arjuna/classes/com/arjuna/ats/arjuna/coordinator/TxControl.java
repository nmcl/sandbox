/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TxControl.java,v 1.2 2003/08/11 14:07:37 nmcl Exp $
 */

package com.arjuna.ats.arjuna.coordinator;

import com.arjuna.ats.arjuna.gandiva.ClassName;
import com.arjuna.ats.arjuna.common.Environment;
import com.arjuna.ats.arjuna.common.arjPropertyManager;
import com.arjuna.ats.arjuna.objectstore.ObjectStore;
import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.recovery.TransactionStatusManager;

import com.arjuna.common.util.propertyservice.PropertyManager;


/*
 * Transaction configuration object. We now have a separate object for
 * this so that other classes can enquire of (and use) this information.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: TxControl.java,v 1.2 2003/08/11 14:07:37 nmcl Exp $
 * @since JTS 2.2.
 */

public class TxControl
{

    public static final void enable ()
    {
	TxControl.enable = true;
    }
    
    public static final void disable ()
    {
	/*
	 * We could have an implementation that did not return until
	 * all transactions had finished. However, this could take
	 * an arbitrary time, especially if participants could fail.
	 * Since this information is available anyway to the application,
	 * let it handle it.
	 */

	TxControl.enable = false;
    }

    public static final boolean isEnabled ()
    {
	return TxControl.enable;
    }

    public static final ClassName getActionStoreType ()
    {
	return actionStoreType;
    }
    
    /**
     * @return the <code>ObjectStore</code> implementation which the
     * transaction coordinator will use.
     * @see com.arjuna.ats.arjuna.objectstore.ObjectStore
     */

    public static final ObjectStore getStore ()
    {
	/*
	 * Check for action store once per application. The second
	 * parameter is the default value, which is returned if no
	 * other value is specified.
	 */
	    
	if (TxControl.actionStoreType == null)
	{
	    String useLog = arjPropertyManager.propertyManager.getProperty(Environment.TRANSACTION_LOG, "OFF");

	    if (useLog.equals("ON"))
		TxControl.actionStoreType = new ClassName(ArjunaNames.Implementation_ObjectStore_ActionLogStore());
	    else
		TxControl.actionStoreType = new ClassName(arjPropertyManager.propertyManager.getProperty(Environment.ACTION_STORE, ArjunaNames.Implementation_ObjectStore_defaultActionStore().stringForm()));

	    String sharedLog = arjPropertyManager.propertyManager.getProperty(Environment.SHARED_TRANSACTION_LOG, "NO");
	    
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

    public static final boolean getAsyncPrepare ()
    {
	return asyncPrepare;
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
    static TransactionStatusManager transactionStatusManager = null;
    static ClassName actionStoreType = null;
    
    static
    {
	String env = arjPropertyManager.propertyManager.getProperty(Environment.MAINTAIN_HEURISTICS);
	
	if (env != null)
	{
	    if (env.compareTo("YES") == 0)
		TxControl.maintainHeuristics = true;
	}

	env = arjPropertyManager.propertyManager.getProperty(Environment.ASYNC_COMMIT);
	    
	if (env != null)
	{
	    if (env.compareTo("YES") == 0)
		TxControl.asyncCommit = true;
	}

	env = arjPropertyManager.propertyManager.getProperty(Environment.ASYNC_PREPARE);
	    
	if (env != null)
	{
	    if (env.compareTo("YES") == 0)
		TxControl.asyncPrepare = true;
	}

	env = arjPropertyManager.propertyManager.getProperty(Environment.COMMIT_ONE_PHASE);
	    
	if (env != null)
	{
	    if (env.compareTo("NO") == 0)
		TxControl.onePhase = false;
	}

	env = arjPropertyManager.propertyManager.getProperty(Environment.READONLY_OPTIMISATION);
	    
	if (env != null)
	{
	    if (env.compareTo("NO") == 0)
		TxControl.readonlyOptimisation = false;
	}

	env = arjPropertyManager.propertyManager.getProperty(Environment.ENABLE_STATISTICS);
	    
	if (env != null)
	{
	    if (env.compareTo("YES") == 0)
		TxControl.enableStatistics = true;
	}

	env = arjPropertyManager.propertyManager.getProperty(Environment.START_DISABLED);
	    
	if (env != null)
	{
	    if (env.compareTo("YES") == 0)
		TxControl.enable = false;
	}

	if (com.arjuna.ats.internal.arjuna.common.Lector.confirm(com.arjuna.ats.internal.arjuna.common.LicenceType.NUMBER_TRANSACTIONS_100))
	{
	    numberOfTransactions = 100;
	}
	else
	{
	    if (com.arjuna.ats.internal.arjuna.common.Lector.confirm(com.arjuna.ats.internal.arjuna.common.LicenceType.NUMBER_TRANSACTIONS_1000))
	    {
		numberOfTransactions = 1000;
	    }
	    else
	    {
		if (com.arjuna.ats.internal.arjuna.common.Lector.confirm(com.arjuna.ats.internal.arjuna.common.LicenceType.NUMBER_TRANSACTIONS_10000))
		{
		    numberOfTransactions = 10000;
		}
		else
		{
		    if (com.arjuna.ats.internal.arjuna.common.Lector.confirm(com.arjuna.ats.internal.arjuna.common.LicenceType.NUMBER_TRANSACTIONS_100000))
		    {
			numberOfTransactions = 100000;
		    }
		}
	    }
	}
        
	if ( transactionStatusManager == null )
	{
	    transactionStatusManager = new TransactionStatusManager();
            
            // add hook to ensure finalize gets called.
            Runtime.getRuntime().addShutdownHook(new Thread()
            {
                public void run()
                {
                    if ( transactionStatusManager != null )
                    {
                       transactionStatusManager.finalize() ;
                    }
                }
            }) ;
        }
    }

}
