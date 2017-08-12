/*
 * Copyright (C) 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: JDBCTest.java,v 1.1.2.1 1999/02/12 09:47:10 nmcl Exp $
 */

import com.arjuna.JDBC.JDBCResourceManager;
import com.arjuna.JavaCosTransactions.OTS;
import com.arjuna.OrbCommon.*;
import java.util.Properties;
import java.sql.*;	

import java.lang.IllegalAccessException;
import org.omg.CosTransactions.Unavailable;
import org.omg.CosTransactions.NotPrepared;
import org.omg.CosTransactions.HeuristicRollback;
import org.omg.CosTransactions.HeuristicCommit;
import org.omg.CosTransactions.HeuristicMixed;
import org.omg.CosTransactions.HeuristicHazard;
import org.omg.CosTransactions.SubtransactionsUnavailable;
import org.omg.CosTransactions.NoTransaction;

public class JDBCTest
{

public static final int MYSQL = 0;
public static final int CLOUDSCAPE = 1;
    
public static void main (String[] args)
    {
	int dbType = MYSQL;
	boolean commit = false;
	boolean nested = false;
	boolean reuseconn = false;
	String url = "jdbc:mysql://glororan:3306/mysql";
	String user = "test";
	String password = "test";
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-mysql") == 0)
	    {
		dbType = MYSQL;
		url = "jdbc:mysql://glororan:3306/mysql";
	    }
	    if (args[i].compareTo("-cloudscape") == 0)
	    {
		dbType = CLOUDSCAPE;
		url = "jdbc:cloudscape:mysql;create=true";
	    }
	    if (args[i].compareTo("-commit") == 0)
		commit = true;
	    if (args[i].compareTo("-nested") == 0)
		nested = true;
	    if (args[i].compareTo("-reuseconn") == 0)
		reuseconn = true;
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: JDBCTest [-commit] [-nested] [-reuseconn] [-mysql] [-cloudscape]");
		System.exit(0);
	    }
	}
	
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
	
	JDBCResourceManager manager = JDBCResourceManager.create();
	
	/*
	 * Register the driver via the system properties variable 
	 * "jdbc.drivers"
	 */ 

	Properties p = System.getProperties(); 
	
	switch (dbType)
	{
	case MYSQL:
	    p.put("jdbc.drivers", "org.gjt.mm.mysql.Driver"); 
	    break;
	case CLOUDSCAPE:
	    p.put("jdbc.drivers", "COM.cloudscape.core.JDBCDriver"); 
	    break;
	}

	System.setProperties(p);
	
	try
	{
	    System.out.println("Starting top-level transaction.");
	    
	    OTS.current().begin();

	    if (nested)
	    {
		System.out.println("Starting nested transaction.");
		
		OTS.current().begin();
	    }
	    
	    Connection conn = null;
	    Statement stmt = null;
	    
	    try
	    {
		System.out.println("\nCreating connection to database.");
		
		conn = manager.connect(url, user, password);
		stmt = conn.createStatement();

		try
		{
		    stmt.executeUpdate("CREATE TABLE test_table (a INTEGER,b INTEGER)");
		}
		catch (SQLException e)
		{
		    // assume table already exists.
		}

		System.out.println("\nAdding entries to table.");
		
		stmt.executeUpdate("INSERT INTO test_table (a, b) VALUES (1,2)");
		
		ResultSet res1 = stmt.executeQuery("SELECT * FROM test_table");

		System.out.println("\nInspecting table.");
		
		if (res1 != null)
		{
		    res1.next();
		    
		    if (res1.wasNull())
			System.out.println("null");
		    else
		    {
			System.out.println("Column 1: "+res1.getInt(1));
			System.out.println("Column 2: "+res1.getInt(2));
		    }
		}
		else
		    System.out.println("null");
	    }
	    catch (SubtransactionsUnavailable e)
	    {
		System.err.println("Error - attempt to create connection within subtransaction.");

		OTS.current().rollback();
		OTS.current().rollback();
	    }
	    catch (Exception e)
	    {
		if (nested)
		    OTS.current().rollback();
		
		OTS.current().rollback();
		
		System.err.println(e);

		System.exit(0);
	    }

	    System.out.print("\nNow attempting to ");
	    
	    if (commit)
	    {
		System.out.print("commit ");
		
		OTS.current().commit(true);
	    }
	    else
	    {
		System.out.print("rollback ");
		
		OTS.current().rollback();
	    }

	    System.out.println("changes.");
	    
	    System.out.println("\nNow checking state of table.");

	    OTS.current().begin();

	    if (!reuseconn)
	    {
		conn = manager.connect(url, user, password);
		stmt = conn.createStatement();
	    }
		
	    ResultSet res2 = stmt.executeQuery("SELECT * FROM test_table");

	    if (res2 != null)
	    {
		res2.next();
		    
		if (res2.wasNull())
		    System.out.println("null");
		else
		{
		    System.out.println("Column 1: "+res2.getInt(1));
		    System.out.println("Column 2: "+res2.getInt(2));
		}
	    }
	    else
		System.out.println("null");

	    OTS.current().commit(true);
	}
	catch (Exception ex)
	{
	    try
	    {
		OTS.current().rollback();
	    }
	    catch (Exception exp)
	    {
	    }
	    
	    System.err.println(ex);
	}
    }
    
};
