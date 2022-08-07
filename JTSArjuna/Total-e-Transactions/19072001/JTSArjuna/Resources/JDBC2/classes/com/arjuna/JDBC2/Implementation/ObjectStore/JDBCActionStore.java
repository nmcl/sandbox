/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCActionStore.java,v 1.1.2.2.10.1 2001/02/22 11:18:56 nmcl Exp $
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
	
	if (!setupStore())
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

	    try
	    {
		_theConnection = _txJDBCAccess.getConnection();
	    }
	    catch (SQLException ex)
	    {
		ErrorStream.warning().println("Received:" +ex+" for: "+_txJDBCAccess);

		_isValid = false;
		
		throw ex;
	    }

	    _theConnection.setAutoCommit(true);

	    Statement stmt = _theConnection.createStatement();

	    /*
	     * Create the transaction table.
	     */

	    // table [type, object UID, format, blob]

	    /*
	     * Need some management interface to delete the table!
	     */

	    if (_txJDBCAccess.dropTable())
	    {
		try
		{
		    stmt.executeUpdate("DROP TABLE "+_jdbcTableName);
		}
		catch (SQLException ex)
		{
		    ex.printStackTrace();
		}
	    }

	    stmt.executeUpdate("CREATE TABLE "+_jdbcTableName+" (StateType INTEGER, TypeName CHAR(1024),UidString CHAR(255),ObjectState BLOB)");
	    
	    /*
	     * Maximum size of blob appears to be 64k. Is this
	     * true for all databases?
	     */
	}
	catch (SQLException ex)
	{
	    ex.printStackTrace();
	    
	    // assume already exists
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println(e);

	    _isValid = false;
	}

	super.initialize();
	
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
