/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: sequelink_5_1.java,v 1.1.2.2 2001/10/18 11:27:15 nmcl Exp $
 */

package com.arjuna.JDBC2.Implementation.ObjectStore.drivers;

import com.arjuna.JDBC2.Implementation.ObjectStore.JDBCImple;
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

public class sequelink_5_1 implements JDBCImple
{

public sequelink_5_1 ()
    {
	_jdbcTableName = null;
	_isValid = true;
	_dropTable = false;
	
	/*
	 * We don't actually create the prepared statements at
	 * this point, but do it lazily, when and if required.
	 */

	_preparedStatements = new PreparedStatement[sequelink_5_1.STATEMENT_SIZE];

	for (int i = 0; i < sequelink_5_1.STATEMENT_SIZE; i++)
	    _preparedStatements[i] = null;

	_theConnection = null;
    }

public boolean initialise (Connection conn, String tableName, boolean dropTable)
    {
	_theConnection = conn;
	_jdbcTableName = tableName;
	_dropTable = dropTable;

	try
	{
	    Statement stmt = _theConnection.createStatement();

	    /*
	     * Create the transaction table.
	     */

	    // table [type, object UID, format, blob]

	    /*
	     * Need some management interface to delete the table!
	     */

	    if (dropTable)
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
	    //	    ex.printStackTrace();
	    
	    // assume already exists
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println(e);

	    _isValid = false;
	}

	return _isValid;
    }
    
public boolean commit_state (Uid objUid, String tName) throws ObjectStoreException
    {
	boolean result = false;

	/* Bail out if the object store is not set up */

	if (!storeValid())
	    return false;

	if (tName != null)
	{
	    int state = currentState(objUid, tName);

	    try
	    {
		if ((state == ObjectStore.OS_UNCOMMITTED_HIDDEN) ||
		    (state == ObjectStore.OS_UNCOMMITTED))
		{
		    PreparedStatement pstmt = _preparedStatements[COMMIT_STATE];
		    
		    if (pstmt == null)
		    {
			pstmt = _theConnection.prepareStatement("UPDATE "+_jdbcTableName+" SET StateType = ? WHERE UidString = ?");

			_preparedStatements[COMMIT_STATE] = pstmt;
		    }
		    
		    /* maintain hidden status on rename */

		    int stateType = ((state == ObjectStore.OS_UNCOMMITTED_HIDDEN) ? ObjectStore.OS_COMMITTED_HIDDEN : ObjectStore.OS_COMMITTED);

		    pstmt.setInt(1, stateType);
		    pstmt.setString(2, "'"+objUid.stringForm()+"'");
		    
		    pstmt.executeUpdate();

		    result = true;
		}
	    }
	    catch (Exception e)
	    {
		e.printStackTrace();
		
		throw new ObjectStoreException(e.toString());
	    }
	}
	else
	    throw new ObjectStoreException("ShadowStore::commit_state - object with uid "+objUid+" has no TypeName");

	return result;
    }

public boolean hide_state (Uid objUid, String tName) throws ObjectStoreException
    {
	boolean hiddenOk = true;

	/* Bail out if the object store is not set up */

	if (storeValid())
	{
	    PreparedStatement pstmt = null;
	    
	    try
	    {
		pstmt = _preparedStatements[HIDE_STATE];

		if (pstmt == null)
		{
		    pstmt = _theConnection.prepareStatement("UPDATE "+_jdbcTableName+" SET StateType = ? WHERE UidString = ?");
			    
		    _preparedStatements[HIDE_STATE] = pstmt;
		}
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println("JDBCStore.hide_state: "+e);
		
		return false;
	    }

	    int state = currentState(objUid, tName);
	    
	    switch (state)
	    {
	    case ObjectStore.OS_UNCOMMITTED_HIDDEN:
	    case ObjectStore.OS_COMMITTED_HIDDEN:
		break;
	    case ObjectStore.OS_COMMITTED:
		{
		    try
		    {
			pstmt.setInt(1, ObjectStore.OS_COMMITTED_HIDDEN);
			pstmt.setString(2, "'"+objUid.stringForm()+"'");

			pstmt.executeUpdate();
		    }
		    catch (Exception e)
		    {
			e.printStackTrace();
			
			throw new ObjectStoreException(e.toString());
		    }

		    break;
		}
	    case ObjectStore.OS_UNCOMMITTED:
		{
		    try
		    {
			pstmt.setInt(1, ObjectStore.OS_UNCOMMITTED_HIDDEN);
			pstmt.setString(2, "'"+objUid.stringForm()+"'");

			pstmt.executeUpdate();
		    }
		    catch (Exception e)
		    {
			e.printStackTrace();
			
			throw new ObjectStoreException(e.toString());
		    }
		    
		    break;
		}
	    default:
		hiddenOk = false;
	    }
	}
	else
	    hiddenOk = false;

	return hiddenOk;
    }
    
public boolean reveal_state (Uid objUid, String tName) throws ObjectStoreException
    {
	boolean revealedOk = true;

	if (storeValid())
	{
	    PreparedStatement pstmt = null;

	    try
	    {
		pstmt = _preparedStatements[REVEAL_STATE];

		if (pstmt == null)
		{
		    pstmt = _theConnection.prepareStatement("UPDATE "+_jdbcTableName+" SET StateType = ? WHERE UidString = ?");
		
		    _preparedStatements[REVEAL_STATE] = pstmt;
		}
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println("JDBCStore.reveal_state: "+e);
		
		return false;
	    }
	    
	    int state = currentState(objUid, tName);
	    String path1 = null;
	    String path2 = null;

	    switch (state)
	    {
	    case ObjectStore.OS_UNCOMMITTED_HIDDEN:
		{
		    try
		    {
			pstmt.setInt(1, ObjectStore.OS_UNCOMMITTED);
			pstmt.setString(2, "'"+objUid.stringForm()+"'");
			
			pstmt.executeUpdate();
		    }
		    catch (Exception e)
		    {
			e.printStackTrace();

			throw new ObjectStoreException(e.toString());
		    }
		    
		    break;
		}
	    case ObjectStore.OS_COMMITTED_HIDDEN:
		{
		    try
		    {
			pstmt.setInt(1, ObjectStore.OS_COMMITTED);
			pstmt.setString(2, "'"+objUid.stringForm()+"'");
			
			pstmt.executeUpdate();
		    }
		    catch (Exception e)
		    {
			e.printStackTrace();

			throw new ObjectStoreException(e.toString());
		    }
		    
		    break;
		}
	    case ObjectStore.OS_COMMITTED:
	    case ObjectStore.OS_UNCOMMITTED:
		break;
	    default:
		revealedOk = false;
	    }
	}
	else
	    revealedOk = false;

	return revealedOk;
    }
    
