/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCStore.java,v 1.3 2003/08/11 15:04:19 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.objectstore;

import com.arjuna.ats.internal.arjuna.objectstore.JDBCImple;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.state.*;

import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.logging.FacilityCode;

import com.arjuna.common.util.propertyservice.PropertyManager;
import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.ats.arjuna.objectstore.ObjectStore;
import com.arjuna.ats.arjuna.objectstore.ObjectStoreImple;
import com.arjuna.ats.arjuna.objectstore.ObjectStoreType;
import com.arjuna.ats.arjuna.objectstore.jdbc.JDBCAccess;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.gandiva.ObjectName;
import com.arjuna.ats.arjuna.gandiva.ClassName;

import com.arjuna.common.util.logging.DebugLevel;
import com.arjuna.common.util.logging.VisibilityLevel;

import java.io.*;
import java.sql.*;

import com.arjuna.ats.arjuna.exceptions.ObjectStoreException;
import com.arjuna.ats.arjuna.exceptions.ObjectStoreError;
import com.arjuna.ats.arjuna.exceptions.FatalError;

import java.io.IOException;

/**
 * An object store implementation which uses a JDBC database for
 * maintaining object states. All states are maintained within a
 * single table.
 *
 * It is assumed that only one object will use a given instance of
 * the JDBCStore. Hence, there is no need for synchronizations.
 */

/**
 * @message com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_1 [com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_1] - JDBCStore could not setup store < {0} , {1} >
 * @message com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_2 [com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_2] - Received: {0} for: {1}
 * @message com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_3 [com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_3] - JDBCStore.setupStore failed to initialise!
 * @message com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_4 [com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_4] - JDBCStore invalid ObjectName parameter: {0}
 * @message com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_5 [com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_5] - No JDBCAccess implementation provided!
*/

public class JDBCStore extends ObjectStoreImple
{

    public String getStoreName ()
    {
	if (storeValid())
	    return _jdbcAccessClassName+":"+_jdbcTableName;
	else
	    return "Invalid";
    }

    public int typeIs ()
    {
	return ObjectStoreType.JDBC;
    }
    
