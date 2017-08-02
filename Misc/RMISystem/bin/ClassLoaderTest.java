/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ClassLoaderTest.java,v 1.2 1997/06/08 14:22:53 nmcl Exp $
 */

import RMISystem.net.NetworkClassLoader;
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

public class ClassLoaderTest
{

public static void main (String args[])
    {
	String hostName = "tarry-f.ncl.ac.uk";
	String inventoryName = "ArjunaInventory";
	String theClass = "DemoClass";
	NetworkClassLoader networkLoader = new NetworkClassLoader(hostName, inventoryName);

	try
	{
	    Class c = networkLoader.loadClass(theClass, true);

	    if (c != null)
	    {
		System.out.println("Class loaded.");

		try
		{
		    Object obj = c.newInstance();
		}
		catch (InstantiationException e)
		{
		    System.out.println(e);
		}
		catch (IllegalAccessException e)
		{
		    System.out.println(e);
		}
	    }
	    else
		System.out.println("Class did not load.");
	}
	catch (ClassNotFoundException e)
	{
	    System.out.println(e.toString());
	}
    }

}
