/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: TxOJNames.java,v 1.1 2003/06/19 11:56:11 nmcl Exp $
 */

package com.arjuna.ats.txoj;

import com.arjuna.ats.arjuna.gandiva.ClassName;
import com.arjuna.ats.arjuna.coordinator.RecordType;

/**
 * This class contains the ClassNames and ObjectName attributes that
 * may be used by implementations within this module.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: TxOJNames.java,v 1.1 2003/06/19 11:56:11 nmcl Exp $
 * @since JTS 1.0.
 * @see com.arjuna.ats.arjuna.gandiva.ClassName
 * @see com.arjuna.ats.arjuna.gandiva.ObjectName
 */

public class TxOJNames
{

public static ClassName Implementation_LockStore_defaultStore ()
    {
	return TxOJNames.Implementation_LockStore_BasicLockStore();
    }
    
public static ClassName Implementation_LockStore_BasicLockStore ()
    {
	return new ClassName("BasicLockStore");
    }

public static ClassName Implementation_LockStore_BasicPersistentLockStore ()
    {
	return new ClassName("BasicPersistentLockStore");
    }

public static ClassName Implementation_Semaphore_BasicSemaphore ()
    {
	return new ClassName("BasicSemaphore");
    }

};
