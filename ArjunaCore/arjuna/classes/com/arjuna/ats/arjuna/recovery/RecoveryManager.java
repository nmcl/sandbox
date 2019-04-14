/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: RecoveryManager.java,v 1.2 2003/08/11 14:07:38 nmcl Exp $
 */

package com.arjuna.ats.arjuna.recovery;

import java.util.Enumeration;
import java.util.Hashtable;
import java.util.Properties;

import com.arjuna.common.util.propertyservice.PropertyManager;
import com.arjuna.ats.internal.arjuna.recovery.RecoveryManagerImple;

/**
 * The RecoveryManager daemon
 */
public class RecoveryManager
{
   /**
    * Run the RecoveryManager. See Administration manual for details.
    */
   public static void main (String[] args)
   {
      boolean testMode = false;

      for (int i = 0; i < args.length; i++)
      {
         if (args[i].compareTo("-help") == 0)
         {
            System.out.println("Usage: com.arjuna.ats.arjuna.recovery.RecoveryManager [-help] [-test] [-version]");
            System.exit(0);
         }
         if (args[i].compareTo("-version") == 0)
         {
            System.out.println("Version " + com.arjuna.ats.arjuna.common.Configuration.version());
            System.exit(0);
         }
         if (args[i].compareTo("-test") == 0)
         {
            testMode = true;
         }
      }

      try
      {
         RecoveryManagerImple theImple = new RecoveryManagerImple();

         if (testMode)
            System.out.println("Ready");
      }
      catch (Throwable e)
      {
         e.printStackTrace();
      }
   }

   /** not used */
   private RecoveryManager()
   {
   }
}
