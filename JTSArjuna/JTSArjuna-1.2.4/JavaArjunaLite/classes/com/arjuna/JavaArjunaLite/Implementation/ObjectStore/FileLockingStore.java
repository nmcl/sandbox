/*
 * Copyright (C) 1994, 1995, 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FileLockingStore.java,v 1.4 1998/07/06 13:26:44 nmcl Exp $
 */

package com.arjuna.JavaArjunaLite.Implementation.ObjectStore;

import com.arjuna.JavaArjunaLite.Interface.ObjectStore;
import com.arjuna.JavaArjunaLite.Implementation.ObjectStoreImple;
import com.arjuna.JavaGandiva.Common.*;
import java.io.*;

import com.arjuna.JavaArjunaLite.Common.ObjectStoreException;
import java.lang.NumberFormatException;
import java.io.IOException;
import java.io.FileNotFoundException;

public abstract class FileLockingStore extends FileSystemStore
{

protected abstract InputObjectState read_state (Uid u, String tn, int s) throws ObjectStoreException;
protected abstract boolean remove_state (Uid u, String tn, int s) throws ObjectStoreException;
protected abstract boolean write_state (Uid u, String tn, OutputObjectState buff, int s) throws ObjectStoreException;
    
public FileLockingStore (String locationOfStore)
    {
	super(locationOfStore);
    }

    /*
     * Lock files as we would do on a Unix system.
     */
    
protected synchronized boolean lock (File fd, int lmode, boolean create)
    {
	FileLock fileLock = new FileLock(fd);

	return fileLock.lock(lmode, create);
    }

protected synchronized boolean unlock (File fd)
    {
	FileLock fileLock = new FileLock(fd);

	return fileLock.unlock();
    }
    
}
