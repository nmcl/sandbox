/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: PeriodicRecovery.java,v 1.4 2003/08/11 15:04:20 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.recovery ;

import java.lang.InterruptedException ;
import java.text.SimpleDateFormat ;
import java.util.Date ;
import java.util.Enumeration ;
import java.util.Properties ;
import java.util.Vector ;

import com.arjuna.common.util.propertyservice.PropertyManager;
import com.arjuna.ats.arjuna.recovery.RecoveryModule ;
import com.arjuna.ats.arjuna.recovery.RecoveryEnvironment ;
import com.arjuna.ats.arjuna.common.arjPropertyManager;

import com.arjuna.ats.arjuna.logging.FacilityCode ;
import com.arjuna.ats.arjuna.logging.tsLogger;

import com.arjuna.common.util.logging.*;

/**
 * Threaded object to perform the periodic recovery. Instantiated in
 * the RecoveryManager. The work is actually completed by the recovery
 * modules. These modules are dynamically loaded. The modules to load
 * are specified by properties beginning with "RecoveryExtension"
 * <P>
 * @author 
 * @version $Id: PeriodicRecovery.java,v 1.4 2003/08/11 15:04:20 nmcl Exp $
 */

/**
 * @message com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_1 [com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_1] - Attempt to load recovery module with null class name!
 * @message com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_2 [com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_2] - Recovery module {0} does not conform to RecoveryModule interface
 * @message com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_3 [com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_3] - Loading recovery module: {0}
 * @message com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_4 [com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_4] - Loading recovery module: {0}
 * @message com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_5 [com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_5] - Loading recovery module: could not find class {0}
 * @message com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_6 [com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_6] - {0} has inappropriate value ( {1} )
 * @message com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_7 [com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_7] - {0} has inappropriate value ( {1} )
*/

public class PeriodicRecovery extends Thread
{

   public PeriodicRecovery()
   {
      initialise();

      // Load the recovery modules that actually do the work.
      loadModules();

      start();
   }

   public void shutdown ()
   {
      _terminate = true;

      this.interrupt();
   }

   /**
    * Start the background thread to perform the periodic recovery
    */
   public void run()
   {
      while(true)
      {
	 
	  tsLogger.arjLogger.info("\n  --- Periodic recovery - first pass ----" + 
				   _theTimestamper.format(new Date()) + "----" );
	  
	  Enumeration modules = _recoveryModules.elements();
         
         while (modules.hasMoreElements())
         {
            RecoveryModule m = (RecoveryModule) modules.nextElement();
            m.periodicWorkFirstPass();
            
    	    if (tsLogger.arjLogger.isDebugEnabled())
	    {
               tsLogger.arjLogger.debug( DebugLevel.FUNCTIONS,
					 VisibilityLevel.VIS_PUBLIC,
					 FacilityCode.FAC_CRASH_RECOVERY,
					 " " );
	    }
	 }

         // wait for a bit to avoid catching (too many) transactions etc. that
         // are really progressing quite happily
         try
         {
            Thread.sleep( _backoffPeriod * 1000 ) ;
         }
         catch ( InterruptedException ie )
         {
            break;
         }

         if ( _terminate )
         {
            return ;
         }

   	 	 
	 tsLogger.arjLogger.info("\n  --- Periodic recovery - second pass ----" +
				 _theTimestamper.format(new Date()) + "----" ) ;
         
         modules = _recoveryModules.elements();
         
         while (modules.hasMoreElements())
         {
            RecoveryModule m = (RecoveryModule) modules.nextElement() ;
            
            m.periodicWorkSecondPass() ;
            
   	    if (tsLogger.arjLogger.isDebugEnabled())
	    {
		tsLogger.arjLogger.debug
		    ( DebugLevel.FUNCTIONS, 
		      VisibilityLevel.VIS_PUBLIC,
		      FacilityCode.FAC_CRASH_RECOVERY,
		      " " ) ;
	    }
	 }

         try
         {
            Thread.sleep( _recoveryPeriod * 1000 ) ;
         }
         catch ( InterruptedException ie )
         {
            break;
         }

         if ( _terminate )
         {
            return ;
         }
      }
   }

   private static void loadModules ()
   {
      // scan the relevant properties so as to get them into sort order
       Properties properties = arjPropertyManager.propertyManager.getProperties();

      if (properties != null)
      {
         Vector moduleNames = new Vector();
         Enumeration names = properties.propertyNames();

         while (names.hasMoreElements())
         {
            String attrName = (String) names.nextElement();

            if (attrName.startsWith(RecoveryEnvironment.MODULE_PROPERTY_PREFIX) )
            {
               // this is one of ours - put it in the right place
               int position = 0;
               while ( position < moduleNames.size() &&
                       attrName.compareTo( moduleNames.elementAt(position)) > 0 )
               {
                  position++;
               }
               moduleNames.add(position,attrName);
            }
         }
         // now go through again and load them
         names = moduleNames.elements();

         while (names.hasMoreElements())
         {
            String attrName = (String) names.nextElement();
            loadModule(properties.getProperty(attrName));
         }

      }
   }

