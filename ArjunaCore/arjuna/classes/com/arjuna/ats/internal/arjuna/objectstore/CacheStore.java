/*
 * Copyright (C) 2003,
 *
 * Arjuna Technologies Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: CacheStore.java,v 1.3 2003/08/11 15:04:18 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.objectstore;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.objectstore.ObjectStore;
import com.arjuna.ats.arjuna.objectstore.ObjectStoreType;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.state.*;
import com.arjuna.ats.arjuna.exceptions.ObjectStoreException;

import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.logging.FacilityCode;

import com.arjuna.common.util.logging.DebugLevel;
import com.arjuna.common.util.logging.VisibilityLevel;

import com.arjuna.ats.arjuna.gandiva.ClassName;
import com.arjuna.ats.arjuna.gandiva.ObjectName;
import com.arjuna.ats.arjuna.utils.FileLock;
import com.arjuna.ats.arjuna.utils.Utility;

import java.io.File;

import java.util.LinkedList;

/**
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: CacheStore.java,v 1.3 2003/08/11 15:04:18 nmcl Exp $
 * @since JTS 3.0.
 */

public class CacheStore extends ShadowNoFileLockStore
{

    public void finalize ()
    {
	CacheStore._storeManager.flush();
    }

    public boolean commit_state (Uid objUid, String tName) throws ObjectStoreException
    {
	return CacheStore._storeManager.addWork(this, AsyncStore.COMMIT, objUid, tName, null, -1);
    }
    
    public int typeIs ()
    {
	return ObjectStoreType.CACHED;
    }
    
    public ClassName className ()
    {
	return ArjunaNames.Implementation_ObjectStore_CacheStore();
    }

    public static ClassName name ()
    {
	return ArjunaNames.Implementation_ObjectStore_CacheStore();
    }    

    /*
     * Have to return as a ShadowingStore because of
     * inheritence.
     */
    
    public static ShadowingStore create ()
    {
	return new CacheStore("");
    }

    /**
     * message com.arjuna.ats.internal.arjuna.objectstore.CacheStore_1 [com.arjuna.ats.internal.arjuna.objectstore.CacheStore_1] - CacheStore.create caught: {0}
     */

    public static ShadowingStore create (Object[] param)
    {
	if (param == null)
	    return null;

	String location = (String) param[0];
	Integer shareStatus = (Integer) param[1];
	int ss = ObjectStore.OS_UNSHARED;
	
	if (shareStatus != null)
	{
	    try
	    {
		if (shareStatus.intValue() == ObjectStore.OS_SHARED)
		    ss = ObjectStore.OS_SHARED;
	    }
	    catch (Exception e)
	    {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		{
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.CacheStore_1",
						new Object[]{e});
		}
	    }
	}

