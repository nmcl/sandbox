/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: JDBC2Test.java,v 1.2.4.1.4.1.2.1.8.1.8.1.36.1.4.1 2001/06/28 14:15:25 nmcl Exp $
 */

import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.JDBC2.*;
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

public class JDBC2Test
{

public static final int CLOUDSCAPE = 0;
public static final int ORACLE = 1;
public static final int SEQUELINK = 2;
    
public static void main (String[] args)
    {
	int dbType = CLOUDSCAPE;
	boolean commit = false;
	boolean nested = false;
	boolean reuseconn = false;
	boolean clean = false;
	boolean create = false;
	String url = null;
	String user = "test";
	String password = "test";
	String dynamicClass = "com.arjuna.JDBC2.drivers.cloudscape_3_5";
	String host = null;
	String port = null;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-oracle") == 0)
	    {
		dbType = ORACLE;
		user = "tester";
		password = "tester";
		dynamicClass = "com.arjuna.JDBC2.drivers.oracle_8_1_6";
	    }
	    if (args[i].compareTo("-sequelink") == 0)
	    {
		dbType = SEQUELINK;
		user = "tester";
		password = "tester";
		dynamicClass = "com.arjuna.JDBC2.drivers.sequelink_5_0";
	    }
	    if (args[i].compareTo("-host") == 0)
		host = args[i+1];
	    if (args[i].compareTo("-port") == 0)
		port = args[i+1];
	    if (args[i].compareTo("-commit") == 0)
		commit = true;
	    if (args[i].compareTo("-nested") == 0)
		nested = true;
	    if (args[i].compareTo("-clean") == 0)
		clean = true;
	    if (args[i].compareTo("-create") == 0)
		create = true;
	    if (args[i].compareTo("-reuseconn") == 0)
		reuseconn = true;
	    if (args[i].compareTo("-url") == 0)
		url = args[i+1];
	    if (args[i].compareTo("-dynamicClass") == 0)
		dynamicClass = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: JDBCTest2 [-commit] [-nested] [-reuseconn] [-oracle] [-sequelink] [-cloudscape] [-url] [-dynamicClass]");
		System.exit(0);
	    }
	}
	
	try
	{
	    ORBInterface.initORB(args, null);
	    OAInterface.initOA();
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	    
	    System.exit(0);
	}

	if (url == null)
	{
	    switch (dbType)
	    {
	    case CLOUDSCAPE:
		url = "jdbc:arjuna:cloudscape:mysql;create=true";
		break;
	    case SEQUELINK:
		{
		    url = "jdbc:arjuna_jdbc1:sequelink://"+host;

		    if (port != null)
			url = url + ":" + port;
		}
		break;
	    case ORACLE:
		{
		    if (port == null)
			url = "jdbc:arjuna:oracle:thin:@"+host+":JDBCTest";
		    else
			url = "jdbc:arjuna:oracle:thin:@"+host+":"+port+":JDBCTest";
		}
		break;
	    default:
		// noop
	    }

	}
	
	/*
	 * Register the driver via the system properties variable 
	 * "jdbc.drivers"
	 */ 

	Properties p = System.getProperties();

	switch (dbType)
	{
	case CLOUDSCAPE:
	    p.put("jdbc.drivers", "COM.cloudscape.core.JDBCDriver"); 
	    break;
	case ORACLE:
	    p.put("jdbc.drivers", "oracle.jdbc.driver.OracleDriver");
	    break;
	case SEQUELINK:
	    p.put("jdbc.drivers", "com.merant.sequelink.jdbc.SequeLinkDriver");
	    break;
	}

	System.setProperties(p);

	try
	{
	    DriverManager.registerDriver(new ArjunaJDBC2Driver());
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	    System.exit(0);
	}

	Connection conn = null;
	Connection conn2 = null;
	Statement stmt = null;  // non-tx statement
	Statement stmtx = null;	// will be a tx-statement
	Properties dbProperties = new Properties();
	    
	try
	{
	    System.out.println("\nCreating connection to database: "+url);
		
	    dbProperties.put(ArjunaJDBC2Driver.userName, user);
	    dbProperties.put(ArjunaJDBC2Driver.password, password);
	    dbProperties.put(ArjunaJDBC2Driver.dynamicClass, dynamicClass);

	    conn = DriverManager.getConnection(url, dbProperties);
	    conn2 = DriverManager.getConnection(url, dbProperties);
	    stmt = conn.createStatement();  // non-tx statement

	    try
	    {
		if (clean)
		{
		    stmt.executeUpdate("DROP TABLE test_table");
		    stmt.executeUpdate("DROP TABLE test_table2");
		}
	    
		if (clean || create)
		{
		    stmt.executeUpdate("CREATE TABLE test_table (a INTEGER,b INTEGER)");
		    stmt.executeUpdate("CREATE TABLE test_table2 (a INTEGER,b INTEGER)");
		}
	    }
	    catch (Exception e)
	    {
		// assume already done.
	    }
	}
	catch (SQLException e)
	{
	    e.printStackTrace();
	    System.exit(0);
	}
	
	try
	{
	    System.out.println("Starting top-level transaction.");
	    
	    OTS.current().begin();

	    if (nested)
	    {
		System.out.println("Starting nested transaction.");
		
		OTS.current().begin();
	    }

	    stmtx = conn.createStatement(); // will be a tx-statement
	    
	    try
	    {
		System.out.println("\nAdding entries to table 1.");
		
		stmtx.executeUpdate("INSERT INTO test_table (a, b) VALUES (1,2)");

		ResultSet res1 = null;

		System.out.println("\nInspecting table 1.");
		
		try
		{
		    res1 = stmtx.executeQuery("SELECT * FROM test_table");
		}
		catch (Exception e)
		{
		    res1 = null;
		}

		try
		{
		    while (res1.next())
		    {
			System.out.println("Column 1: "+res1.getInt(1));
			System.out.println("Column 2: "+res1.getInt(2));
		    }
		}
		catch (Exception e)
		{
		    System.out.println("null");
		}

		System.out.println("\nAdding entries to table 2.");

		stmtx.executeUpdate("INSERT INTO test_table2 (a, b) VALUES (3,4)");
		
		try
		{
		    res1 = stmtx.executeQuery("SELECT * FROM test_table2");
		}
		catch (Exception e)
		{
		    res1 = null;
		}

		System.out.println("\nInspecting table 2.");

		try
		{
		    while (res1.next())
		    {
			System.out.println("Column 1: "+res1.getInt(1));
			System.out.println("Column 2: "+res1.getInt(2));
		    }
		}
		catch (Exception e)
		{
		    System.out.println("null");
		}
	    }
	    catch (SQLException e)
	    {
		System.err.println(e);

		OTS.current().rollback();

		if (nested)
		    OTS.current().rollback();

		try
		{
		    conn.close();
		}
		catch (Exception ex)
		{
		}
		
		System.exit(0);
	    }
	    catch (Exception e)
	    {
		if (nested)
		    OTS.current().rollback();
		
		OTS.current().rollback();
		
		System.err.println(e);

		try
		{
		    conn.close();
		}
		catch (Exception ex)
		{
		}

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
	    
	    System.out.println("\nNow checking state of table 1.");

	    OTS.current().begin();

	    if (!reuseconn)
	    {
		conn = DriverManager.getConnection(url, dbProperties);
	    }

	    stmtx = conn.createStatement();

	    ResultSet res2 = null;

	    try
	    {
		res2 = stmtx.executeQuery("SELECT * FROM test_table");
	    }
	    catch (Exception e)
	    {
		res2 = null;
	    }

	    try
	    {
		while (res2.next())
		{
		    System.out.println("Column 1: "+res2.getInt(1));
		    System.out.println("Column 2: "+res2.getInt(2));
		}
	    }
	    catch (Exception e)
	    {
		System.out.println("null");
	    }

	    OTS.current().commit(true);

	    OTS.current().begin();

	    if (!reuseconn)
	    {
		conn = DriverManager.getConnection(url, dbProperties);
	    }

	    System.out.println("\nNow checking state of table 2.");

	    stmtx = conn.createStatement();

	    try
	    {
		res2 = stmtx.executeQuery("SELECT * FROM test_table2");
	    }
	    catch (Exception e)
	    {
		res2 = null;
	    }

	    try
	    {
		while (res2.next())
		{
		    System.out.println("Column 1: "+res2.getInt(1));
		    System.out.println("Column 2: "+res2.getInt(2));
		}
	    }
	    catch (Exception e)
	    {
		System.out.println("null");
	    }

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

	try
	{
	    conn.close();
	}
	catch (Exception e)
	{
	}

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();

	System.out.println("Test completed successfully.");
	System.exit(0);
    }
    
};
