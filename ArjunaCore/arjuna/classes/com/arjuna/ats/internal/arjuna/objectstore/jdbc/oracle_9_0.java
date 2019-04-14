/*
 * oracle_9_0.java
 *
 * Copyright (c) 2001 Hewlett-Packard Company
 * Hewlett-Packard Company Confidential
 *
 * $Project: ArjunaCore$
 * $Revision: 1.1 $
 * $Date: 2003/06/19 10:50:35 $
 * $Author: nmcl $
 */

/**
 * Developer's TODO list:
 *
 * Do multiple instances of this class get created for a given connection url ?
 * If so, how do we stop this happening ? 
 * 
 * Re: connection robustness
 * The connection pooling/retry code from HP-MS has been disabled,
 * as it will not work under ArjunaCore (no way to get a new connection)
 * 
 * Note: This impl has come from HP-TS-2.2 via. HP-MS 1.0
 * It had CSF style logging added for HP-MS. Rather than removing this,
 * is has just been commented out, as it may be needed in the future...
 */

/*
 * JDBC store implementations are driver specific.
 * This version for Oracle 8i/9i JDBC Drivers (OCI or Thin) ONLY.
 */
package com.arjuna.ats.internal.arjuna.objectstore.jdbc;

import java.io.IOException;

import java.sql.Blob;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

import com.arjuna.ats.arjuna.common.Uid;
import com.arjuna.ats.arjuna.state.InputBuffer;
import com.arjuna.ats.arjuna.state.InputObjectState;
import com.arjuna.ats.arjuna.state.OutputBuffer;
import com.arjuna.ats.arjuna.state.OutputObjectState;
import com.arjuna.ats.arjuna.objectstore.ObjectStore;
import com.arjuna.ats.arjuna.objectstore.ObjectStoreImple;
import com.arjuna.ats.internal.arjuna.objectstore.JDBCImple;
import com.arjuna.ats.arjuna.logging.tsLogger;
import com.arjuna.ats.arjuna.exceptions.ObjectStoreException;

import oracle.jdbc.OracleDriver;
import oracle.sql.BLOB;

/**
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_1 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_1] - oracle_9_0.hide_state caught exception: 
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_2 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_2] - oracle_9_0.reveal_state caught exception: 
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_3 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_3] - oracle_9_0.currentState caught exception: 
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_4 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_4] - oracle_9_0.allObjUids caught exception: 
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_5 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_5] - oracle_9_0.allObjUids - pack of Uid failed: 
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_6 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_6] - oracle_9_0.allTypes caught exception: 
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_7 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_7] - oracle_9_0.allTypes - pack of Uid failed: 
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_8 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_8] - oracle_9_0.read_state failed
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_9 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_9] - oracle_9_0.remove_state caught exception: 
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_10 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_10] - oracle_9_0.remove_state() attempted removal of {0} state for object with uid {1}
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_11 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_11] - oracle_9_0.remove_state - type() operation of object with uid {0} returns NULL
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_12 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_12] - oracle_9_0.write_state caught exception: 
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_13 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_13] - oracle_9_0.initialise caught exception:
 * @message com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_14 [com.arjuna.ats.internal.arjuna.objectstore.drivers.oracle_9_0_14] - oracle_9_0.freePool - freeing a connection which is already free!
 */

/**
 * An object store implementation which uses a JDBC database for
 * maintaining object states. All states are maintained within a
 * single table.
 */
public class oracle_9_0 extends JDBCImple
{
	public oracle_9_0()
	{
		_jdbcTableName = null;
		_isValid = false;
		_dropTable = false;
	}

