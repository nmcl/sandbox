/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: LockStoreImple.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:08 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation;

import com.arjuna.ArjunaCommon.Common.*;

import com.arjuna.ArjunaCore.Common.LockStoreException;

/**
 * Implementations of lock stores should derive from
 * this abstract class and provide implementations of the
 * appropriate methods.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: LockStoreImple.java,v 1.1.4.1.2.1.4.1 2000/12/21 11:21:08 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class LockStoreImple
{

    /**
     * Read a state from the lock store.
     */

public abstract InputObjectState read_state (Uid u, String tName) throws LockStoreException;

    /**
     * Remove the state from the lock store.
     */

public abstract boolean remove_state (Uid u, String tname);

    /**
     * Write the state to the lock store.
     */

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
    
}