   private static void loadModule (String className)
   {
       if (tsLogger.arjLogger.isDebugEnabled())
       {
         tsLogger.arjLogger.debug( DebugLevel.FUNCTIONS,
				   VisibilityLevel.VIS_PRIVATE,
				   FacilityCode.FAC_CRASH_RECOVERY,
				   "Loading recovery module "+
				   className ) ;
       }
       
      if (className == null)
      {
  	  if (tsLogger.arjLoggerI18N.isWarnEnabled())
	      tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_1");

         return;
      }
      else
      {
         try
         {
	     Class c = PeriodicRecovery.class.getClassLoader().loadClass( className );

            try
            {
               RecoveryModule m = (RecoveryModule) c.newInstance() ;
               _recoveryModules.add( m );
            }
            catch (ClassCastException e)
            {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		{
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_2",
						new Object[]{className});
		}
            }
            catch (IllegalAccessException iae)
            {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		{
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_3",
						new Object[]{iae});
		}
            }
            catch (InstantiationException ie)
            {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		{
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_4",
						new Object[]{ie});
		}
            }

            c = null;
         }
         catch ( ClassNotFoundException cnfe )
         {
 	     if (tsLogger.arjLoggerI18N.isWarnEnabled())
	     {
		 tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_5",
					     new Object[]{className});
	     }
         }
      }
   }

   private final void initialise ()
   {
      _recoveryModules = new Vector();
      _terminate = false;
   }


   // this refers to the modules specified in the recovery manager
   // property file which are dynamically loaded.
   private static Vector _recoveryModules = null ;

   // back off period is the time between the first and second pass.
   // recovery period is the time between the second pass and the start
   // of the first pass.   
   private static int _backoffPeriod = 0;
   private static int _recoveryPeriod = 0 ;
   
   // default values for the above
   private static final int _defaultBackoffPeriod = 10;
   private static final int _defaultRecoveryPeriod = 120;

   // exit thread flag
   private static boolean _terminate = false;
   
   private static SimpleDateFormat 
      _theTimestamper = new SimpleDateFormat("EEE, d MMM yyyy HH:mm:ss");

   /*
    * Read the system properties to set the configurable options
    *
    * Note: if we start and stop the service then changes to the timeouts
    * won't be reflected. We will need to modify this eventually.
    */

   static
   {
      _recoveryPeriod = _defaultRecoveryPeriod;

      String recoveryPeriodString = 
         arjPropertyManager.propertyManager.getProperty(com.arjuna.ats.arjuna.common.Environment.PERIODIC_RECOVERY_PERIOD );

      if ( recoveryPeriodString != null )
      {
         try
         {
            Integer recoveryPeriodInteger = new Integer( recoveryPeriodString );
            _recoveryPeriod = recoveryPeriodInteger.intValue() ;

	    if (tsLogger.arjLogger.isDebugEnabled())
	    {
               tsLogger.arjLogger.debug
                  ( DebugLevel.FUNCTIONS, 
                    VisibilityLevel.VIS_PRIVATE,
                    FacilityCode.FAC_CRASH_RECOVERY,
                    "com.arjuna.ats.arjuna.recovery.PeriodicRecovery" +
                    ": Recovery period set to " + _recoveryPeriod + " seconds" ) ;
	    }
         }
         catch (NumberFormatException e)
         {
	     if (tsLogger.arjLoggerI18N.isWarnEnabled())
	     {
		 tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_6",
					     new Object[]{com.arjuna.ats.arjuna.common.Environment.PERIODIC_RECOVERY_PERIOD, recoveryPeriodString});
	     }
         }
      }

      _backoffPeriod = _defaultBackoffPeriod;

      String backoffPeriodString=
         arjPropertyManager.propertyManager.getProperty(com.arjuna.ats.arjuna.common.Environment.RECOVERY_BACKOFF_PERIOD);


      if (backoffPeriodString != null)
      {
         try
         {
            Integer backoffPeriodInteger = new Integer(backoffPeriodString) ;
            _backoffPeriod = backoffPeriodInteger.intValue() ;

	    if (tsLogger.arjLogger.isDebugEnabled())
	    {
               tsLogger.arjLogger.debug
                  ( DebugLevel.FUNCTIONS, 
                    VisibilityLevel.VIS_PRIVATE,
                    FacilityCode.FAC_CRASH_RECOVERY,
                    "PeriodicRecovery" +
                    ": Backoff period set to " + _backoffPeriod + " seconds" ) ;
	    }
         }
         catch (NumberFormatException e)
         {
     	     if (tsLogger.arjLoggerI18N.isWarnEnabled())
	     {
		 tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.recovery.PeriodicRecovery_7",
					     new Object[]{com.arjuna.ats.arjuna.common.Environment.RECOVERY_BACKOFF_PERIOD, backoffPeriodString});
	     }
         }
      }
   }

 

}








