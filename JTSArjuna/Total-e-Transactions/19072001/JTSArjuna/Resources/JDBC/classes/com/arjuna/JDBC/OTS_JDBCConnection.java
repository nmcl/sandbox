/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_JDBCConnection.javatmpl,v 1.17.4.1.4.1.10.1.6.1.2.1.2.1.2.1 2000/12/21 11:21:34 nmcl Exp $
 */

/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OrbPortability.h,v 1.1 2000/02/25 14:09:59 nmcl Exp $
 */




/*
 * Try to get around the differences between Ansi CPP and
 * K&R cpp with concatenation.
 */


/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OrbPortability_stdc.h,v 1.2.4.1.2.3.26.1.2.2.4.2 2001/02/02 11:51:52 nmcl Exp $
 */
































































package com.arjuna.JDBC;

import com.arjuna.OrbCommon.*;
import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.CosTransactions.OTS;
import com.arjuna.CosTransactions.ArjunaOTS.AtomicTransaction;
import com.arjuna.CosTransactions.ArjunaOTS.TopLevelTransaction;
import org.omg.CosTransactions.*;
import java.util.*;
import java.sql.*;

import org.omg.CosTransactions.Unavailable;
import org.omg.CosTransactions.NotPrepared;
import org.omg.CosTransactions.HeuristicRollback;
import org.omg.CosTransactions.HeuristicCommit;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CosTransactions.SubtransactionsUnavailable;
import org.omg.CosTransactions.NoTransaction;
import org.omg.CORBA  .SystemException;
import java.sql.SQLException;

/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranExceptions.h,v 1.1.2.1.28.1.14.1 2001/03/20 16:24:46 nmcl Exp $
 */





/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: CosTranNames.h,v 1.1.42.1 2001/03/20 16:24:47 nmcl Exp $
 */




/*
 * Simple renaming macros for portability.
 *
 * Slightly different from C++ version in that we don't need the
 * typedefs. However, we do need some mapping for OTS exceptions which
 * should now be system exceptions.
 */

/*
 * CosTransaction module exceptions
 */






/*
 * We need to check this. WRONG_TRANSACTION is supposed to
 * be a system exception (in the latest draft of the OTS),
 * but two ORBs have it as a user exception.
 */





/**/





/*
 * An attempt at a portable way of including the OTS exceptions,
 * which are either mapped as system exceptions or user exceptions,
 * depending upon the Orb.
 */



import org.omg.CosTransactions.WrongTransaction;

import org.omg.CORBA.INVALID_TRANSACTION;
import org.omg.CORBA.TRANSACTION_REQUIRED;
import org.omg.CORBA.TRANSACTION_ROLLEDBACK;





/**
 * JDBC 1.0 has no notion of assigning a txid to a given operation,
 * so all accesses through the same connection are implicitly associated
 * with the same transaction in the db.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_JDBCConnection.javatmpl,v 1.17.4.1.4.1.10.1.6.1.2.1.2.1.2.1 2000/12/21 11:21:34 nmcl Exp $
 * @since JTS 1.2.4.
 */

public class OTS_JDBCConnection implements java.sql.Connection
{

public OTS_JDBCConnection (String url, Properties info) throws SQLException
    {
	_theConnections = new Hashtable();
	_defaultConnection = DriverManager.getConnection(url, info);

	/*
	 * Any requirement for branding?
	 */

	JDBCBranding theBrander = JDBCBrandingFactory.getBranding(url);
		
	if (theBrander != null)
	    theBrander.brand(_defaultConnection);

	_url = url;
	_info = info;
    }

public void finalize ()
    {
	abortConnections();
	
	try
	{
	    close();
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	}
	
	_theConnections.clear();
	_theConnections = null;
    }
    
public Statement createStatement () throws SQLException
    {
	return getConnection().createStatement();
    }


public Statement createStatement (int rs, int rc) throws SQLException
    {
	return getConnection().createStatement(rs, rc);
    }    

    
public PreparedStatement prepareStatement (String sql) throws SQLException
    {
	return getConnection().prepareStatement(sql);
    }


public PreparedStatement prepareStatement (String sql, int rs, int rc) throws SQLException
    {
	return getConnection().prepareStatement(sql, rs, rc);
    }

    
public CallableStatement prepareCall (String sql) throws SQLException
    {
	return getConnection().prepareCall(sql);
    }


public CallableStatement prepareCall (String sql, int rs, int rc) throws SQLException
    {
	return getConnection().prepareCall(sql, rs, rc);
    }


public String nativeSQL (String sql) throws SQLException
    {
	return getConnection().nativeSQL(sql);
    }


public Map getTypeMap () throws SQLException
    {
	return getConnection().getTypeMap();
    }

public void setTypeMap (Map map) throws SQLException
    {
	getConnection().setTypeMap(map);
    }

    
    /*
     * Not allowed.
     */
    
public void setAutoCommit (boolean autoCommit) throws SQLException
    {
	getConnection().setAutoCommit(autoCommit);
    }

public boolean getAutoCommit () throws SQLException
    {
	return getConnection().getAutoCommit();
    }

public void commit () throws SQLException
    {
	getConnection().commit();
    }

public void rollback () throws SQLException
    {
	getConnection().rollback();
    }