    public boolean commit_state (Uid objUid, String tName) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					     FacilityCode.FAC_OBJECT_STORE, "JDBCStore.commit_state("+objUid+", "+tName+")");
	}
	
	/* Bail out if the object store is not set up */

	if (!storeValid())
	    return false;
	else
	    return _theImple.commit_state(objUid, tName);
    }

    public boolean hide_state (Uid objUid, String tName) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "ShadowingStore.hide_state("+objUid+", "+tName+")");
	}
	
	/* Bail out if the object store is not set up */

	if (storeValid())
	    return _theImple.hide_state(objUid, tName);
	else
	    return false;
    }
    
    public boolean reveal_state (Uid objUid, String tName) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "ShadowingStore.reveal_state("+objUid+", "+tName+")");
	}
	
	if (storeValid())
	    return _theImple.reveal_state(objUid, tName);
	else
	    return false;
    }
    
    /*
     * Determine current state of object.
     * State search is ordered OS_UNCOMMITTED, OS_UNCOMMITTED_HIDDEN, OS_COMMITTED, OS_COMMITTED_HIDDEN
     */

    public int currentState (Uid objUid, String tName) throws ObjectStoreException
    {
	if (storeValid())
	    return _theImple.currentState(objUid, tName);
	else
	    return ObjectStore.OS_UNKNOWN;
    }

    /**
     * Read an uncommitted instance of State out of the object store. 
     * The instance is identified by the unique id and type
     */

    public InputObjectState read_committed (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCStore.read_committed("+storeUid+", "+tName+")");
	}
	
	return read_state(storeUid, tName, ObjectStore.OS_COMMITTED);
    }

    public InputObjectState read_uncommitted (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCStore.read_uncommitted("+storeUid+", "+tName+")");
	}
	
	return read_state(storeUid, tName, ObjectStore.OS_UNCOMMITTED);
    }

    public boolean remove_committed (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCStore.remove_committed("+storeUid+", "+tName+")");
	}
	
	return remove_state(storeUid, tName, ObjectStore.OS_COMMITTED);
    }

    public boolean remove_uncommitted (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, 
				     "JDBCStore.remove_uncommitted("+storeUid+", "+tName+")");
	}
	
	return remove_state(storeUid, tName, ObjectStore.OS_UNCOMMITTED);
    }

    public boolean write_committed (Uid storeUid, String tName, OutputObjectState state) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCStore.write_committed("+storeUid+", "+tName+")");
	}
	
	return write_state(storeUid, tName, state, ObjectStore.OS_COMMITTED);
    }

    public boolean write_uncommitted (Uid storeUid, String tName, OutputObjectState state) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, 
				     "JDBCStore.write_uncommitted("+storeUid+", "+tName+", "+state+")");
	}
	
	return write_state(storeUid, tName, state, ObjectStore.OS_UNCOMMITTED);
    }

    public final boolean storeValid ()
    {
	return _isValid;
    }

    /*
     * Given a type name return an ObjectState that contains all of the
     * uids of objects of that type
     */

    public boolean allObjUids (String tName, InputObjectState state, int match) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, 
				     "JDBCStore.allObjUids("+tName+", "+state+", "+match+")");
	}
	
	if (storeValid())
	    return _theImple.allObjUids(tName, state, match);
	else
	    return false;
    }
 
    public boolean allTypes (InputObjectState foundTypes) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCStore.allTypes("+foundTypes+")");
	}
	
	if (storeValid())
	    return _theImple.allTypes(foundTypes);
	else
	    return false;
    }

    public synchronized void packInto (OutputBuffer buff) throws IOException
    {
	buff.packString(_jdbcAccessClassName);
	buff.packString(_jdbcTableName);
    }

    public synchronized void unpackFrom (InputBuffer buff) throws IOException
    {
	_jdbcAccessClassName = buff.unpackString();
	_jdbcTableName = buff.unpackString();
    }

    public ClassName className ()
    {
	return ArjunaNames.Implementation_ObjectStore_JDBCStore();
    }

    public static ClassName name ()
    {
	return ArjunaNames.Implementation_ObjectStore_JDBCStore();
    }    

    public static JDBCStore create ()
    {
	return new JDBCStore();
    }

    public static JDBCStore create (Object[] param)
    {
	if (param == null)
	    return null;

	String tableName = (String) param[0];
	
	return new JDBCStore(tableName);
    }

    public static JDBCStore create (ObjectName param)
    {
	if (param == null)
	    return null;
	
	return new JDBCStore(param);
    }

    protected InputObjectState read_state (Uid objUid, String tName, int ft) throws ObjectStoreException
    {
	if (!storeValid())
	    return null;
	else
	    return _theImple.read_state(objUid, tName, ft);
    }

    /**
     * We don't actually delete the state entry, only change
     * its type.
     */

    protected boolean remove_state (Uid objUid, String name, int ft) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
				     FacilityCode.FAC_OBJECT_STORE, 
				     "JDBCStore.remove_state("+objUid+", "+name+", "+ObjectStore.stateTypeString(ft)+")");
	}
	
	if (!storeValid())
	    return false;
	else
	    return _theImple.remove_state(objUid, name, ft);
    }
    
    protected boolean write_state (Uid objUid, String tName, OutputObjectState state, int s) throws ObjectStoreException
    {
	if (!storeValid())
	    return false;
	else
	    return _theImple.write_state(objUid, tName, state, s);
    }

    protected JDBCStore ()
    {
	initialise();

	_jdbcTableName = JDBCStore._defaultTableName;
	_isValid = true;
    }

    protected JDBCStore (String tableName)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCStore.JDBCStore()");
	}
	
	initialise();

	_jdbcTableName = (((tableName == null) || (tableName.equals(""))) ? JDBCStore._defaultTableName : tableName);

	_isValid = true;
	
	if (!setupStore())
	{
	    if (tsLogger.arjLoggerI18N.isFatalEnabled())
	    {
		tsLogger.arjLoggerI18N.fatal("com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_1",
					     new Object[]{_jdbcAccessClassName, _jdbcTableName});
	    }

	    _isValid = false;
	    
	    throw new ObjectStoreError();
	}
    }

    protected JDBCStore (ObjectName objName)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCStore.JDBCStore( "+objName+" )");
	}
	
	parseObjectName(objName);
	
	if (!setupStore())
	{
	    if (tsLogger.arjLoggerI18N.isFatalEnabled())
	    {
		tsLogger.arjLoggerI18N.fatal("com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_1",
					     new Object[]{_jdbcAccessClassName,_jdbcTableName});
	    }

	    _isValid = false;
	    
	    throw new ObjectStoreError();
	}
    }
    
    protected boolean supressEntry (String name)
    {
	return true;
    }

