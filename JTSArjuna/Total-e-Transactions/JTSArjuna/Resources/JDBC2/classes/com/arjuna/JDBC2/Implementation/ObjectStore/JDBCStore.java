/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCStore.java,v 1.1.2.3.2.3.58.1.14.2.36.1 2002/06/17 12:32:27 nmcl Exp $
 */

package com.arjuna.JDBC2.Implementation.ObjectStore;

import com.arjuna.JDBC2.JDBC2Environment;
import com.arjuna.ArjunaCore.ArjunaCoreNames;
import com.arjuna.ArjunaCore.Interface.ObjectStore;
import com.arjuna.ArjunaCore.Implementation.ObjectStoreImple;
import com.arjuna.ArjunaCore.Implementation.ObjectStore.*;
import com.arjuna.ArjunaCore.Common.*;
import com.arjuna.ArjunaCommon.Common.*;
import java.io.*;
import java.sql.*;

import com.arjuna.ArjunaCore.Common.ObjectStoreException;
import com.arjuna.ArjunaCore.Common.ObjectStoreError;
import java.io.IOException;

/**
 * An object store implementation which uses a JDBC database for
 * maintaining object states. All states are maintained within a
 * single table.
 *
 * It is assumed that only one object will use a given instance of
 * the JDBCStore. Hence, there is no need for synchronizations.
 */

public class JDBCStore extends ObjectStoreImple
{

public String getStoreName ()
    {
	if (storeValid())
	    return JDBCStore._jdbcAccessName+":"+_jdbcTableName;
	else
	    return "Invalid";
    }

public int typeIs ()
    {
	return ObjectStoreType.JDBC;
    }
    
public boolean commit_state (Uid objUid, String tName) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
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
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
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
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "ShadowingStore.reveal_state("+objUid+", "+tName+")");
	}

	if (storeValid())
	    return _theImple.reveal_state(objUid, tName);
	else
	    return false;
    }
    
    /*
     * Determine current state of object.
     * State search is ordered OS_SHADOW, OS_HIDDEN, OS_ORIGINAL, OS_HIDDEN
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
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCStore.read_committed("+storeUid+", "+tName+")");
	}

	return read_state(storeUid, tName, ObjectStore.OS_ORIGINAL);
    }

public InputObjectState read_uncommitted (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCStore.read_uncommitted("+storeUid+", "+tName+")");
	}

	return read_state(storeUid, tName, ObjectStore.OS_SHADOW);
    }

public boolean remove_committed (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCStore.remove_committed("+storeUid+", "+tName+")");
	}

	return remove_state(storeUid, tName, ObjectStore.OS_ORIGINAL);
    }

public boolean remove_uncommitted (Uid storeUid, String tName) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCStore.remove_uncommitted("+storeUid+", "+tName+")");
	}

	return remove_state(storeUid, tName, ObjectStore.OS_SHADOW);
    }

public boolean write_committed (Uid storeUid, String tName, OutputObjectState state) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCStore.write_committed("+storeUid+", "+tName+")");
	}

	return write_state(storeUid, tName, state, ObjectStore.OS_ORIGINAL);
    }

public boolean write_uncommitted (Uid storeUid, String tName, OutputObjectState state) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCStore.write_uncommitted("+storeUid+", "+tName+", "+state+")");
	}

	return write_state(storeUid, tName, state, ObjectStore.OS_SHADOW);
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
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCStore.allObjUids("+tName+", "+state+", "+match+")");
	}

	if (storeValid())
	    return _theImple.allObjUids(tName, state, match);
	else
	    return false;
    }
 
public boolean allTypes (InputObjectState foundTypes) throws ObjectStoreException
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCStore.allTypes("+foundTypes+")");
	}

	if (storeValid())
	    return _theImple.allTypes(foundTypes);
	else
	    return false;
    }

public synchronized void packInto (OutputBuffer buff) throws IOException
    {
	buff.packString(_jdbcAccessName);
	buff.packString(_jdbcTableName);
    }

public synchronized void unpackFrom (InputBuffer buff) throws IOException
    {
	_jdbcAccessName = buff.unpackString();
	_jdbcTableName = buff.unpackString();
    }

public ClassName className ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_JDBCStore();
    }

public static ClassName name ()
    {
	return ArjunaCoreNames.Implementation_ObjectStore_JDBCStore();
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

protected JDBCStore (String tableName)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCStore.JDBCStore()");
	}

	_jdbcTableName = (((tableName == null) || (tableName.equals(""))) ? JDBCStore._defaultTableName : tableName);

	_isValid = true;
	
	if (!setupStore())
	{
	    ErrorStream.fatal().println("JDBCStore could not setup store < "+_jdbcAccessName+", "+_jdbcTableName+" >");

	    _isValid = false;
	    
	    throw new ObjectStoreError();
	}
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
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PROTECTED,
					       FacilityCode.FAC_OBJECT_STORE, "JDBCStore.remove_state("+objUid+", "+name+", "+ObjectStore.stateTypeString(ft)+")");
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
	_jdbcTableName = JDBCStore._defaultTableName;
	_isValid = true;

	if (!setupStore())
	{
	    ErrorStream.fatal().println("JDBCStore could not setup store < "+_jdbcAccessName+", "+_jdbcTableName+" >");

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
		ErrorStream.warning().println("Received:" +ex+" for: "+_jdbcAccess);

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
	    
	    if (!_theImple.initialise(conn, _jdbcTableName, _jdbcAccess.dropTable()))
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

    /*
     * Instance specific data.
     */

protected String    _jdbcTableName;
protected boolean   _isValid;
protected JDBCImple _theImple = null;

protected static final String _package = "com.arjuna.JDBC2.Implementation.ObjectStore.drivers";
    
private static JDBCAccess _jdbcAccess = null;
private static String     _defaultTableName = "ArjunaJTSTable";
private static String     _jdbcAccessName = null;

    static
    {
	String env = PropertyManager.getProperty(JDBC2Environment.JDBC2_USER_DB_ACCESS);
	
	boolean noTable = false;
	
	if (env == null)
	{
	    ErrorStream.fatal().println("No JDBCAccess implementation provided!");

	    throw new ExceptionInInitializerError("No JDBCAccess implementation provided!");
	}

	try
	{
	    Class c = Class.forName(env);

	    _jdbcAccess = (JDBCAccess) c.newInstance();
	    _jdbcAccessName = env;
	}
	catch (Exception e)
	{
	    throw new ExceptionInInitializerError(e.toString());
	}

	if (_jdbcAccess.tableName() != null)
	    _defaultTableName = _jdbcAccess.tableName();
    }

}