	public final boolean storeValid ()
	{
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
			int currState = currentState(objUid, tName);
			
			int pool = getPool();
			try
			{
				// remove the old committed state, if any:
				PreparedStatement pstmt = _preparedStatements[pool][PRECOMMIT_CLEANUP];
				if(pstmt == null)
				{
					pstmt = _theConnection[pool].prepareStatement("DELETE FROM "+_jdbcTableName+" WHERE UidString = ? AND StateType = "+ObjectStore.OS_COMMITTED);
					_preparedStatements[pool][PRECOMMIT_CLEANUP] = pstmt;
				}
				pstmt.setString(1, objUid.stringForm());
				pstmt.executeUpdate();
				// done cleanup
					
				// now do the commit itself:
				pstmt = _preparedStatements[pool][COMMIT_STATE];

				if (pstmt == null)
				{
					pstmt = _theConnection[pool].prepareStatement("UPDATE "+_jdbcTableName+" SET StateType = ? WHERE UidString = ? AND StateType = ?");
					_preparedStatements[pool][COMMIT_STATE] = pstmt;
				}

				if(currState == ObjectStore.OS_UNCOMMITTED) {
					pstmt.setInt(1, ObjectStore.OS_COMMITTED);
				} else if (currState == ObjectStore.OS_UNCOMMITTED_HIDDEN) {
					pstmt.setInt(1, ObjectStore.OS_COMMITTED_HIDDEN);
				} else {
					throw new ObjectStoreException("can't commit object "+objUid+" in state "+currState);
				}
					
				pstmt.setString(2, objUid.stringForm());
				pstmt.setInt(3, currState);

				int rowcount = pstmt.executeUpdate();

				if(rowcount > 0) {
					result = true;
				}
			}
			catch (Exception e)
			{
				if(retryConnection(e, pool)) {
					return commit_state(objUid, tName);
				} else {
					throw new ObjectStoreException(e.toString());
				}
			}
			finally
			{
				freePool(pool);
			}
		}
		else
			throw new ObjectStoreException("oracle_9_0::commit_state - object with uid "+objUid+" has no TypeName");

