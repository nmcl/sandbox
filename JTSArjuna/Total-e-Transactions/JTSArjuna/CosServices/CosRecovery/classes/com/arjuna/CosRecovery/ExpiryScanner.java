/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ExpiryScanner.java,v 1.1.2.3 2001/02/15 21:41:54 pfurniss Exp $
 */

package com.arjuna.CosRecovery;

/**
 * Interface for Expiry scanner plug-ins.
 * ExpiryScanners check for ancient entries in the ObjectStore (or elsewhere) to avoid
 * leaving long-dead and irrelevant entries lying around for ever.
 * The scan() method of each registered ExpiryScanner implementation is called 
 * by the RecoveryManager at an interval EXPIRY_SCAN_INTERVAL (hours).
 */

public interface ExpiryScanner
{
    /**
     * perform a scan
     */
    public void scan ();
    /**
     * Is this scanner to be used.  (E.g. if zero age means "don't remove", and it
     * has been set to zero, toBeUsed replies false)
     */
    public boolean toBeUsed();
};
