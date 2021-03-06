/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockStoreImple.java,v 1.2 1998/07/06 13:26:37 nmcl Exp $
 */

package com.arjuna.JavaArjuna.LockStore;

import com.arjuna.JavaArjuna.Common.*;

import com.arjuna.JavaArjuna.Common.LockStoreException;

public abstract class LockStoreImple
{

public abstract InputObjectState read_state (Uid u, String tName) throws LockStoreException;
public abstract boolean remove_state (Uid u, String tname);
public abstract boolean write_committed (Uid u, String tName, OutputObjectState state);

public String className ()
    {
	return LockStoreImple._className;
    }

public static String name ()
    {
	return LockStoreImple._className;
    }    

private static final String _className = "LockStoreImple";

};