/*
 * Try to create the original and shadow/hidden tables. If this
 * fails, then we will exit.
 */

    protected synchronized boolean setupStore ()
    {
	try
	{
	    /*
	     * This had better not be an Arjuna jdbc connection!
	     */
	    
	    Connection conn = null;
	    
	    try
	    {
		/*
		 * If some form of pooling is required, then the access class
		 * can do that on a per implementation basis.
		 */

		conn = _jdbcAccess.getConnection();
	    }
	    catch (SQLException ex)
	    {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		{
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_2",
						new Object[]{ex, _jdbcAccess});
		}

		_isValid = false;
		
		throw ex;
	    }

	    conn.setAutoCommit(true);

	    try
	    {
		DatabaseMetaData md = conn.getMetaData();
		
		String name = md.getDriverName();
		int major = md.getDriverMajorVersion();
		int minor = md.getDriverMinorVersion();
		
		/*
		 * Check for spaces in the name - our implementation classes
		 * are always just the first part of such names.
		 */

		int index = name.indexOf(' ');
		
		if (index != -1)
		    name = name.substring(0, index);
		
		Class c = Class.forName(this.getClass().getPackage().getName()+ ".jdbc." + name.toLowerCase() + "_" + major + "_" + minor);
		
		_theImple = (JDBCImple) c.newInstance();
		_theImple.setShareStatus(super.shareStatus);
	    }
	    catch (Exception e)
	    {
		_isValid = false;

		throw new SQLException(e.toString());
	    }
	    
	    if (!_theImple.initialise(conn, _jdbcTableName, _jdbcAccess.dropTable()))
	    {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_3");

		_isValid = false;
	    }
	}
	catch (Exception e)
	{
	    tsLogger.arjLogger.warn(e.toString());

	    _isValid = false;
	}

	return _isValid;
    }

    private void parseObjectName (ObjectName objName)
    {
	boolean error = false;
	String tableName = null;
	
	if (objName == null)
	    error = true;
	else
	{
	    try
	    {
		_jdbcAccessClassName = objName.getStringAttribute(Environment.JDBC_USER_DB_ACCESS);
		tableName = objName.getStringAttribute(ArjunaNames.Implementation_ObjectStore_JDBC_tableName());
	    }
	    catch (Exception ex)
	    {
		error = true;
	    }
	}
	
	if (error)
	{
	    if (tsLogger.arjLoggerI18N.isFatalEnabled())
	    {
		tsLogger.arjLoggerI18N.fatal("com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_4",
					     new Object[]{objName});
	    }

	    _isValid = false;
	    
	    throw new ObjectStoreError();
	}
	else
	{
	    _jdbcTableName = (((tableName == null) || (tableName.equals(""))) ? JDBCStore._defaultTableName : tableName);

	    _isValid = false;

	    try
	    {
		Class c = Class.forName(_jdbcAccessClassName);

		_jdbcAccess = (JDBCAccess) c.newInstance();

		_jdbcAccess.initialise(objName);
	    }
	    catch (Exception e)
	    {
		throw new FatalError(e.toString());
	    }

	    if (_jdbcAccess.tableName() != null)
		_defaultTableName = _jdbcAccess.tableName();

	    _isValid = true;
	}
    }
    
    private final void initialise ()
    {
	String env = arjPropertyManager.propertyManager.getProperty(Environment.JDBC_USER_DB_ACCESS);
	
	boolean noTable = false;
	
	if (env == null)
	{
	    if (tsLogger.arjLoggerI18N.isFatalEnabled())
		tsLogger.arjLoggerI18N.fatal("com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_5");

	    throw new FatalError(tsLogger.log_mesg.getString("com.arjuna.ats.internal.arjuna.objectstore.JDBCStore_5"));
	}

	try
	{
	    Class c = Class.forName(env);

	    _jdbcAccess = (JDBCAccess) c.newInstance();
	    _jdbcAccessClassName = env;
	}
	catch (Exception e)
	{
	    throw new FatalError(e.toString());
	}

	if (_jdbcAccess.tableName() != null)
	    _defaultTableName = _jdbcAccess.tableName();
    }

    /*
     * Instance specific data.
     */

    protected String    _jdbcTableName;
    protected boolean   _isValid;
    protected JDBCImple _theImple = null;

    private JDBCAccess _jdbcAccess = null;
    private String     _jdbcAccessClassName = null;

    private static String _defaultTableName = "ArjunaJTSTable";
 
}
