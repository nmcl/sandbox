/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockStoreImple.java,v 1.1 2000/02/25 14:04:02 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Implementation;

import com.arjuna.ArjunaCommon.Common.*;

import com.arjuna.ArjunaLite.Common.LockStoreException;

public abstract class LockStoreImple
{

public abstract InputObjectState read_state (Uid u, String tName) throws LockStoreException;
public abstract boolean remove_state (Uid u, String tname);
public abstract boolean write_committed (Uid u, String tName, OutputObjectState state);

public ClassName className ()
    {
	return LockStoreImple._className;
    }

public static ClassName name ()
    {
	return LockStoreImple._className;
    }    

private static final ClassName _className = new ClassName("LockStoreImple");
    
};