	return new CacheStore(location, ss);
    }

    public static ShadowingStore create (ObjectName param)
    {
	if (param == null)
	    return null;
	else
	    return new CacheStore(param);
    }

    protected boolean remove_state (Uid objUid, String name, int ft) throws ObjectStoreException
    {
	boolean res = CacheStore._storeManager.removeState(objUid);

	if (!res) // not in cache
	    return CacheStore._storeManager.addWork(this, AsyncStore.REMOVE, objUid, name, null, ft);
	else
	    return true;
    }
    
    protected boolean write_state (Uid objUid, String tName,
				   OutputObjectState state,
				   int ft) throws ObjectStoreException
    {
	return CacheStore._storeManager.addWork(this, AsyncStore.WRITE, objUid, tName, state, ft);
    }

    /*
     * Before we look at the disk let's look in the state cache first for
     * the state, just in case it hasn't been written out to persistent
     * store yet.
     */

    protected InputObjectState read_state (Uid objUid, String tName,
					   int ft) throws ObjectStoreException
    {
	OutputObjectState state = CacheStore._storeManager.getState(objUid);
	
	if (state == null)  // not in the cache
	    return super.read_state(objUid, tName, ft);
	else
	    return new InputObjectState(state);
    }

    /*
     * The methods that do the real work when the thread gets round to
     * it.
     */

    protected boolean commitState (Uid objUid, String tName) throws ObjectStoreException
    {
	return super.commit_state(objUid, tName);
    }

    protected boolean removeState (Uid objUid, String name, int ft) throws ObjectStoreException
    {
	return super.remove_state(objUid, name, ft);
    }

    protected boolean writeState (Uid objUid, String tName,
				  OutputObjectState state,
				  int ft) throws ObjectStoreException
    {
	return super.write_state(objUid, tName, state, ft);
    }
    
    protected CacheStore (String locationOfStore)
    {
	this(locationOfStore, ObjectStore.OS_UNSHARED);
    }
    
    protected CacheStore (String locationOfStore, int shareStatus)
    {
	super(locationOfStore, shareStatus);

	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
				     FacilityCode.FAC_OBJECT_STORE, "CacheStore.CacheStore("+locationOfStore+")");
	}
    }
    
    protected CacheStore ()
    {
	this(ObjectStore.OS_UNSHARED);
    }
	
    protected CacheStore (int shareStatus)
    {
	super(shareStatus);

	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
				     FacilityCode.FAC_OBJECT_STORE, "CacheStore.CacheStore( "+shareStatus+" )");
	}
    }

    protected CacheStore (ObjectName objName)
    {
	super(objName);
	
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PROTECTED,
				     FacilityCode.FAC_OBJECT_STORE, "CacheStore.CacheStore( "+objName+" )");
	}
    }

    private static AsyncStore _storeManager = null;
    
    static
    {
	_storeManager = new AsyncStore();  // only want one instance

	_storeManager.start();
    }

}

class StoreElement
{

    public StoreElement (CacheStore s, int tow, Uid ou, String tn, OutputObjectState st, int ft)
    {
	store = s;
	typeOfWork = tow;
	objUid = ou;
	tName = tn;
	state = st;
	fileType = ft;
    }
    
    public CacheStore       store = null;
    public int               typeOfWork = -1;
    public Uid               objUid = null;
    public String            tName = null;
    public OutputObjectState state = null;
    public int               fileType = -1;

}

class AsyncStore extends Thread  // keep priority same as app. threads
{

    public static final int COMMIT = 0;
    public static final int REMOVE = 1;
    public static final int WRITE = 2;

    public final void flush ()
    {
	/*
	 * Do it this way because by the time we get here the daemon thread
	 * has been removed by the system.
	 */

	while (_workList.size() > 0)
	    doWork();

	_terminated = true;
    }
    
    public final boolean addWork (CacheStore store, int workType, Uid objUid, String tName, OutputObjectState state, int ft)
    {
	synchronized (_workList)
	{
	    if (state != null)
		_currentCacheSize += state.size();

	    boolean res = _workList.add(new StoreElement(store, workType, objUid, tName, state, ft));

	    if (_currentCacheSize > _maximumCacheSize)
		this.interrupt();

	    return res;
	}
    }

    public final boolean removeState (Uid objUid)
    {
	synchronized (_workList)
	{
	    for (int i = 0; i < _workList.size(); i++)
	    {
		StoreElement element = (StoreElement) _workList.get(i);
		
		if ((element != null) && (element.objUid.equals(objUid)))
		{
		    _workList.remove(element);

		    return true;
		}
	    }
	}

	return false;
    }

    public final OutputObjectState getState (Uid objUid)
    {
	synchronized (_workList)
	{
	    for (int i = 0; i < _workList.size(); i++)
	    {
		StoreElement element = (StoreElement) _workList.get(i);
		
		if ((element != null) && (element.objUid.equals(objUid)))
		    return element.state;
	    }
	}

	return null;
    }
    
    public void run ()
    {
	for (;!_terminated;)
	{
	    // this catches the write operations

	    if (_currentCacheSize >= _maximumCacheSize)
	    {
		while (_currentCacheSize >= _maximumCacheSize)
		{
		    doWork();
		}
	    }
	    else
		doWork();  // clear out removes and commits

	    // only yields if sleeptime is 0

	    try
	    {
		Thread.currentThread().sleep(_sleepTime);
	    }
	    catch (Exception ex)
	    {
	    }
	}
    }