    /*
     * Determine current state of object.
     * State search is ordered OS_SHADOW, OS_HIDDEN, OS_ORIGINAL, OS_HIDDEN
     */

public int currentState (Uid objUid, String tName) throws ObjectStoreException
    {
	int theState = ObjectStore.OS_UNKNOWN;
    
	if (storeValid())
	{
	    /*
	     * First check the shadow/hidden table for the
	     * entry.
	     */

	    try
	    {
		ResultSet rs = null;
		
		try
		{
		    PreparedStatement pstmt = _preparedStatements[CURRENT_STATE];
		    
		    if (pstmt == null)
		    {
			pstmt = _theConnection.prepareStatement("SELECT StateType,UidString FROM "+_jdbcTableName+" WHERE UidString = ?");
			
			_preparedStatements[CURRENT_STATE] = pstmt;
		    }
		    
		    pstmt.setString(1, "'"+objUid.stringForm()+"'");
		    
		    rs = pstmt.executeQuery();
		}
		catch (Exception e)
		{
		    e.printStackTrace();
		    
		    rs = null;
		}

		try
		{
		    rs.next();

		    int stateStatus = rs.getInt(1);
		    
		    switch (stateStatus)
		    {
		    case ObjectStore.OS_SHADOW:
			theState = ObjectStore.OS_UNCOMMITTED;
			break;
		    case ObjectStore.OS_ORIGINAL:
			theState = ObjectStore.OS_COMMITTED;
			break;
		    case ObjectStore.OS_INVISIBLE:
			theState = ObjectStore.OS_HIDDEN;
			break;
		    default:
			theState = stateStatus;
			break;
		    }
		}
		catch (SQLException e)
		{
		    switch (e.getErrorCode())
		    {
		    case 7421:  // no row, so not present!
			break;
		    default:
			{
			    e.printStackTrace();
			}
			break;
		    }
		}
		catch (NullPointerException ex)
		{
		    //		    ex.printStackTrace();
		}
		catch (Exception e)
		{
		    ErrorStream.warning().println(e);
		    
		    return ObjectStore.OS_UNKNOWN;
		}
	    }
	    catch (Exception e)
	    {
		ErrorStream.warning().println(e);
		    
		return ObjectStore.OS_UNKNOWN;
	    }
	}

	return theState;
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
	try
	{
	    Statement stmt = _theConnection.createStatement();
	    ResultSet rs = null;
	    OutputObjectState store = new OutputObjectState();
		
	    try
	    {
		/*
		 * Not used enough to warrant a PreparedStatement.
		 */

		rs = stmt.executeQuery("SELECT UidString FROM "+_jdbcTableName);
	    }
	    catch (Exception e)
	    {
		e.printStackTrace();

		rs = null;
	    }

	    boolean finished = false;
	    
	    while (!finished)
	    {
		Uid theUid = null;
		
		rs.next();

		try
		{
		    theUid = new Uid(rs.getString(1));
		    theUid.pack(store);
		}
		catch (IOException ex)
		{
		    ErrorStream.warning().println("JDBCStore.allObjUids - pack of Uid failed: "+ex);
		    
		    return false;
		}
		catch (NullPointerException ex)
		{
		    ex.printStackTrace();

		    finished = true;
		}
		catch (Exception e)
		{
		    e.printStackTrace();

		    finished = true;
		}
	    }

	    try
	    {
		Uid.nullUid().pack(store);
	    }
	    catch (IOException e)
	    {
		e.printStackTrace();

		throw new ObjectStoreException("JDBCStore::allObjUids - could not pack end of list Uid.");
	    }

	    state.setBuffer(store.buffer());

	    store = null;

	    return true;
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println(e);
	}

	return false;
    }
 
public boolean allTypes (InputObjectState foundTypes) throws ObjectStoreException
    {
	try
	{
	    Statement stmt = _theConnection.createStatement();
	    ResultSet rs = null;
	    OutputObjectState store = new OutputObjectState();
		
	    try
	    {
		/*
		 * Not used enough to warrant a PreparedStatement.
		 */

		rs = stmt.executeQuery("SELECT TypeName FROM "+_jdbcTableName);
	    }
	    catch (Exception e)
	    {
		e.printStackTrace();

		rs = null;
	    }

	    boolean finished = false;
	    
	    while (!finished)
	    {
		rs.next();

		try
		{
		    String type = rs.getString(1);
		    store.packString(type);
		}
		catch (IOException ex)
		{
		    ErrorStream.warning().println("JDBCStore.allTypes - pack failed: "+ex);
		    
		    return false;
		}
		catch (NullPointerException exp)
		{
		    exp.printStackTrace();

		    finished = true;
		}
		catch (Exception e)
		{
		    e.printStackTrace();

		    finished = true;
		}
	    }

	    try
	    {
		store.packString("");
	    }
	    catch (IOException e)
	    {
		e.printStackTrace();

		throw new ObjectStoreException("JDBCStore::allTypes - could not pack end of list string.");
	    }
	    
	    foundTypes.setBuffer(store.buffer());

	    return true;
	}
	catch (Exception e)
	{
	    ErrorStream.warning().println(e);
	}

	return false;
    }

public InputObjectState read_state (Uid objUid, String tName, int ft) throws ObjectStoreException
    {
	InputObjectState newImage = null;
	
	if (!storeValid())
	    return newImage;

	if (tName != null)
	{
	    int state = currentState(objUid, tName);

	    if ((state == ObjectStore.OS_COMMITTED) ||
		(state == ObjectStore.OS_UNCOMMITTED))
	    {
		/*
		 * Is the current state the same as that requested?
		 */

		if (((state == ObjectStore.OS_COMMITTED) && (ft != ObjectStore.OS_ORIGINAL)) ||
		    ((state == ObjectStore.OS_UNCOMMITTED) && (ft != ObjectStore.OS_SHADOW)))
		{
		    return null;
		}

		/*
		 * Load the committed state.
		 */

		try
		{
		    PreparedStatement pstmt = _preparedStatements[READ_STATE];
		    
		    if (pstmt == null)
		    {
			pstmt = _theConnection.prepareStatement("SELECT ObjectState,TypeName FROM "+_jdbcTableName+" WHERE UidString = ?");
			
			_preparedStatements[READ_STATE] = pstmt;
		    }
		    
		    pstmt.setString(1, "'"+objUid.stringForm()+"'");
		    
		    ResultSet rs = pstmt.executeQuery();

		    rs.next();

		    byte[] buffer = rs.getBytes(1);

		    if (buffer != null)
		    {
			newImage = new InputObjectState(objUid, tName, buffer);
		    }
		    else
			ErrorStream.stream(ErrorStream.WARNING).println("JDBCStore::read_state() failed");
		}
		catch (Exception e)
		{
		    e.printStackTrace();

		    throw new ObjectStoreException(e.toString());
		}
	    }
	}
	else
	    throw new ObjectStoreException("JDBCStore.read_state - object with uid "+objUid+" has no TypeName");

	return newImage;
    }

