/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: FileLockingStore.java,v 1.1 2000/02/25 14:04:08 nmcl Exp $
 */

package com.arjuna.ArjunaLite.Implementation.ObjectStore;

import com.arjuna.ArjunaLite.Interface.ObjectStore;
import com.arjuna.ArjunaLite.Implementation.ObjectStoreImple;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.*;

import com.arjuna.ArjunaLite.Common.ObjectStoreException;
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
