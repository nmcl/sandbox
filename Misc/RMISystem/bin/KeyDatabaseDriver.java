/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: KeyDatabaseDriver.java,v 1.1 1997/06/08 14:22:53 nmcl Exp $
 */

import RMISystem.net.*;
import java.io.*;
import java.rmi.*;

import java.rmi.RemoteException;
import java.rmi.UnknownHostException;
import java.rmi.NotBoundException;
import java.rmi.AccessException;
import java.net.MalformedURLException;
import java.io.IOException;

public class KeyDatabaseDriver
{
    
public static void main (String args[])
    {
	String hostName = null;
	String objName = null;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-host") == 0)
		hostName = args[i+1];
	    if (args[i].compareTo("-obj") == 0)
		objName = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: [-host <host name>] [-obj <object name>] [-help]");
		System.exit(0);
	    }
	}

	if (hostName == null)
	    localDatabase();
	else
	{
	    if (objName != null)
		remoteDatabase(hostName, objName);
	    else
		System.err.println("Error - no object name specified.");
	}
    }

private static void localDatabase ()
    {
	boolean finished = false;
	ArjunaKeyDatabase db = null;

	try
	{
	    db = new ArjunaKeyDatabase();
	}
	catch (RemoteException e)
	{
	    System.err.println(e);
	    System.exit(1);
	}
	
	while (!finished)
	{
	    System.out.println();	    
	    System.out.println("Options:");
	    System.out.println("1) Add entry.");
	    System.out.println("2) Remove entry.");
	    System.out.println("3) List entries.");
	    System.out.println("4) Clear entries.");	    
	    System.out.println("5) Quit.");
	    System.out.println();
	    
	    int option = 0;
	    
	    try
	    {
		String s = _data.readLine();
		
		option = (int) Long.parseLong(s);
	    }
	    catch (IOException e)
	    {
		option = 5;
	    }
	    catch (NumberFormatException e)
	    {
		System.err.println("Not a number.");
	    }

	    switch (option)
	    {
	    case 1:
		addEntry(db);
		break;
	    case 2:
		removeEntry(db);
		break;
	    case 3:
		listEntries(db);
		break;
	    case 4:
		try
		{
		    db.removeAllEntries();
		}
		catch (IOException e)
		{
		    System.err.println(e);
		}
		
		break;
	    case 5:
		finished = true;
		break;
	    default:
		System.out.println("Unknown option: "+option);
		break;
	    }
	}
    }

private static void addEntry (ArjunaKeyDatabase db)
    {
	System.out.println();
	
	try
	{
	    System.out.println("ObjectName: ");
	    String objName = _data.readLine();

	    System.out.println("\nKey: ");
	    String key = _data.readLine();

	    db.setKey(objName, key);

	    System.out.println("\nMapping added.");
	}
	catch (IOException e)
	{
	    System.err.println(e);
	}
    }

private static void removeEntry (ArjunaKeyDatabase db)
    {
	System.out.println();
	
	try
	{
	    System.out.println("ObjectName: ");
	    String objName = _data.readLine();

	    db.removeObjectName(objName);

	    System.out.println("\nMapping removed.");
	}
	catch (IOException e)
	{
	    System.err.println(e);
	}
    }

private static void listEntries (ArjunaKeyDatabase db)
    {
	System.out.println();
	db.list(System.out);
    }

private static void remoteDatabase (String hostName, String objectName)
    {
	try
	{
	    KeyDatabase obj = (KeyDatabase) Naming.lookup("//" + hostName + "/ArjunaKeyDatabase");
	    String key = obj.getKey(objectName);

	    System.out.println("< "+objectName+", "+key+" >");
	}
	catch (Exception e)
	{
	    System.err.println(e);
	}
    }
    
private static BufferedReader _data = new BufferedReader(new InputStreamReader(System.in));
    
}
