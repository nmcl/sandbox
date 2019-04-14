/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCActionStore.java,v 1.3 2003/08/11 15:04:19 nmcl Exp $
 */

package com.arjuna.ats.internal.arjuna.objectstore;

import com.arjuna.ats.internal.arjuna.objectstore.JDBCImple;

import com.arjuna.ats.arjuna.ArjunaNames;
import com.arjuna.ats.arjuna.state.*;

import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.logging.FacilityCode;

import com.arjuna.common.util.logging.*;

import com.arjuna.ats.arjuna.gandiva.*;
import com.arjuna.common.util.propertyservice.PropertyManager;
import com.arjuna.ats.arjuna.objectstore.ObjectStore;
import com.arjuna.ats.arjuna.objectstore.ObjectStoreImple;
import com.arjuna.ats.arjuna.objectstore.ObjectStoreType;
import com.arjuna.ats.arjuna.objectstore.jdbc.JDBCAccess;
import com.arjuna.ats.arjuna.common.*;
import com.arjuna.ats.arjuna.gandiva.ObjectName;
import com.arjuna.ats.arjuna.gandiva.ClassName;

import java.io.*;
import java.sql.*;

import com.arjuna.ats.arjuna.exceptions.ObjectStoreException;
import com.arjuna.ats.arjuna.exceptions.ObjectStoreError;
import com.arjuna.ats.arjuna.exceptions.FatalError;

import java.io.IOException;

/**
 * The transaction log implementation. Refines the JDBCStore
 * implementation.
 */

/** 
 * @message com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_1 [com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_1] - JDBCActionStore could not setup store < {0} , {1} >
 * @message com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_2 [com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_2] - Received: {0} for: {1}
 * @message com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_3 [com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_3] - JDBCStore.setupStore failed to initialise!
 * @message com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_4 [com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_4] - JDBCActionStore invalid ObjectName parameter: {0}
 * @message com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_5 [com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_5] - No JDBCAccess implementation provided!
*/

public class JDBCActionStore extends JDBCStore
{

    public String getStoreName ()
    {
	if (storeValid())
	    return _txClassName+":"+_jdbcTableName;
	else
	    return "Invalid";
    }

    public int typeIs ()
    {
	return ObjectStoreType.JDBC_ACTION;
    }
    
    /**
     * The following operation commits a previous write_state operation which
     * was made with the SHADOW StateType argument. This is achieved by
     * renaming the shadow and removing the hidden version.
     */

