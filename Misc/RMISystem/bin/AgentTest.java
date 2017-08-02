/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AgentTest.java,v 1.2 1997/06/08 14:22:52 nmcl Exp $
 */

import RMISystem.net.NetworkInventory;
import java.rmi.Remote;
import java.rmi.registry.Registry;
import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;

import java.lang.ClassNotFoundException;
import java.lang.InstantiationException;
import java.lang.IllegalAccessException;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.rmi.AccessException;
import java.rmi.AlreadyBoundException;

public class AgentTest
{

public static void main (String args[])
    {
	String hostName = "tarry-f.ncl.ac.uk";
	String localInventoryName = "ArjunaInventory";
	String remoteInventoryName = "ArjunaInventory2";
	String className = "DemoClass";
	NetworkInventory inventory = null;

	try
	{
	    inventory = (NetworkInventory) Naming.lookup("//"+hostName+"/"+localInventoryName);	    
	}
	catch (Exception e)
	{
	    System.out.println(e.toString());
	    System.exit(1);
	}

	try
	{
	    inventory.retrieveClass(className, hostName, remoteInventoryName);
	    System.out.println("Class retrieved.");
	}
	catch (Exception e)
	{
	    System.out.println(e.toString());
	}
    }

}
