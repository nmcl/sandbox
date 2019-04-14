/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryModule.java,v 1.1 2003/06/19 10:50:24 nmcl Exp $
 */

package com.arjuna.ats.arjuna.recovery ;

/**
 * Interface for Recovery manager plug-in module.
 * RecoveryModules are registered via the properties mechanisms.
 * The periodicWorkFirstPass of each module is called, then RecoveryManager
 * waits for the time interval RECOVERY_BACKOFF_PERIOD (seconds), then the
 * periodicWorkSecondPass of each module are called.
 * The RecoveryManager then waits for period PERIODIC_RECOVERY_PERIOD (seconds)
 * before starting the first pass again
 * The backoff period between the first and second pass is intended to allow
 * transactions that were in-flight during the first pass to be completed normally,
 * without requiring the status of each one to be checked. The recovery period 
 * will typically be appreciably longer.
 */

public interface RecoveryModule
{
    /**
     * Called by the RecoveryManager at start up, and then
     * PERIODIC_RECOVERY_PERIOD seconds after the completion, for all RecoveryModules,
     * of the second pass
     */
    public void periodicWorkFirstPass ();
    
    /**
     * Called by the RecoveryManager RECOVERY_BACKOFF_PERIOD seconds
     * after the completion of the first pass
     */
    public void periodicWorkSecondPass ();
}
