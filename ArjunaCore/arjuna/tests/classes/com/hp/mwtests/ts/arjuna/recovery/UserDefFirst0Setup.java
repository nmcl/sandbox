package com.hp.mwtests.ts.arjuna.recovery;

/*
 * Copyright (C) 1999-2001 by HP Bluestone Software, Inc. All rights Reserved.
 *
 * HP Arjuna Labs,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: UserDefFirst0Setup.java,v 1.2 2003/06/19 10:51:20 nmcl Exp $
 */

import com.arjuna.ats.arjuna.coordinator.RecordType ;
import com.arjuna.ats.arjuna.gandiva.ClassName ;
import com.arjuna.ats.arjuna.gandiva.ObjectName ;
import com.arjuna.ats.arjuna.gandiva.inventory.InventoryElement ;

public class UserDefFirst0Setup implements InventoryElement
{
   public UserDefFirst0Setup ()
   {
   }

   public synchronized Object createVoid ()
   {
      return CrashAbstractRecordImpl.create();
   }

   public synchronized Object createClassName (ClassName className)
   {
      return null;
   }

   public synchronized Object createObjectName (ObjectName objectName)
   {
      return null;
   }

   public synchronized Object createResources (Object[] resources)
   {
      return null;
   }

   public synchronized Object createClassNameResources (ClassName className, Object[] resources)
   {
      return null;
   }

   public synchronized Object createObjectNameResources (ObjectName objectName, Object[] resources)
   {
      return null;
   }

   public ClassName className ()
   {
       return new ClassName("RecordType.USER_DEF_FIRST0");
   }
}