    public synchronized boolean commit_state (Uid objUid,
					      String tName) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, 
				     "JDBCActionStore.commit_state("+objUid+", "+tName+")");
	}

	boolean result = false;

	/* Bail out if the object store is not set up */

	if (!storeValid())
	    return false;

	if (currentState(objUid, tName) == ObjectStore.OS_COMMITTED)
	    result = true;
    
	return result;
    }

    public boolean hide_state (Uid u, String tn) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.hide_state("+u+", "+tn+")");
	}

	return false;
    }

    public boolean reveal_state (Uid u, String tn) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.reveal_state("+u+", "+tn+")");
	}
	
	return false;
    }

    public InputObjectState read_committed (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.read_committed("+storeUid+", "+tName+")");
	}
	
	return super.read_committed(storeUid, tName);
    }

    public InputObjectState read_uncommitted (Uid u, String tn) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.read_uncommitted("+u+", "+tn+")");
	}
	
	return null;
    }

    public boolean remove_committed (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.remove_committed("+storeUid+", "+tName+")");
	}
	
	return super.remove_committed(storeUid, tName);
    }

    public boolean remove_uncommitted (Uid u, String tn) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.remove_uncommitted("+u+", "+tn+")");
	}
	
	return false;
    }

    public boolean write_committed (Uid storeUid, String tName, OutputObjectState state) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.write_committed("+storeUid+", "+tName+")");
	}
	
	return super.write_committed(storeUid, tName, state);
    }

    public boolean write_uncommitted (Uid u, String tn, OutputObjectState s) throws ObjectStoreException
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.write_uncommitted("+u+", "+tn+", "+s+")");
	}
	
	return false;
    }

    public ClassName className ()
    {
	return ArjunaNames.Implementation_ObjectStore_JDBCActionStore();
    }

    public static ClassName name ()
    {
	return ArjunaNames.Implementation_ObjectStore_JDBCActionStore();
    }    

    /**
     * Have to return as a JDBCStore because of
     * inheritence.
     */
    
    public static JDBCStore create ()
    {
	return new JDBCActionStore("");
    }

    public static JDBCStore create (Object[] param)
    {
	if (param == null)
	    return null;

	String tableName = (String) param[0];

	return new JDBCActionStore(tableName);
    }

    protected JDBCActionStore ()
    {
	initialise();

	_jdbcTableName = JDBCActionStore._defaultTxTableName;
	_isValid = true;
    }

    protected JDBCActionStore (String tableName)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.JDBCActionStore()");
	}
	
	initialise();

	_jdbcTableName = (((tableName == null) || (tableName.equals(""))) ? JDBCActionStore._defaultTxTableName : tableName);

	_isValid = true;
	
	if (!super.setupStore())
	{
	    if (tsLogger.arjLoggerI18N.isFatalEnabled())
	    {
		tsLogger.arjLoggerI18N.fatal("com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_1",
					     new Object[]{_txClassName, _jdbcTableName});
	    }

	    _isValid = false;
	    
	    throw new ObjectStoreError();
	}
    }

    protected JDBCActionStore (ObjectName objName)
    {
	if (tsLogger.arjLogger.debugAllowed())
	{
	    tsLogger.arjLogger.debug(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
				     FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.JDBCActionStore( "+objName+" )");
	}
	
	parseObjectName(objName);
	
	if (!setupStore())
	{
	    if (tsLogger.arjLoggerI18N.isFatalEnabled())
	    {
		tsLogger.arjLoggerI18N.fatal("com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_1",
					     new Object[]{_txClassName, _jdbcTableName});
	    }

	    _isValid = false;
	    
	    throw new ObjectStoreError();
	}
    }

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

		conn = _txJDBCAccess.getConnection();
	    }
	    catch (SQLException ex)
	    {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		{
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_2",
						 new Object[]{ex, _txJDBCAccess});
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
		
		Class c = Class.forName(this.getClass().getPackage().getName()+ "." + name.toLowerCase() + "_" + major + "_" + minor);
		
		_theImple = (JDBCImple) c.newInstance();
		_theImple.setShareStatus(super.shareStatus);
	    }
	    catch (Exception e)
	    {
		_isValid = false;

		throw new SQLException(e.toString());
	    }
	    
	    if (!_theImple.initialise(conn, _jdbcTableName, _txJDBCAccess.dropTable()))
	    {
		if (tsLogger.arjLoggerI18N.isWarnEnabled())
		    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_3");

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
		_txClassName = objName.getStringAttribute(com.arjuna.ats.arjuna.common.Environment.JDBC_TX_DB_ACCESS);
		tableName = objName.getStringAttribute(ArjunaNames.Implementation_ObjectStore_JDBC_tableName());
	    }
	    catch (Exception ex)
	    {
		error = true;
	    }
	}
	
	if (error)
	{
	    if (tsLogger.arjLoggerI18N.isWarnEnabled())
	    {
		tsLogger.arjLoggerI18N.fatal("com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_4",
					     new Object[]{objName});
	    }

	    _isValid = false;
	    
	    throw new ObjectStoreError();
	}
	else
	{
	    _jdbcTableName = (((tableName == null) || (tableName.equals(""))) ? JDBCActionStore._defaultTxTableName : tableName);

	    _isValid = false;

	    try
	    {
		Class c = Class.forName(_txClassName);

		_txJDBCAccess = (JDBCAccess) c.newInstance();
		_txJDBCAccess.initialise(objName);
	    }
	    catch (Exception e)
	    {
		throw new FatalError(e.toString());
	    }

	    if (_txJDBCAccess.tableName() != null)
		_defaultTxTableName = _txJDBCAccess.tableName();

	    _isValid = true;
	}
    }

    private final void initialise ()
    {
	String env = arjPropertyManager.propertyManager.getProperty(com.arjuna.ats.arjuna.common.Environment.JDBC_TX_DB_ACCESS);
	
	if (env == null)
	{
	    if (tsLogger.arjLoggerI18N.isFatalEnabled())
		tsLogger.arjLoggerI18N.fatal("com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_5");
	    
	    throw new FatalError(tsLogger.log_mesg.getString("com.arjuna.ats.internal.arjuna.objectstore.JDBCActionStore_5"));
	}

	try
	{
	    Class c = Class.forName(env);

	    _txJDBCAccess = (JDBCAccess) c.newInstance();
	    _txClassName = env;
	}
	catch (Exception e)
	{
	    throw new FatalError(e.toString());
	}

	if (_txJDBCAccess.tableName() != null)
	    _defaultTxTableName = _txJDBCAccess.tableName();
    }

    private JDBCAccess _txJDBCAccess = null;
    private String     _txClassName = null;

    private static String _defaultTxTableName = "ArjunaJTSTxTable";

}
