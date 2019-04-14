package com.hp.mwtests.ts.arjuna.recovery;

/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CrashAbstractRecordSetup.java,v 1.2 2003/06/19 10:51:20 nmcl Exp $
 */

import com.arjuna.ats.arjuna.gandiva.inventory.Inventory ;

public class CrashAbstractRecordSetup
{
   public CrashAbstractRecordSetup()
   {
      if ( !_added )
      {
         Inventory.inventory().addToList( new UserDefFirst0Setup() ) ;
         _added = true ;
      }
   }
   
   private static boolean _added = false ;
}