    /**
     * We don't actually delete the state entry, only change
     * its type.
     */

public boolean remove_state (Uid objUid, String name, int ft) throws ObjectStoreException
    {
	boolean removeOk = true;

	if (!storeValid())
	    return false;
    
	if (name != null)
	{
	    int state = currentState(objUid, name);

	    if ((state == ObjectStore.OS_COMMITTED) || (state == ObjectStore.OS_UNCOMMITTED))
	    {
		try
		{
		    PreparedStatement pstmt = _preparedStatements[REMOVE_STATE];
		    
		    if (pstmt == null)
		    {
			pstmt = _theConnection.prepareStatement("DELETE FROM "+_jdbcTableName+" WHERE StateType = ? AND UidString = ?");

			_preparedStatements[REMOVE_STATE] = pstmt;
		    }
		    
		    pstmt.setInt(1, state);
		    pstmt.setString(2, "'"+objUid.stringForm()+"'");
		    
		    pstmt.executeUpdate();
		}
		catch (Exception e)
		{
		    e.printStackTrace();

		    removeOk = false;
		}
	    }
	    else
	    {
		removeOk = false;

		ErrorStream.warning().print("JDBCStore::remove_state() attempted removal of ");
		
		if (state == ObjectStore.OS_UNKNOWN)
		    ErrorStream.stream().print("unknown ");
		else
		    ErrorStream.stream().print("hidden ");
		
		ErrorStream.stream().println("state for object with uid "+objUid);
	    }
	}
	else
	{
	    removeOk = false;

	    ErrorStream.warning().println("JDBCStore.remove_state - type() operation of object with uid "+objUid+" returns NULL");
	}
    
	return removeOk;
    }
    
public boolean write_state (Uid objUid, String tName, OutputObjectState state, int s) throws ObjectStoreException
    {
	if (!storeValid())
	    return false;
    
	if (tName != null)
	{
	    int imageSize = (int) state.length();

	    if (imageSize > sequelink_5_1._maxStateSize)
		throw new ObjectStoreException("Object state is too large - maximum size allowed: "+ sequelink_5_1._maxStateSize);
	    
	    byte[] b = state.buffer();

	    /*
	     * Get the current state to determine whether or not this
	     * write call is for a new object, in which case we need
	     * to insert, or for an existing object, in which case we
	     * need to update.
	     */

	    int stateStatus = currentState(objUid, tName);
	    
	    if (imageSize > 0)
	    {
		try
		{
		    /*
		     * Does the database do the equivalent of a sync?
		     * Should we call close to do this?
		     */

		    switch (stateStatus)
		    {
		    case ObjectStore.OS_UNKNOWN: // new state, so insert
			{
			    PreparedStatement pstmt = _preparedStatements[WRITE_STATE_NEW];
			    
			    if (pstmt == null)
			    {
				pstmt = _theConnection.prepareStatement("INSERT INTO "+_jdbcTableName+" (StateType,TypeName,UidString,ObjectState) VALUES (?,?,?,?)");

				_preparedStatements[WRITE_STATE_NEW] = pstmt;
			    }

			    pstmt.setInt(1, s);
			    pstmt.setString(2, "'"+tName+"'");
			    pstmt.setString(3, "'"+objUid.stringForm()+"'");
			    pstmt.setBytes(4, b);

			    pstmt.executeUpdate();
			}
			break;
		    default: // existing state, so update
			{
			    PreparedStatement pstmt = _preparedStatements[WRITE_STATE_EXISTING];
			    
			    if (pstmt == null)
			    {
				pstmt = _theConnection.prepareStatement("UPDATE "+_jdbcTableName+" SET StateType = ?, ObjectState = ? WHERE UidString = ? AND TypeName = ?");

				_preparedStatements[WRITE_STATE_EXISTING] = pstmt;
			    }

			    pstmt.setInt(1, s);
			    pstmt.setBytes(2, b);
			    pstmt.setString(3, "'"+objUid.stringForm()+"'");
			    pstmt.setString(4, "'"+tName+"'");

			    pstmt.executeUpdate();
			}
			break;
		    }
		}
		catch (SQLException ex)
		{
		    ex.printStackTrace();
		    
		    return false;
		}
		catch (Exception e)
		{
		    ErrorStream.warning().println(e);
		    
		    return false;
		}
	    }
	    else
		throw new ObjectStoreException("JDBCStore.read_state - object state size 0!");
	}
	else
	    throw new ObjectStoreException("JDBCStore.read_state - object with uid "+objUid+" has no TypeName");

	return true;
    }

    /*
     * Instance specific data.
     */

protected String              _jdbcTableName;
protected boolean             _isValid;
protected boolean             _dropTable;
protected PreparedStatement[] _preparedStatements = null;
protected Connection          _theConnection = null;

private static final int  _maxStateSize = 65535;

    /*
     * IMPORTANT: remember to update if we add more prepare statements!
     */

private static final int COMMIT_STATE = 0;
private static final int HIDE_STATE = 1;
private static final int REVEAL_STATE = 2;
private static final int CURRENT_STATE = 3;
private static final int READ_STATE = 4;
private static final int REMOVE_STATE = 5;
private static final int WRITE_STATE_NEW = 6;
private static final int WRITE_STATE_EXISTING = 7;
private static final int STATEMENT_SIZE = 8;

}