		return result;
	}

	
	public boolean hide_state (Uid objUid, String tName) throws ObjectStoreException
	{
		
		boolean hiddenOk = true;

		/* Bail out if the object store is not set up */

		if (storeValid())
		{
			int state = currentState(objUid, tName);
			
			int pool = getPool();
			
			PreparedStatement pstmt = null;
			try
			{
				pstmt = _preparedStatements[pool][HIDE_STATE];

				if (pstmt == null)
				{
					pstmt = _theConnection[pool].prepareStatement("UPDATE "+_jdbcTableName+" SET StateType = ? WHERE UidString = ? AND StateType = ?");

					_preparedStatements[pool][HIDE_STATE] = pstmt;
				}
			}
			catch (Exception e)
			{
			    if (tsLogger.arjLoggerI18n.isWarnEnabled())
				tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_1",e);

			    freePool(pool);
			    return false;
			}

			try
			{
				switch (state)
				{
				case ObjectStore.OS_UNCOMMITTED_HIDDEN:
				case ObjectStore.OS_COMMITTED_HIDDEN:
					break;
				case ObjectStore.OS_UNCOMMITTED:
					{
						pstmt.setInt(1, ObjectStore.OS_UNCOMMITTED_HIDDEN);
						pstmt.setString(2, objUid.stringForm());
						pstmt.setInt(3, state);
						pstmt.executeUpdate();
					}
					break;
				case ObjectStore.OS_COMMITTED:
					{
						pstmt.setInt(1, ObjectStore.OS_COMMITTED_HIDDEN);
						pstmt.setString(2, objUid.stringForm());
						pstmt.setInt(3, state);
						pstmt.executeUpdate();
					}
					break;
				default:
					hiddenOk = false;
				}
			}
			catch(Exception e)
			{
				if(retryConnection(e, pool)) {
					hide_state(objUid, tName);
				} else {
					throw new ObjectStoreException(e.toString());
				}
			} finally {
				freePool(pool);
			}
		}
		else
		{
			hiddenOk = false;
		}

		return hiddenOk;
	}

	
	public boolean reveal_state (Uid objUid, String tName) throws ObjectStoreException
	{
		
		boolean revealedOk = true;

		if (storeValid())
		{
			int state = currentState(objUid, tName);
			
			int pool = getPool();
			
			PreparedStatement pstmt = null;
			try
			{
				pstmt = _preparedStatements[pool][REVEAL_STATE];

				if (pstmt == null)
				{
					pstmt = _theConnection[pool].prepareStatement("UPDATE "+_jdbcTableName+" SET StateType = ? WHERE UidString = ? AND StateType = ?");
					_preparedStatements[pool][REVEAL_STATE] = pstmt;
				}
			}
			catch (Exception e)
			{
			    if (tsLogger.arjLoggerI18n.isWarnEnabled())
				tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_2",2);

			    freePool(pool);
			    return false;
			}

			try
			{
				switch (state)
				{
				case ObjectStore.OS_UNCOMMITTED_HIDDEN:
					{
						pstmt.setInt(1, ObjectStore.OS_UNCOMMITTED);
						pstmt.setString(2, objUid.stringForm());
						pstmt.setInt(3, state);
						pstmt.executeUpdate();
					}
					break;
				case ObjectStore.OS_COMMITTED_HIDDEN:
					{
						pstmt.setInt(1, ObjectStore.OS_COMMITTED);
						pstmt.setString(2, objUid.stringForm());
						pstmt.setInt(3, state);
						pstmt.executeUpdate();
					}
					break;
				case ObjectStore.OS_COMMITTED:
				case ObjectStore.OS_UNCOMMITTED:
					break;
				default:
					revealedOk = false;
				}
			}
			catch(Exception e)
			{
				if(retryConnection(e, pool)) {
					reveal_state(objUid, tName);
				} else {
					throw new ObjectStoreException(e.toString());
				}
			} finally {
				freePool(pool);
			}
		}
		else
		{
			revealedOk = false;
		}

		return revealedOk;
	}

	
		
	/**
	 * currentState - determine the current state of an object.
	 * State search is ordered OS_UNCOMMITTED, OS_UNCOMMITTED_HIDDEN, OS_COMMITTED, OS_COMMITTED_HIDDEN
	 *
	 */
	public int currentState (Uid objUid, String tName) throws ObjectStoreException
	{
		int theState = ObjectStore.OS_UNKNOWN;

		if (storeValid())
		{
			int pool = getPool();
			try
			{
				ResultSet rs = null;

				PreparedStatement pstmt = _preparedStatements[pool][CURRENT_STATE];

				if (pstmt == null)
				{
					pstmt = _theConnection[pool].prepareStatement("SELECT StateType, UidString FROM "+_jdbcTableName+" WHERE UidString = ?");
					_preparedStatements[pool][CURRENT_STATE] = pstmt;
				}

				pstmt.setString(1, objUid.stringForm());

				rs = pstmt.executeQuery();
				
				// we may have multiple states. need to sort out the order of precedence
				// without making multiple round trips out to the db. this gets a bit messy:

				boolean have_OS_UNCOMMITTED = false;
				boolean have_OS_UNCOMMITTED_HIDDEN = false;
				boolean have_OS_COMMITTED = false;
				boolean have_OS_COMMITTED_HIDDEN = false;
				
				while(  rs.next() ) {

					int stateStatus = rs.getInt(1);

					switch (stateStatus)
					{
						case ObjectStore.OS_UNCOMMITTED:
							have_OS_UNCOMMITTED = true;
							break;
						case ObjectStore.OS_COMMITTED:
							have_OS_COMMITTED = true;
							break;
						case ObjectStore.OS_COMMITTED_HIDDEN:
							have_OS_COMMITTED_HIDDEN = true;
							break;
						case ObjectStore.OS_UNCOMMITTED_HIDDEN:
							have_OS_UNCOMMITTED_HIDDEN = true;
							break;
					}
				}
				
				// examine in reverse order:
				if(have_OS_COMMITTED_HIDDEN) {
					theState = ObjectStore.OS_COMMITTED_HIDDEN;
				}
				if(have_OS_COMMITTED) {
					theState = ObjectStore.OS_COMMITTED;
				}				
				if(have_OS_UNCOMMITTED_HIDDEN) {
					theState = ObjectStore.OS_UNCOMMITTED_HIDDEN;
				}
				if(have_OS_UNCOMMITTED) {
					theState = ObjectStore.OS_UNCOMMITTED;
				}
			}
			catch (Exception e)
			{
				if(retryConnection(e, pool)) {
					return currentState(objUid, tName);
				} else {
				    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_3",e);

				    return ObjectStore.OS_UNKNOWN;
				}
			} finally {
				freePool(pool);
			}
		}

		return theState;
	}

	/**
	 * allObjUids - Given a type name, return an ObjectState that contains
	 * all of the uids of objects of that 
	 */
	public boolean allObjUids (String tName, InputObjectState state, int match) throws ObjectStoreException
	{
		
		int pool = getPool();
		try
		{
			Statement stmt = _theConnection[pool].createStatement();
			ResultSet rs = null;
			OutputObjectState store = new OutputObjectState();

			try
			{
				/*
				* Not used enough to warrant a PreparedStatement.
				*/

				rs = stmt.executeQuery("SELECT DISTINCT UidString FROM "+_jdbcTableName+" WHERE TypeName= '"+tName+"'");
			}
			catch (Exception e)
			{
			    if (tsLogger.arjLoggerI18n.isWarnEnabled())
				tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_4",e);

			    rs = null;
			}

			boolean finished = false;

			while ( !finished && rs.next() )
			{
				Uid theUid = null;

				try
				{
					theUid = new Uid(rs.getString(1));
					theUid.pack(store);
				}
				catch (IOException ex)
				{
				    if (tsLogger.arjLoggerI18n.isWarnEnabled())
					tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_5",ex);

				    return false;
				}
				catch (NullPointerException ex)
				{
				    if (tsLogger.arjLoggerI18n.isWarnEnabled())
					tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_4",ex);

				    finished = true;
				}
				catch (Exception e)
				{
				    if (tsLogger.arjLoggerI18n.isWarnEnabled())
					tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_4",e);

				    finished = true;
				}
			}
			try
			{
				Uid.nullUid().pack(store);
			}
			catch (IOException e)
			{
				throw new ObjectStoreException("oracle_9_0::allObjUids - could not pack end of list Uid.");
			}

			state.setBuffer(store.buffer());

			store = null;

			stmt.close();

			return true;
		}
		catch (Exception e)
		{
		    if (tsLogger.arjLoggerI18n.isWarnEnabled())
			tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_4",e);
		}
		finally
		{
			freePool(pool);
		}

		return false;
	}


	
	public boolean allTypes (InputObjectState foundTypes) throws ObjectStoreException
	{
		
		int pool = getPool();
		try
		{
			Statement stmt = _theConnection[pool].createStatement();
			ResultSet rs = null;
			OutputObjectState store = new OutputObjectState();

			try
			{
				/*
				* Not used enough to warrant a PreparedStatement.
				*/

				rs = stmt.executeQuery("SELECT DISTINCT TypeName FROM "+_jdbcTableName);
			}
			catch (Exception e)
			{
			    if (tsLogger.arjLoggerI18n.isWarnEnabled())
				tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_6",e);

				rs = null;
			}

			boolean finished = false;

			while ( !finished && rs.next() )
			{
				try
				{
					String type = rs.getString(1);
					store.packString(type);
				}
				catch (IOException ex)
				{
				    if (tsLogger.arjLoggerI18n.isWarnEnabled())
					tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_7",ex);

				    return false;
				}
				catch (NullPointerException ex)
				{
				    if (tsLogger.arjLoggerI18n.isWarnEnabled())
					tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_6",ex);

				    finished = true;
				}
				catch (Exception e)
				{
				    if (tsLogger.arjLoggerI18n.isWarnEnabled())
					tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_6",e);

				    finished = true;
				}
			}

			try
			{
				store.packString("");
			}
			catch (IOException e)
			{
				throw new ObjectStoreException("oracle_9_0::allTypes - could not pack end of list string.");
			}

			foundTypes.setBuffer(store.buffer());
			stmt.close();

			return true;
		}
		catch (Exception e)
		{
		    if (tsLogger.arjLoggerI18n.isWarnEnabled())
			tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_6",e);
		}
		finally {
			freePool(pool);
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
			if ((ft == ObjectStore.OS_COMMITTED) || (ft == ObjectStore.OS_UNCOMMITTED))
			{
				int pool = getPool();
				try
				{
					PreparedStatement pstmt = _preparedStatements[pool][READ_STATE];

					if (pstmt == null)
					{
						pstmt = _theConnection[pool].prepareStatement("SELECT ObjectState FROM "+_jdbcTableName+" WHERE UidString = ? AND TypeName = ? AND StateType = ?");

						_preparedStatements[pool][READ_STATE] = pstmt;
					}

					pstmt.setString(1, objUid.stringForm());
					pstmt.setString(2, tName);
					pstmt.setInt(3, ft);

					ResultSet rs = pstmt.executeQuery();

					if(! rs.next()) {
						return null; // no matching state in db
					}
					
					Blob myBlob = (Blob)rs.getBlob(1);
					byte[] buffer = myBlob.getBytes(1, (int)myBlob.length());

					rs.close();
					
					if (buffer != null)
					{
						newImage = new InputObjectState(objUid, tName, buffer);
					}
					else {
					    if (tsLogger.arjLoggerI18n.isWarnEnabled())
						tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_8");
					}
				}
				catch (Exception e)
				{
					if(retryConnection(e, pool)) {
						return read_state(objUid, tName, ft);
					} else {
						throw new ObjectStoreException(e.toString());
					}
				}
				finally {
					freePool(pool);
				}
			}
		}
		else
			throw new ObjectStoreException("oracle_9_0.read_state - object with uid "+objUid+" has no TypeName");

		return newImage;
	}


	/**
	 * Remove an object from the store.
	 *
	 */
	public boolean remove_state (Uid objUid, String name, int ft) throws ObjectStoreException
	{
		
		boolean removeOk = false;

		if (!storeValid())
			return false;

		if (name != null)
		{
			if ((ft == ObjectStore.OS_COMMITTED) || (ft == ObjectStore.OS_UNCOMMITTED))
			{
				int pool = getPool();
				try
				{
					PreparedStatement pstmt = _preparedStatements[pool][REMOVE_STATE];

					if (pstmt == null)
					{
						pstmt = _theConnection[pool].prepareStatement("DELETE FROM "+_jdbcTableName+" WHERE UidString = ? AND StateType = ?");

						_preparedStatements[pool][REMOVE_STATE] = pstmt;
					}

					pstmt.setString(1, objUid.stringForm());
					pstmt.setInt(2, ft);
					if(pstmt.executeUpdate() > 0) {
						removeOk = true;
					}
				}
				catch (Exception e)
				{
					if(retryConnection(e, pool)) {
						return remove_state(objUid, name, ft);
					} else {
					    if (tsLogger.arjLoggerI18n.isWarnEnabled())
						tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_9", e);

					    removeOk = false;
					}
				}
				finally
				{
					freePool(pool);
				}
			}
			else
			{
				removeOk = false;
				// can only remove (UN)COMMITTED objs
				if (tsLogger.arjLoggerI18n.isWarnEnabled())
				{
				    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_10", new Object[]{ft, objUid});
				}
			}
		}
		else
		{
			removeOk = false;

			if (tsLogger.arjLoggerI18n.isWarnEnabled())
			{
			    tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_11",
							new Object[]{objUid});
			}

		}

		return removeOk;
	}
	
	
	public boolean write_state (Uid objUid, String tName, OutputObjectState state, int s) throws ObjectStoreException
	{
		boolean result = false;

		int imageSize = (int) state.length();

		if (imageSize > oracle_9_0._maxStateSize)
			throw new ObjectStoreException("Object state is too large - maximum size allowed: "+oracle_9_0._maxStateSize);

		byte[] b = state.buffer();

		if (imageSize > 0 && storeValid())
		{
			int pool = getPool();
			try
			{
				ResultSet rs = null;

				PreparedStatement pstmt = _preparedStatements[pool][READ_WRITE_SHORTCUT];

				_theConnection[pool].setAutoCommit(false);

				if (pstmt == null)
				{
					pstmt = _theConnection[pool].prepareStatement("SELECT ObjectState FROM "+_jdbcTableName+" WHERE UidString = ? AND StateType = ? FOR UPDATE", ResultSet.TYPE_FORWARD_ONLY, ResultSet.CONCUR_UPDATABLE);
					_preparedStatements[pool][READ_WRITE_SHORTCUT] = pstmt;
				}

				pstmt.setString(1, objUid.stringForm());
				pstmt.setInt(2, s);

				rs = pstmt.executeQuery();

				if( rs.next() ) {

					BLOB myBlob = (BLOB)rs.getBlob(1);
					myBlob.putBytes(1, b);

				} else {
					// not in database, do insert:
					PreparedStatement pstmt2 = _preparedStatements[pool][WRITE_STATE_NEW];

					if (pstmt2 == null)
					{
						pstmt2 = _theConnection[pool].prepareStatement("INSERT INTO "+_jdbcTableName+" (StateType,TypeName,UidString,ObjectState) VALUES (?,?,?,empty_blob())");

						_preparedStatements[pool][WRITE_STATE_NEW] = pstmt2;
					}

					pstmt2.setInt(1, s);
					pstmt2.setString(2, tName);
					pstmt2.setString(3, objUid.stringForm());

					pstmt2.executeUpdate();
					_theConnection[pool].commit();

					PreparedStatement pstmt3 = _preparedStatements[pool][SELECT_FOR_WRITE_STATE];
					if(pstmt3 == null) {
						pstmt3 = _theConnection[pool].prepareStatement("SELECT ObjectState FROM "+_jdbcTableName+" WHERE UidString = ? AND StateType = ? FOR UPDATE");
						_preparedStatements[pool][SELECT_FOR_WRITE_STATE] = pstmt3;
					}

					pstmt3.setString(1, objUid.stringForm());
					pstmt3.setInt(2, s);
					ResultSet rs3 = pstmt3.executeQuery();
					rs3.next();
					BLOB myBlob = (BLOB)rs3.getBlob(1);
					myBlob.putBytes(1, b);
					rs3.close();
				}

				rs.close();
				_theConnection[pool].commit();
				result = true;

			}
			catch(Exception e)
			{
				if(retryConnection(e, pool)) {
					return write_state(objUid, tName, state, s);
				} else {
				    if (tsLogger.arjLoggerI18n.isWarnEnabled())
					tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_12",e);
				}
			}
			finally
			{
				try
				{
					_theConnection[pool].setAutoCommit(true);
				}
				catch(Exception e) {}
				freePool(pool);
			}
		}
		return result;
	}



	/**
	 * retryConnection. Called in exeption handlers where the problem
	 * may be due to use of a stale (broken) cached connection. If this
	 * is hte case, we re-establish the connection before returning.
	 * @param e The exception, which may be due to a bad connection.
	 * @param pool The pooled connection which was in use when the
	 *  exception was thrown and which is therfore suspect.
	 * @return true if the connection was reestablished
	 *  (in which case it is worth retrying the calling function),
	 *  false is a broken connection was unlikely to be the problem.
	 * 
	 */
	protected synchronized boolean retryConnection(Exception e, int pool) {
				
		// not possible in ArjunaCore - only in HP-MS
		
		/*
		if(e instanceof SQLException && !_isRefreshed[pool]) {
			// ToTo: Look for specific Oracle error codes here...
			
			try {
				reconnect(pool);
			} catch(Exception e1) {
				return false;
			}
			
			// tell isFree that it's OK to free this one twice:
			_isRefreshed[pool] = true;
			freePool(pool);
		}
		*/
		
		return false;
	}
	
	/**
	 * reconnect(int pool): re-establish a potentially failed cached connection.
	 */
	protected void reconnect(int pool) throws SQLException {
		// just in case the connection is still live,
		// attempt to clean it up nicely:

		// not possible in ArjunaCore - only in HP-MS.
		return;
		
		/*
		try {
			_theConnection[pool].close();
			_theConnection[pool] = null;
		} catch(SQLException e) {
		}
		
		// release the statements associated with the closed
		// connection so they dont get used by mistake...
		for(int i = 0; i < STATEMENT_SIZE; i++) {
			_preparedStatements[pool][i] = null;
		}
		
		// re-establish the connection:
		_theConnection[pool] = DriverManager.getConnection(_connectionURL);
		_theConnection[pool].setAutoCommit(true);
		*/
	}


	/**
	 * Set up the store for use.
	 */
	public boolean initialise(Connection conn, String tableName, boolean dropTable)
	{
		_theConnection = new Connection[_poolSize];
		_theConnection[0] = conn;
		
		if(_inUse == null) {
			_inUse = new boolean[_poolSize];
		}
		if(_isRefreshed == null) {
			_isRefreshed = new boolean[_poolSize];
		}

		_preparedStatements = new PreparedStatement[_poolSize][];
		for(int i = 0; i < _poolSize; i++) {

			_preparedStatements[i] = new PreparedStatement[oracle_9_0.STATEMENT_SIZE];

			for (int j = 0; j < oracle_9_0.STATEMENT_SIZE; j++) {
				_preparedStatements[i][j] = null;
			}
		}

		_jdbcTableName = tableName;
		_dropTable = dropTable;

		try
		{
			Statement stmt = _theConnection[0].createStatement();

			// table [type, object UID, format, blob]

			// Need some management interface to delete the table!

			if (_dropTable)
			{
				try
				{
					stmt.executeUpdate("DROP TABLE "+_jdbcTableName);
				}
				catch (SQLException ex)
				{
					// don't want to print error - chances are it
					// just reports that the table does not exist
					// ex.printStackTrace();
				}
			}

			try
			{
				stmt.executeUpdate("CREATE TABLE "+_jdbcTableName+" (StateType INTEGER, TypeName VARCHAR(1024),UidString VARCHAR(255), ObjectState BLOB, CONSTRAINT "+_jdbcTableName+"_pk PRIMARY KEY(UidString, StateType))");
			}
			catch(SQLException ex) {
				// assume this is reporting that the table already exists:
				// ex.printStackTrace();
			}
			
			_isValid = true;
		}
		catch (Exception e)
		{
		    if (tsLogger.arjLoggerI18n.isWarnEnabled())
			tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_13",e);

		    _isValid = false;
		}

		return _isValid;
	}