    /**
     * If there are any transactions running when close is called then we
     * throw an exception and mark them as rollback only.
     */
    
public void close () throws SQLException
    {
	if (_theConnections.size() == 0)
	{
	    _defaultConnection.close();

	    JDBCConnectionManager.remove(this);  // finalize?
	}
    }

public boolean isClosed () throws SQLException
    {
	if (_theConnections.size() == 0)
	    return true;
	else
	    return false;
    }
    
public DatabaseMetaData getMetaData () throws SQLException
    {
	return getConnection().getMetaData();
    }

    /**
     * Can only set readonly before we use the connection in a
     * given transaction!
     */
    
public void setReadOnly (boolean ro) throws SQLException
    {
	getConnection().setReadOnly(ro);
    }
    
public boolean isReadOnly () throws SQLException
    {
	return getConnection().isReadOnly();
    }

public void setCatalog (String cat) throws SQLException
    {
	getConnection().setCatalog(cat);
    }
	    
public String getCatalog () throws SQLException
    {
	return getConnection().getCatalog();
    }

public void setTransactionIsolation (int iso) throws SQLException
    {
	getConnection().setTransactionIsolation(iso);
    }

public int getTransactionIsolation () throws SQLException
    {
	return getConnection().getTransactionIsolation();
    }

public SQLWarning getWarnings () throws SQLException
    {
	return getConnection().getWarnings();
    }
    
public void clearWarnings () throws SQLException
    {
	getConnection().clearWarnings();
    }

synchronized java.sql.Connection getConnection () throws SQLException
    {
	Control cont = null;
	
	try
	{
	    cont = checkTransaction();
	}
	catch (Unavailable e)
	{
	    /*
	     * No transaction, so ok to proceed.
	     */
	}

	Coordinator theCoord = null;

	try
	{
	    theCoord = cont.get_coordinator();
	}
	catch (Exception e)
	{
	    cont = null;
	}

	if (cont == null)
	{
	    return _defaultConnection;
	}
	else
	{
	    Enumeration e = _theConnections.keys();
		
	    while (e.hasMoreElements())
	    {
		try
		{
		    Control c = (Control) e.nextElement();
		    
		    if (c != null)
		    {
			Coordinator coord = c.get_coordinator();
			
			if (coord.is_same_transaction(theCoord))
			    return (java.sql.Connection) _theConnections.get(c);
		    }
		}
		catch (Exception ex)
		{
		    throw new SQLException(ex.toString());
		}
	    }
	    
	    /*
	     * If here we have not found a connection for
	     * the current transaction. So, create one.
	     */

	    JDBCConnection con = new JDBCConnection(_url, _info);
	    
	    _theConnections.put(cont, con);
	    
	    return con;
	}
    }
   
String connectionUrl ()
    {
	return _url;
    }

Properties connectionProperties ()
    {
	return _info;
    }

private final synchronized Control checkTransaction () throws SQLException, Unavailable
    {
	try
	{
	    Control c = OTS.current().get_control();

	    if (c != null)
	    {
		Coordinator coord = c.get_coordinator();
	    
		if (!coord.is_top_level_transaction())
		    throw new SubtransactionsUnavailable();
		
		coord = null;

		return c;
	    }
	    else
		throw new Unavailable();
	}
	catch (Unavailable e1)
	{
	    throw e1;
	}
	catch (SubtransactionsUnavailable e2)
	{
	    throw new SQLException("OTS_JDBCConnection - subtransactions unavailable!");
	}
	catch (Exception e3)
	{
	    throw new SQLException(e3.toString());
	}
    }

private final void abortConnections ()
    {
	Enumeration e = _theConnections.elements();
	
	while (e.hasMoreElements())
	{
	    /*
	     * Only bother with transactional connections.
	     */

	    JDBCConnection con = (JDBCConnection) e.nextElement();
	    
	    try
	    {
		if (con != null)
		    con.close();
	    }
	    catch (Exception ex)
	    {
		ex.printStackTrace();
	    }
	}
    }

private Hashtable  _theConnections;
private Connection _defaultConnection;  // for no transaction connections
private String     _url;
private Properties _info;    
};

