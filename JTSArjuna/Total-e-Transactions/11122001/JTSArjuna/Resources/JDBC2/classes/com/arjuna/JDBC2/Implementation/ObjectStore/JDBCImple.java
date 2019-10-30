/*
 * Copyright (C) 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBCImple.java,v 1.1.2.1 2001/09/28 10:29:23 nmcl Exp $
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

public interface JDBCImple
{

public boolean commit_state (Uid objUid, String tName) throws ObjectStoreException;
public boolean hide_state (Uid objUid, String tName) throws ObjectStoreException;
public boolean reveal_state (Uid objUid, String tName) throws ObjectStoreException;

public int currentState (Uid objUid, String tName) throws ObjectStoreException;

public boolean allObjUids (String tName, InputObjectState state, int match) throws ObjectStoreException;
public boolean allTypes (InputObjectState foundTypes) throws ObjectStoreException;
public InputObjectState read_state (Uid objUid, String tName, int ft) throws ObjectStoreException;
public boolean remove_state (Uid objUid, String name, int ft) throws ObjectStoreException;
public boolean write_state (Uid objUid, String tName, OutputObjectState state, int s) throws ObjectStoreException;

public boolean initialise (Connection conn, String tableName, boolean dropTable) throws SQLException;

}
