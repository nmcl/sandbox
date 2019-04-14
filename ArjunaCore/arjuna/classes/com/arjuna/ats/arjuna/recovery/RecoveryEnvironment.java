/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryEnvironment.java,v 1.1 2003/06/19 10:50:24 nmcl Exp $
 */

package com.arjuna.ats.arjuna.recovery ;

/**
 * Container for property names used by recovery
 */
public class RecoveryEnvironment
{
    public static final String MODULE_PROPERTY_PREFIX  = "com.arjuna.ats.arjuna.recovery.recoveryExtension" ;
    public static final String SCANNER_PROPERTY_PREFIX = "com.arjuna.ats.arjuna.recovery.expiryScanner" ;
    public static final String EXPIRY_SCAN_INTERVAL    = "com.arjuna.ats.arjuna.recovery.expiryScanInterval" ;
    public static final String TRANSACTION_STATUS_MANAGER_EXPIRY_TIME = 
	"com.arjuna.ats.arjuna.recovery.transactionStatusManagerExpiryTime";
    public static final String ACTIVATOR_PROPERTY_PREFIX  = "com.arjuna.ats.arjuna.recovery.recoveryActivator" ;
    
   /** Not used */

   private RecoveryEnvironment()
   {
   }
}