    /**
     * @message com.arjuna.ats.internal.arjuna.objectstore.CacheStore_1 [com.arjuna.ats.internal.arjuna.objectstore.CacheStore_1] - Commit state failed for {0} and {1}
     * @message com.arjuna.ats.internal.arjuna.objectstore.CacheStore_2 [com.arjuna.ats.internal.arjuna.objectstore.CacheStore_2] - Remove state failed for {0} and {1} and {2}
     * @message com.arjuna.ats.internal.arjuna.objectstore.CacheStore_3 [com.arjuna.ats.internal.arjuna.objectstore.CacheStore_3] - Write state failed for {0} and {1} and {2} and {3}
     * @message com.arjuna.ats.internal.arjuna.objectstore.CacheStore_4 [com.arjuna.ats.internal.arjuna.objectstore.CacheStore_4] - Unknown work type {0}
     */

    private final synchronized void doWork ()
    {
	StoreElement work = null;

	synchronized (_workList)
	{
	    try
	    {
		work = (StoreElement) _workList.removeLast();
	    }
	    catch (java.util.NoSuchElementException ex)
	    {
		work = null;
	    }
	}
	    
	if (work != null)
	{
	    /*
	     * Should write any errors to a persistent log so that
	     * an admin tool can pick up the pieces later.
	     */

	    try
	    {
		switch (work.typeOfWork)
		{
		case AsyncStore.COMMIT:
		    {
			if (!work.store.commitState(work.objUid, work.tName))
			{
			    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.CacheStore_1",
							new Object[] { work.objUid, work.tName });
			}
		    }
		    break;
		case AsyncStore.REMOVE:
		    {
			if (!work.store.removeState(work.objUid, work.tName, work.fileType))
			{
			    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.CacheStore_2",
							new Object[] { work.objUid, work.tName, new Integer(work.fileType) });
			}
		    }
		    break;
		case AsyncStore.WRITE:
		    {
			if (!work.store.writeState(work.objUid, work.tName, work.state, work.fileType))
			{
			    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.CacheStore_3",
							new Object[] { work.objUid, work.tName, work.state, new Integer(work.fileType) });
			}			    
			
			_currentCacheSize -= work.state.size();
		    }
		    break;
		default:
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.CacheStore_3",
						new Object[] { new Integer(work.typeOfWork) });
		    break;
		}
	    }
	    catch (ObjectStoreException ex)
	    {
		ex.printStackTrace();
	    }
	}
    }
    
    private LinkedList _workList = new LinkedList();  // allows fifo manipulation
    private boolean    _terminated = false;
    private int        _currentCacheSize = 0;
    private int        _maximumCacheSize = _defaultCacheSize;  // should be from property
    private int        _sleepTime = _defaultSleepTime;  // again, from a property
    
    private static int _defaultCacheSize = 1024;  // size in bytes
    private static int _defaultSleepTime = 10000;  // time in milliseconds
    
    static
    {
	String cacheSize = arjPropertyManager.propertyManager.getProperty(com.arjuna.ats.arjuna.common.Environment.CACHE_STORE_SIZE);
	
	if (cacheSize != null)
	{
	    try
	    {
		_defaultCacheSize = Integer.parseInt(cacheSize);
	    }
	    catch (Exception ex)
	    {
		ex.printStackTrace();
	    }
	}

	String sleepTime = arjPropertyManager.propertyManager.getProperty(com.arjuna.ats.arjuna.common.Environment.CACHE_STORE_SCAN_PERIOD);

	if (sleepTime != null)
	{
	    try
	    {
		_defaultSleepTime = Integer.parseInt(sleepTime);
	    }
	    catch (Exception ex)
	    {
		ex.printStackTrace();
	    }
	}
    }
    
}
