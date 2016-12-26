/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCActionStore.java,v 1.1.2.2.10.1.62.1 2001/09/28 10:29:23 nmcl Exp $
 */

package com.arjuna.JDBC2.Implementation.ObjectStore;

import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Common.ArjunaCoreEnvironment;
import com.arjuna.ArjunaCore.Atomic.*;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Implementation.ObjectStore.*;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.JDBC2.JDBC2Environment;
import java.io.File;
import java.sql.*;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import com.arjuna.ArjunaCore.Common.ObjectStoreError;
import java.io.IOException;

/**
 * The transaction log implementation. Refines the JDBCStore
 * implementation.
 */

public class JDBCActionStore extends JDBCStore
{

public String getStoreName ()
    {
	if (storeValid())
	    return JDBCActionStore._txClassName+":"+_jdbcTableName;
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
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.commit_state("+objUid+", "+tName+")");
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
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.hide_state("+u+", "+tn+")");
	}

	return false;
    }

public boolean reveal_state (Uid u, String tn) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.reveal_state("+u+", "+tn+")");
	}

	return false;
    }

public InputObjectState read_committed (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.read_committed("+storeUid+", "+tName+")");
	}

	return super.read_committed(storeUid, tName);
    }

public InputObjectState read_uncommitted (Uid u, String tn) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.read_uncommitted("+u+", "+tn+")");
	}

	return null;
    }

public boolean remove_committed (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.remove_committed("+storeUid+", "+tName+")");
	}

	return super.remove_committed(storeUid, tName);
    }

public boolean remove_uncommitted (Uid u, String tn) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.remove_uncommitted("+u+", "+tn+")");
	}

	return false;
    }

public boolean write_committed (Uid storeUid, String tName, OutputObjectState state) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.write_committed("+storeUid+", "+tName+")");
	}

	return super.write_committed(storeUid, tName, state);
    }

public boolean write_uncommitted (Uid u, String tn, OutputObjectState s) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.write_uncommitted("+u+", "+tn+", "+s+")");
	}

	return false;
    }

public ClassName className ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_JDBCActionStore();
    }

public static ClassName name ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_JDBCActionStore();
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

protected JDBCActionStore (String tableName)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().print(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCActionStore.JDBCActionStore()");
	}

	_jdbcTableName = (((tableName == null) || (tableName.equals(""))) ? JDBCActionStore._defaultTxTableName : tableName);

	_isValid = true;
	
	if (!super.setupStore())
	{
	    ErrorStream.fatal().println("JDBCActionStore could not setup store < "+_txClassName+", "+_jdbcTableName+" >");

	    _isValid = false;
	    
	    throw new ObjectStoreError();
	}
    }

protected JDBCActionStore ()
    {
	_jdbcTableName = JDBCActionStore._defaultTxTableName;
	_isValid = true;
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
		ErrorStream.warning().println("Received:" +ex+" for: "+_txJDBCAccess);

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
		
		Class c = Class.forName(_package + "." + name.toLowerCase() + "_" + major + "_" + minor);
		
		_theImple = (JDBCImple) c.newInstance();
	    }
	    catch (Exception e)
	    {
		_isValid = false;

		throw new SQLException(e.toString());
	    }
	    
	    if (!_theImple.initialise(conn, _jdbcTableName, _txJDBCAccess.dropTable()))
	    {
		ErrorStream.warning().println("JDBCStore.setupStore failed to initialise!");
		
		_isValid = false;
	    }
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println(e);

	    _isValid = false;
	}

	return _isValid;
    }

private static JDBCAccess _txJDBCAccess = null;
private static String     _defaultTxTableName = "ArjunaJTSTxTable";
private static String     _txClassName = null;

    static
    {
	String env = PropertyManager.getProperty(JDBC2Environment.JDBC2_TX_DB_ACCESS);
	
	if (env == null)
	{
	    ErrorStream.fatal().println("No JDBCAccess implementation provided!");

	    throw new ExceptionInInitializerError("No JDBCAccess implementation provided!");
	}

	try
	{
	    Class c = Class.forName(env);

	    _txJDBCAccess = (JDBCAccess) c.newInstance();
	    _txClassName = env;
	}
	catch (Exception e)
	{
	    throw new ExceptionInInitializerError(e.toString());
	}

	if (_txJDBCAccess.tableName() != null)
	    _defaultTxTableName = _txJDBCAccess.tableName();
    }
    
}
