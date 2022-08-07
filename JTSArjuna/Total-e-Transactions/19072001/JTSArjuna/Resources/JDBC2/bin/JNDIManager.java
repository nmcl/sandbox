/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 * 
 * $Id: JNDIManager.java,v 1.1.2.1.14.1 2000/12/14 15:11:14 nmcl Exp $
 */

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.JDBC2.*;
import java.io.*;
import java.util.*;
import java.sql.*;
import javax.sql.*;
import javax.transaction.xa.*;
import javax.naming.*;

public class JNDIManager
{

public static void main (String[] args)
    {
	String url = "jdbc:arjuna:oracle:thin:@reshend.ncl.ac.uk:1521:JDBCTest";
	String dynamicClass = "com.arjuna.JDBC2.drivers.oracle_8_1_6";
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].equals("-help"))
	    {
		System.out.println("Usage: JNDIManager [-url <url>] [-dynamic <class>] [-help]");
		System.exit(0);
	    }
	    if (args[i].equals("-url"))
		url = args[i+1];
	    if (args[i].equals("-dynamic"))
		dynamicClass = args[i+1];
	}

	try
	{
	    Class c = Class.forName(dynamicClass);
	    ArjunaJDBC2DynamicClass dc = (ArjunaJDBC2DynamicClass) c.newInstance();
	    XADataSource ds = (XADataSource) dc.getDataSource(url);

	    Hashtable env = new Hashtable();
	    String initialCtx = PropertyManager.getProperty("Context.INITIAL_CONTEXT_FACTORY");
	    
	    env.put(Context.INITIAL_CONTEXT_FACTORY, initialCtx);

	    InitialContext ctx = new InitialContext(env);
	    
	    ctx.bind("/tmp/foo", ds);

	    System.out.println("Ready");

	    for (;;);
	}
	catch (Exception e)
	{
	    System.err.println(e);
	}
    }

};
