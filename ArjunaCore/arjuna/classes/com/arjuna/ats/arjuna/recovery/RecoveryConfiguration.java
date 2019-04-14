/*
 * Copyright (C) 2001
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryConfiguration.javatmpl,v 1.1 2003/06/19 10:50:24 nmcl Exp $
 */

package com.arjuna.ats.arjuna.recovery;

/**
 * This class contains various run-time configuration options. Default
 * values are provided at compile-time, and may be operating system
 * specific.
 *
 */
public class RecoveryConfiguration
{

   /**
    * @return the name of the RecoveryManager properties file to use.
    * @since JTS 2.1.
    */
   public static synchronized final String recoveryManagerPropertiesFile()
   {
      return _rmPropertyFile;
   }

   /**
    * Set the name of the RecoveryManager properties file.
    * @since JTS 2.1.
    */

   public static synchronized final void setRecoveryManagerPropertiesFile (String file)
   {
      _rmPropertyFile = file;
   }

   private static String _rmPropertyFile = "RecoveryManager-properties.xml"  ;

   private RecoveryConfiguration()
   {
      // zero-ton class
   }
}



