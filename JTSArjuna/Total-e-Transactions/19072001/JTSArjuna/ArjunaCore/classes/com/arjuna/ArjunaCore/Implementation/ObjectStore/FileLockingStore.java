/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: FileLockingStore.java,v 1.1.4.1.2.1.2.1.2.1 2000/12/21 11:21:09 nmcl Exp $
 */

package com.arjuna.ArjunaCore.Implementation.ObjectStore;

import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Implementation.ObjectStoreImple;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.*;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import java.lang.NumberFormatException;
import java.io.IOException;
import java.io.FileNotFoundException;

/**
 * An refinement of the basic FileSystemStore which provides file-level
 * locking.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: FileLockingStore.java,v 1.1.4.1.2.1.2.1.2.1 2000/12/21 11:21:09 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class FileLockingStore extends FileSystemStore
{

protected abstract InputObjectState read_state (Uid u, String tn, int s) throws ObjectStoreException;
protected abstract boolean remove_state (Uid u, String tn, int s) throws ObjectStoreException;
protected abstract boolean write_state (Uid u, String tn, OutputObjectState buff, int s) throws ObjectStoreException;
    
public FileLockingStore (String locationOfStore)
    {
	super(locationOfStore);

	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "FileLockingStore.FileLockingStore("+locationOfStore+")");
	}
    }

    /**
     * Lock files as we would do on a Unix system.
     */
    
protected synchronized boolean lock (File fd, int lmode, boolean create)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OBJECT_STORE, "FileLockingStore.lock("+fd+", "+FileLock.modeString(lmode)+", "+create+")");
	}

	FileLock fileLock = new FileLock(fd);

	return fileLock.lock(lmode, create);
    }

protected synchronized boolean unlock (File fd)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OBJECT_STORE, "FileLockingStore.unlock("+fd+")");
	}

	FileLock fileLock = new FileLock(fd);

	return fileLock.unlock();
    }
    
}
