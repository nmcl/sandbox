/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: InventoryDriver.java,v 1.1 1997/06/01 14:56:22 nmcl Exp $
 */

import RMISystem.net.ArjunaInventory;
import java.rmi.Remote;
import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;

import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.rmi.AccessException;
import java.rmi.AlreadyBoundException;

public class InventoryDriver
{

public static void main (String args[])
    {
	String hostName = "tarry-f.ncl.ac.uk";
	String inventoryName = "ArjunaInventory";

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-name") == 0)
		inventoryName = args[i+1];
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: [-name <name>] [-help]");
		System.exit(0);
	    }
	}
	
	// Create and install a security manager
	
	System.setSecurityManager(new RMISecurityManager());

	try
	{
	    ArjunaInventory obj = new ArjunaInventory();
	    Naming.rebind("//"+hostName+"/"+inventoryName, obj);
	    System.out.println("Inventory: "+inventoryName+" bound in registry");
	}
	catch (Exception e)
	{
	    System.out.println("Inventory: "+inventoryName+" error:\n" + e.getMessage());
	    e.printStackTrace();
	}
    }
    
}
