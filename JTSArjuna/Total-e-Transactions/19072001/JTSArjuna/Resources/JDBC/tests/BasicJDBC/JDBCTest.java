/*
 * Copyright (C) 1998, 1999, 2000, 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JDBCTest.java,v 1.4.4.1.24.1.36.1.4.1 2001/06/28 14:14:34 nmcl Exp $
 */

import com.arjuna.CosTransactions.*;
import com.arjuna.OrbCommon.*;
import com.arjuna.JDBC.*;
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
public static final int SEQUELINK = 2;
public static final int ORACLE = 3;
    
public static void main (String[] args)
    {
	int dbType = MYSQL;
	boolean commit = false;
	boolean nested = false;
	boolean reuseconn = false;
	boolean clean = false;
	boolean create = false;
	String url = null;
	String user = "test";
	String password = "test";
	String host = null;
	String port = null;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-mysql") == 0)
	    {
		dbType = MYSQL;
	    }
	    if (args[i].compareTo("-cloudscape") == 0)
	    {
		dbType = CLOUDSCAPE;
	    }
	    if (args[i].compareTo("-sequelink") == 0)
	    {
		dbType = SEQUELINK;
		user = "tester";
		password = "tester";
	    }
	    if (args[i].compareTo("-oracle") == 0)
	    {
		dbType = ORACLE;
		user = "tester";
		password = "tester";
	    }
	    if (args[i].compareTo("-clean") == 0)
		clean = true;
	    if (args[i].compareTo("-url") == 0)
		url = args[i+1];
	    if (args[i].compareTo("-host") == 0)
		host = args[i+1];
	    if (args[i].compareTo("-port") == 0)
		port = args[i+1];
	    if (args[i].compareTo("-create") == 0)
		create = true;
	    if (args[i].compareTo("-commit") == 0)
		commit = true;
	    if (args[i].compareTo("-nested") == 0)
		nested = true;
	    if (args[i].compareTo("-reuseconn") == 0)
		reuseconn = true;
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: JDBCTest [-commit] [-nested] [-reuseconn] [-clean] [-create] [-mysql] [-cloudscape] [-sequelink] [-oracle] [-url <url>] [-host <host>] [-port <port>]");
		System.exit(0);
	    }
	}

	if (url == null)
	{
	    switch (dbType)
	    {
	    case MYSQL:
		{
		    if (port == null)
			url = "jdbc:arjuna_jdbc1:mysql://"+host+"/mysql";
		    else
			url = "jdbc:arjuna_jdbc1:mysql://"+host+":"+port+"/mysql";
		}
		break;
	    case CLOUDSCAPE:
		url = "jdbc:arjuna_jdbc1:cloudscape:mysql;create=true";
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
			url = "jdbc:arjuna_jdbc1:oracle:thin:@"+host+":JDBCTest";
		    else
			url = "jdbc:arjuna_jdbc1:oracle:thin:@"+host+":"+port+":JDBCTest";
		}
		break;
	    default:
		// noop
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
	case SEQUELINK:
	    p.put("jdbc.drivers", "com.merant.sequelink.jdbc.SequeLinkDriver");
	    break;
	case ORACLE:
	    p.put("jdbc.drivers", "oracle.jdbc.driver.OracleDriver");
	    break;
	}

	System.setProperties(p);

	try
	{
	    DriverManager.registerDriver(new OTS_JDBCDriver());
	}
	catch (Exception e)
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
	    
	    Connection conn = null;
	    Statement stmt = null;
	    
	    try
	    {
		System.out.println("\nCreating connection to database: "+url);

		conn = DriverManager.getConnection(url, user, password);
		stmt = conn.createStatement();

		try
		{
		    if (clean)
			stmt.executeUpdate("DROP TABLE test_table");
		    
		    if (clean || create)
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
	    catch (SQLException e)
	    {
		System.err.println(e);

		OTS.current().rollback();

		if (nested)
		    OTS.current().rollback();

		System.exit(0);
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
		conn = DriverManager.getConnection(url, user, password);
		stmt = conn.createStatement();
	    }

	    ResultSet res2 = null;
	    
	    try
	    {
		res2 = stmt.executeQuery("SELECT * FROM test_table");
	    }
	    catch (Exception e)
	    {
	    }

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
		exp.printStackTrace();
	    }
	    
	    System.err.println(ex);
	}

	OAInterface.shutdownOA();
	ORBInterface.shutdownORB();

	System.out.println("Test completed successfully.");
	System.exit(0);
    }
    
};
