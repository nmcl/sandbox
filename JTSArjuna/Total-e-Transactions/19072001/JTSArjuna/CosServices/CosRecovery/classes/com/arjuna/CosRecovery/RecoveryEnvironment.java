/*
 * Copyright (C) 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryEnvironment.java,v 1.1.2.1.4.1.22.1.4.4.2.2 2001/02/15 21:42:12 pfurniss Exp $
 */

package com.arjuna.CosRecovery;

/**
 * Container for property names used by recovery
 */
public class RecoveryEnvironment
{
    public static final String PERIODIC_RECOVERY_PERIOD = "PERIODIC_RECOVERY_PERIOD";
    public static final String RECOVERY_BACKOFF_PERIOD = "RECOVERY_BACKOFF_PERIOD";
    public static final String TRANSACTIONAL_OBJECT_RECOVERY_BACKOFF_PERIOD = "TRANSACTIONAL_OBJECT_RECOVERY_BACKOFF_PERIOD";
    public static final String OTS_ISSUE_RECOVERY_ROLLBACK="OTS_ISSUE_RECOVERY_ROLLBACK";
    public static final String ADMINSERVICENAME = "RecoveryAdmin" ;
    public static final String RECOVERY_MANAGER_TAG="RecoveryManagerTag";
    public static final String COMMITTED_TRANSACTION_RETRY_LIMIT="COMMITTED_TRANSACTION_RETRY_LIMIT";
    public static final String FACTORY_CONTACT_EXPIRY_TIME="FACTORY_CONTACT_EXPIRY_TIME"; 
    public static final String ASSUMED_COMPLETE_EXPIRY_TIME="ASSUMED_COMPLETE_EXPIRY_TIME";
    public static final String EXPIRY_SCAN_INTERVAL="EXPIRY_SCAN_INTERVAL";
    public static final String SCANNERPROPERTYPREFIX="ExpiryScanner";
    public static final String MODULEPROPERTYPREFIX="RecoveryExtension";
    /** Not used */
    private RecoveryEnvironment()
    {
    }
    
};
