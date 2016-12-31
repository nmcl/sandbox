/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: JDBCTest.java,v 1.4.4.1 2000/05/04 15:33:13 nmcl Exp $
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
	String url = "jdbc:arjuna_jdbc1:mysql://glororan:3306/mysql";
	String user = "test";
	String password = "test";
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-mysql") == 0)
	    {
		dbType = MYSQL;
		url = "jdbc:arjuna_jdbc1:mysql://glororan:3306/mysql";
	    }
	    if (args[i].compareTo("-cloudscape") == 0)
	    {
		dbType = CLOUDSCAPE;
		url = "jdbc:arjuna_jdbc1:cloudscape:mysql;create=true";
	    }
	    if (args[i].compareTo("-sequelink") == 0)
	    {
		dbType = SEQUELINK;
		url = "jdbc:arjuna_jdbc1:sequelink://reshend:20000";
		user = "tester";
		password = "tester";
	    }
	    if (args[i].compareTo("-oracle") == 0)
	    {
		dbType = ORACLE;
		url = "jdbc:arjuna_jdbc1:oracle:thin:@reshend.ncl.ac.uk:1521:JDBCTest";
		user = "tester";
		password = "tester";
	    }
	    if (args[i].compareTo("-clean") == 0)
		clean = true;
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
		System.out.println("Usage: JDBCTest [-commit] [-nested] [-reuseconn] [-clean] [-create] [-mysql] [-cloudscape] [-sequelink] [-oracle]");
		System.exit(0);
	    }
	}
	
	ORBInterface.initORB(args, null);
	ORBInterface.initBOA();
	
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
    }
    
};
