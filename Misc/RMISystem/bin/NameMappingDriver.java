/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NameMappingDriver.java,v 1.3 1997/06/08 14:22:53 nmcl Exp $
 */

import RMISystem.rmi.NameMapping;
import java.io.*;

import java.lang.NumberFormatException;

public class NameMappingDriver
{
    
public static void main (String args[])
    {
	String mapFile = NameMapping.defaultMap;
	
	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-file") == 0)
		mapFile = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: [-file <map file>] [-help]");
		System.exit(0);
	    }
	}

	_theMap = null;
	
	createMap(mapFile);
    }

private static void createMap (String mapFile)
    {
	boolean finished = false;
	
	_theMap = new NameMapping(mapFile);

	while (!finished)
	{
	    System.out.println();	    
	    System.out.println("Options:");
	    System.out.println("1) Add entry.");
	    System.out.println("2) Remove entry.");
	    System.out.println("3) List entries.");
	    System.out.println("4) Clear entries.");	    
	    System.out.println("5) Save mapping.");
	    System.out.println("6) Quit.");
	    System.out.println();
	    
	    int option = 0;
	    
	    try
	    {
		String s = _data.readLine();
		
		option = (int) Long.parseLong(s);
	    }
	    catch (IOException e)
	    {
		option = 6;
	    }
	    catch (NumberFormatException e)
	    {
		System.err.println("Not a number.");
	    }

	    switch (option)
	    {
	    case 1:
		addEntry();
		break;
	    case 2:
		removeEntry();
		break;
	    case 3:
		listEntries();
		break;
	    case 4:
		_theMap.clearMapping();
		break;
	    case 5:
		_theMap.saveMapping();
		break;
	    case 6:
		finished = true;
		break;
	    default:
		System.out.println("Unknown option: "+option);
		break;
	    }
	}
    }

private static void addEntry ()
    {
	System.out.println();
	
	try
	{
	    System.out.println("ObjectName: ");
	    String objName = _data.readLine();

	    System.out.println("\nJavaExec: ");
	    String javaExec = _data.readLine();

	    if (_theMap.setJavaCodeName(objName, javaExec))
		System.out.println("\nMapping added.");
	    else
		System.out.println("\nMapping could not be added.");
	}
	catch (IOException e)
	{
	    System.err.println(e);
	}
    }

private static void removeEntry ()
    {
	System.out.println();
	
	try
	{
	    System.out.println("ObjectName: ");
	    String objName = _data.readLine();

	    if (_theMap.removeJavaCodeName(objName))
		System.out.println("\nMapping removed.");
	    else
		System.out.println("\nMapping could not be removed.");
	}
	catch (IOException e)
	{
	    System.err.println(e);
	}
    }

private static void listEntries ()
    {
	System.out.println();
	
	_theMap.list(System.out);
    }
    
private static NameMapping _theMap = null;
private static BufferedReader _data = new BufferedReader(new InputStreamReader(System.in));
    
}