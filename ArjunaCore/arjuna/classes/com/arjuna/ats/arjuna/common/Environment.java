/*
 * Copyright (C) 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Environment.java,v 1.2 2003/06/26 10:03:44 nmcl Exp $
 */

package com.arjuna.ats.arjuna.common;

/**
 * The various property variables that can be set at
 * runtime to configure the some of the classes within
 * the package.
 *
 * The various values are:
 * <ul>
 * <li> STATIC_INVENTORY_IMPLE = com.arjuna.ats.internal.arjuna.inventory.staticInventoryImple
 * <li> VAR_DIR = com.arjuna.ats.arjuna.common.varDir
 * <li> ACTION_STORE= com.arjuna.ats.arjuna.coordinator.actionStore
 * <li> ASYNC_COMMIT = com.arjuna.ats.arjuna.coordinator.asyncCommit
 * <li> ASYNC_PREPARE = com.arjuna.ats.arjuna.coordinator.asyncPrepare
 * <li> COMMIT_ONE_PHASE = com.arjuna.ats.arjuna.coordinator.commitOnePhase
 * <li> LOCALOSROOT = com.arjuna.ats.arjuna.objectstore.localOSRoot
 * <li> MAINTAIN_HEURISTICS = com.arjuna.ats.arjuna.coordinator.maintainHeuristics
 * <li> OBJECTSTORE_DIR = com.arjuna.ats.arjuna.objectstore.objectStoreDir
 * <li> OBJECTSTORE_SYNC = com.arjuna.ats.arjuna.objectstore.objectStoreSync
 * <li> OBJECTSTORE_TYPE = com.arjuna.ats.arjuna.objectstore.objectStoreType
 * <li> HASHED_DIRECTORIES = com.arjuna.ats.arjuna.objectstore.hashedDirectories
 * <li> TRANSACTION_LOG = com.arjuna.ats.arjuna.coordinator.transactionLog
 * <li> TRANSACTION_SYNC = com.arjuna.ats.arjuna.objectstore.transactionSync
 * <li> READONLY_OPTIMISATION = com.arjuna.ats.arjuna.coordinator.readonlyOptimisation
 * <li> CLASSIC_PREPARE = com.arjuna.ats.arjuna.coordinator.classicPrepare
 * <li> ENABLE_STATISTICS = com.arjuna.ats.arjuna.coordinator.enableStatistics
 * <li> SHARED_TRANSACTION_LOG = com.arjuna.ats.arjuna.coordinator.sharedTransactionLog
 * <li> START_DISABLED = com.arjuna.ats.arjuna.coordinator.startDisabled
 * <li> JDBC_USER_DB_ACCESS = com.arjuna.ats.arjuna.objectstore.jdbcUserDbAccess
 * <li> JDBC_TX_DB_ACCESS = com.arjuna.ats.arjuna.objectstore.jdbcTxDbAccess
 * <li> LICENCE = com.arjuna.ats.arjuna.licence
 * <li> PERIODIC_RECOVERY_PERIOD = com.arjuna.ats.arjuna.recovery.periodicRecoveryPeriod
 * <li> RECOVERY_BACKOFF_PERIOD = com.arjuna.ats.arjuna.recovery.recoveryBackoffPeriod
 * <li> TX_REAPER_MODE = com.arjuna.ats.arjuna.coordinator.txReaperMode
 * <li> TX_REAPER_TIMEOUT = com.arjuna.ats.arjuna.coordinator.txReaperTimeout
 * <li> OBJECTSTORE_SHARE = com.arjuna.ats.arjuna.objectstore.share
 * </ul>
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: Environment.java,v 1.2 2003/06/26 10:03:44 nmcl Exp $
 * @since 1.0.
 */

public class Environment
{
    public static final String PROPERTIES_FILE = "com.arjuna.ats.arjuna.common.propertiesFile";
    public static final String STATIC_INVENTORY_IMPLE = "com.arjuna.ats.internal.arjuna.inventory.staticInventoryImple";
    public static final String VAR_DIR = "com.arjuna.ats.arjuna.common.varDir";
    
    public static final String ACTION_STORE= "com.arjuna.ats.arjuna.coordinator.actionStore";
    public static final String ASYNC_COMMIT = "com.arjuna.ats.arjuna.coordinator.asyncCommit";
    public static final String ASYNC_PREPARE = "com.arjuna.ats.arjuna.coordinator.asyncPrepare";
    public static final String COMMIT_ONE_PHASE = "com.arjuna.ats.arjuna.coordinator.commitOnePhase";
    public static final String LOCALOSROOT = "com.arjuna.ats.arjuna.objectstore.localOSRoot";
    public static final String MAINTAIN_HEURISTICS = "com.arjuna.ats.arjuna.coordinator.maintainHeuristics";
    public static final String OBJECTSTORE_DIR = "com.arjuna.ats.arjuna.objectstore.objectStoreDir";
    public static final String OBJECTSTORE_SYNC = "com.arjuna.ats.arjuna.objectstore.objectStoreSync";
    public static final String OBJECTSTORE_TYPE = "com.arjuna.ats.arjuna.objectstore.objectStoreType";
    public static final String HASHED_DIRECTORIES = "com.arjuna.ats.arjuna.objectstore.hashedDirectories";
    public static final String TRANSACTION_LOG = "com.arjuna.ats.arjuna.coordinator.transactionLog";
    public static final String TRANSACTION_SYNC = "com.arjuna.ats.arjuna.objectstore.transactionSync";
    public static final String READONLY_OPTIMISATION = "com.arjuna.ats.arjuna.coordinator.readonlyOptimisation";
    public static final String CLASSIC_PREPARE = "com.arjuna.ats.arjuna.coordinator.classicPrepare";
    public static final String ENABLE_STATISTICS = "com.arjuna.ats.arjuna.coordinator.enableStatistics";
    public static final String SHARED_TRANSACTION_LOG = "com.arjuna.ats.arjuna.coordinator.sharedTransactionLog";
    public static final String START_DISABLED = "com.arjuna.ats.arjuna.coordinator.startDisabled";
    public static final String JDBC_USER_DB_ACCESS = "com.arjuna.ats.arjuna.objectstore.jdbcUserDbAccess";
    public static final String JDBC_TX_DB_ACCESS = "com.arjuna.ats.arjuna.objectstore.jdbcTxDbAccess";
    public static final String LICENCE = "com.arjuna.ats.arjuna.licence";
    public static final String CACHE_STORE_SIZE = "com.arjuna.ats.internal.arjuna.objectstore.cacheStore.size";
    public static final String CACHE_STORE_SCAN_PERIOD = "com.arjuna.ats.internal.arjuna.objectstore.cacheStore.period";

    public static final String PERIODIC_RECOVERY_PERIOD = "com.arjuna.ats.arjuna.recovery.periodicRecoveryPeriod" ;
    public static final String RECOVERY_BACKOFF_PERIOD = "com.arjuna.ats.arjuna.recovery.recoveryBackoffPeriod" ;
    public static final String TX_REAPER_MODE = "com.arjuna.ats.arjuna.coordinator.txReaperMode";
    public static final String TX_REAPER_TIMEOUT = "com.arjuna.ats.arjuna.coordinator.txReaperTimeout";
    public static final String OBJECTSTORE_SHARE = "com.arjuna.ats.arjuna.objectstore.share";
    
}