public String name ()
    {
	return "oracle_9_0";
    }

	protected String _jdbcTableName = null;
	protected Connection[] _theConnection = null;
	protected boolean _isValid = false;
	protected PreparedStatement[][] _preparedStatements = null;

	// private static final int  _maxStateSize = 65535;
	// Oracle BLOBs should be OK up to > 4 GB, but cap @ 10 MB for testing:
	private static final int _maxStateSize = 1024 * 1024 * 10;

	/*
	* IMPORTANT: remember to update if we add more prepared statements!
	*/

	private static final int COMMIT_STATE = 0;
	private static final int HIDE_STATE = 1;
	private static final int REVEAL_STATE = 2;
	private static final int CURRENT_STATE = 3;
	private static final int READ_STATE = 4;
	private static final int REMOVE_STATE = 5;
	private static final int WRITE_STATE_NEW = 6;
	private static final int WRITE_STATE_EXISTING = 7;
	private static final int SELECT_FOR_WRITE_STATE = 8;
	private static final int READ_WRITE_SHORTCUT = 9;
	private static final int PRECOMMIT_CLEANUP = 10;
	// size:
	private static final int STATEMENT_SIZE = 11;

	// this should be determined at runtime ?:
	// increasing it would require redesign of initialise...
	protected int _poolSize = 1;

	// record the status of each connection in the pool:
	protected boolean[] _inUse = null;
	protected boolean[] _isRefreshed = null;
	
	//protected String _connectionURL = null;

	protected boolean _dropTable = false;

	/**
	 * The name of this class. Used as the resource bundle name and log channel.
	 */
	//public static final String CLASS = oracle_9_0.class.getName();

	/**
	 * The default log channel, established as the class name.
	 */
	
	/**
	 * Allocate a database connection from the pool:
	 * Walks the array and allocates the first available connection.
	 * If non are free, sleeps for 1 second before trying again.
	 * (This is a bit of a kludge - should really use wait/notify...)
	 */
	protected int getPool() {
		int i = 0;
		int rtn = -1;

		while(true) {
			synchronized(_inUse) {
				if(! _inUse[i]) {
					_inUse[i] = true;
					rtn = i;
					break;
				}
			}

			i++;
			if(i == _poolSize) {
				i = 0;
				try {
					Thread.sleep(1000);
				} catch(InterruptedException ie) {
				}
			}
		}

		return rtn;
	}

	/**
	 * return a connection to the free pool:
	 *
	 */
	protected void freePool (int pool) {
		/*
		try {
		_theConnection[pool].commit();
		} catch(Exception e) {}
		*/

		synchronized(_inUse) {
			if(_inUse[pool] == false) {
				// this is OK if and only if the connection has just 
				// been reestablished...
				if(_isRefreshed[pool]) {
					_isRefreshed[pool] = false;
				} else {
				    if (tsLogger.arjLoggerI18n.isWarnEnabled())
					tsLogger.arjLoggerI18N.warn("com.arjuna.ats.internal.arjuna.objectstore.jdbc.oracle_9_0_14");
				}
			}
			_inUse[pool] = false;
		}
	}


}
