/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCImple.java,v 1.1 2003/06/19 10:50:32 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.objectstore;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.objectstore.ObjectStore;
import com.arjuna.ats.arjuna.objectstore.ObjectStoreImple;
import com.arjuna.ats.arjuna.common.*;

import java.io.*;
import java.sql.*;
import java.util.Hashtable;

import com.arjuna.ats.arjuna.exceptions.ObjectStoreException;
import com.arjuna.ats.arjuna.exceptions.ObjectStoreError;
import java.io.IOException;

public abstract class JDBCImple
{

    public abstract boolean commit_state (Uid objUid, String tName) throws ObjectStoreException;
    public abstract boolean hide_state (Uid objUid, String tName) throws ObjectStoreException;
    public abstract boolean reveal_state (Uid objUid, String tName) throws ObjectStoreException;

    public abstract int currentState (Uid objUid, String tName) throws ObjectStoreException;

    public abstract boolean allObjUids (String tName, InputObjectState state, int match) throws ObjectStoreException;
    public abstract boolean allTypes (InputObjectState foundTypes) throws ObjectStoreException;
    public abstract InputObjectState read_state (Uid objUid, String tName, int ft) throws ObjectStoreException;
    public abstract boolean remove_state (Uid objUid, String name, int ft) throws ObjectStoreException;
    public abstract boolean write_state (Uid objUid, String tName, OutputObjectState state, int s) throws ObjectStoreException;

    public abstract boolean initialise (Connection conn, String tableName, boolean dropTable) throws SQLException;

    public abstract String name ();

    //protected abstract boolean exists (String state);

    /**
     * @message com.arjuna.ats.internal.arjuna.objectstore.JDBCImple_1 [com.arjuna.ats.internal.arjuna.objectstore.JDBCImple_1] - JDBCImple.getState caught: {0}
     */
    
    protected final int getState (String state)
    {
	try
	{
	    Integer st = (Integer) stateCache.get(state);
	    
	    if (st != null)
	    {
		return st.intValue();
	    }
	}
	catch (Exception ex)
	{
	    if (tsLogger.arjLoggerI18N.isWarnEnabled())
	    {
		tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.JDBCImple_1",
						new Object[]{ex});
	    }
	}

	return ObjectStore.OS_UNKNOWN;
    }
    
    protected final void addToCache (Uid state, int status)
    {
	if (shareStatus == ObjectStore.OS_UNSHARED)
	{
	    stateCache.put(state, new Integer(status));
	}
    }

    protected final void removeFromCache (String state)
    {
	removeFromCache(state, true);
    }
    
    /**
     * Print a warning if the file to be removed is not in the cache.
     *
     * @message com.arjuna.ats.internal.arjuna.objectstore.JDBCImple_2 [com.arjuna.ats.internal.arjuna.objectstore.JDBCImple_2] - JDBCImple.removeFromCache - no entry for {0}
     */

    protected final void removeFromCache (String state, boolean warn)
    {
	if (shareStatus == ObjectStore.OS_UNSHARED)
	{
	    if ((stateCache.remove(state) == null) && warn)
	    {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		{
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.JDBCImple_2",
						new Object[]{state});
		}
	    }
	}
    }

    final void setShareStatus (int status)
    {
	shareStatus = status;
    }

    private int       shareStatus;
    private Hashtable stateCache = new Hashtable();

}
