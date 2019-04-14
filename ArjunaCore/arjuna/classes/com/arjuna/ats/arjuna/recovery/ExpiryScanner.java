/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: ExpiryScanner.java,v 1.1 2003/06/19 10:50:24 nmcl Exp $
 */

package com.arjuna.ats.arjuna.recovery ;

/**
 * Interface for Expiry scanner plug-ins.
 * ExpiryScanners check for ancient entries in the ObjectStore (or elsewhere) to 
 * avoid leaving long-dead and irrelevant entries lying around for ever.
 * The scan() method of each registered ExpiryScanner implementation is called 
 * by the RecoveryManager at an interval expiryScanInterval (hours).
 */

public interface ExpiryScanner
{
   /**
    * perform a scan
    */
   public void scan() ;

   /**
    * Is this scanner to be used.  (E.g. if zero age means "don't remove", and it
    * has been set to zero, toBeUsed replies false)
    */
   public boolean toBeUsed() ;
}
