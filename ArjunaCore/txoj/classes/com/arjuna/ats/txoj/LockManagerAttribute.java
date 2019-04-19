/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockManagerAttribute.java,v 1.1 2003/06/19 11:56:10 nmcl Exp $
 */

package com.arjuna.ats.txoj;

import com.arjuna.ats.arjuna.gandiva.ClassName;

/**
 * Instances of this class may be passed to a LockManager object at
 * construction time and used to configure it.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: LockManagerAttribute.java,v 1.1 2003/06/19 11:56:10 nmcl Exp $
 * @since JTS 1.0.
 */

public class LockManagerAttribute
{

public LockManagerAttribute ()
    {
	remoteLockStore = false;
	lockStoreType = null;
    }

public boolean   remoteLockStore;
public ClassName lockStoreType;
    
};
