/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: FileLockingStore.java,v 1.3 2003/08/11 15:04:18 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.objectstore;

import com.arjuna.ats.arjuna.objectstore.*;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.utils.*;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.gandiva.ObjectName;

import java.io.*;

import com.arjuna.ats.arjuna.exceptions.ObjectStoreException;
import java.lang.NumberFormatException;
import java.io.IOException;
import java.io.FileNotFoundException;

import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.logging.FacilityCode;

import com.arjuna.common.util.logging.*;

/**
 * An refinement of the basic FileSystemStore which provides file-level
 * locking.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: FileLockingStore.java,v 1.3 2003/08/11 15:04:18 nmcl Exp $
 * @since JTS 1.0.
 */

public abstract class FileLockingStore extends FileSystemStore
{

protected abstract InputObjectState read_state (Uid u, String tn, int s) throws ObjectStoreException;
protected abstract boolean remove_state (Uid u, String tn, int s) throws ObjectStoreException;
protected abstract boolean write_state (Uid u, String tn, OutputObjectState buff, int s) throws ObjectStoreException;
    
public FileLockingStore (String locationOfStore, int ss)
    {
	super(locationOfStore, ss);

	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "FileLockingStore.FileLockingStore("+locationOfStore+")");
	}
    }

protected FileLockingStore (ObjectName objName)
    {
	super(objName);

	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
				     FacilityCode.FAC_OBJECT_STORE, "FileLockingStore.FileLockingStore("+objName+")");
	}
    }

    /**
     * Lock files as we would do on a Unix system.
     */
    
protected synchronized boolean lock (File fd, int lmode, boolean create)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
				     FacilityCode.FAC_OBJECT_STORE, 
				     "FileLockingStore.lock("+fd+", "+FileLock.modeString(lmode)+", "+create+")");
	}

	FileLock fileLock = new FileLock(fd);

	return fileLock.lock(lmode, create);
    }

protected synchronized boolean unlock (File fd)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
				     FacilityCode.FAC_OBJECT_STORE, "FileLockingStore.unlock("+fd+")");
	}

	FileLock fileLock = new FileLock(fd);

	return fileLock.unlock();
    }
    
}
