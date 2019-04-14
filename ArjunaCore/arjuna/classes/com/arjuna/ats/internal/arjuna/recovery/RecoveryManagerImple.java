/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryManagerImple.java,v 1.2 2003/08/11 14:07:44 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.recovery ;

import java.io.IOException ;
import java.util.Enumeration ;
import java.util.Hashtable ;
import java.util.Properties ;

import com.arjuna.common.util.propertyservice.PropertyManager;
import com.arjuna.ats.arjuna.common.arjPropertyManager;
import com.arjuna.ats.arjuna.exceptions.FatalError ;
import com.arjuna.ats.arjuna.recovery.RecoveryConfiguration ;
import com.arjuna.ats.internal.arjuna.Implementations ;

import com.arjuna.ats.arjuna.logging.FacilityCode ;
import com.arjuna.ats.arjuna.logging.tsLogger;

import com.arjuna.common.util.logging.*;

/**
 * The RecoveryManagerImple - does the real work.
 * Currently we can have only one of these per node, so
 * each instance checks it's the only one running. If it isn't
 * it will kill itself before doing any work.
 */

public class RecoveryManagerImple
{
    private PeriodicRecovery _periodicRecovery = null ;
    private RecActivatorLoader _recActivatorLoader = null ; 

   /**
    * Does the work of setting up crash recovery. 
    *
    * @message com.arjuna.ats.internal.arjuna.recovery.RecoveryManagerImple_1 [com.arjuna.ats.internal.arjuna.recovery.RecoveryManagerImple_1] -  property io exception {0}
    */

   public RecoveryManagerImple ()
   {

      String rmPropertyFile = RecoveryConfiguration.recoveryManagerPropertiesFile() ;

      try
      {
         arjPropertyManager.propertyManager.addPropertiesFile(rmPropertyFile);

         arjPropertyManager.propertyManager.loadProperties(true);
      }
      catch ( IOException ex )
      {
	  if (tsLogger.arjLoggerI18N.isWarnEnabled())
	  {
	      tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.recovery.RecoveryManagerImple_1",
					  new Object[]{ex});
	  }
      }

      // force normal recovery trace on
      tsLogger.arjLogger.mergeFacilityCode(FacilityCode.FAC_RECOVERY_NORMAL);
      tsLogger.arjLoggerI18N.mergeFacilityCode(FacilityCode.FAC_RECOVERY_NORMAL);

      // this next would force debugging on, but separate recovery mgr file makes this
      // unnecessary
  
       Implementations.initialise() ;
      /*
       * Check whether there is a recovery daemon running - only allow
       * one per machine (currently!)
       */

      if ( activeRecoveryManager() )
      {
         throw new FatalError("Recovery manager already active!");
      }

      // start the expiry scanners

      ExpiredEntryMonitor.startUp();


      //start the activator recovery loader

      _recActivatorLoader = new RecActivatorLoader();

      
      // start the periodic recovery thread
      //  (don't start this until just about to go on the other stuff)

      _periodicRecovery = new PeriodicRecovery();

 
      if (tsLogger.arjLogger.isInfoEnabled())
      {
	  tsLogger.arjLogger.info("RecoveryManagerImple is ready" );
      }
   
   }

   public void stop ()
   {
      _periodicRecovery.shutdown();
      //ExpiredEntryMonitor.shutdown();
   }
    
   public void finalize ()
   {
      stop();
   }
    
   private final boolean activeRecoveryManager ()
   {
      return false ;
   }


}


