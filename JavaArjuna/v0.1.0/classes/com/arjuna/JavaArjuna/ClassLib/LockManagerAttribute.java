/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockManagerAttribute.java,v 1.2 1998/07/06 13:26:23 nmcl Exp $
 */

package com.arjuna.JavaArjuna.ClassLib;

public class LockManagerAttribute extends StateManagerAttribute
{

public LockManagerAttribute ()
    {
	remoteLockStore = false;
    }

public boolean remoteLockStore;
    
};
