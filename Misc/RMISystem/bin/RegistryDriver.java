/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RegistryDriver.java,v 1.2 1997/05/31 14:21:28 nmcl Exp $
 */

import RMISystem.rmi.ArjunaRegistry;
import RMISystem.rmi.ArjunaContextRegistry;
import java.rmi.Remote;
import java.rmi.registry.Registry;
import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;

import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.rmi.AccessException;
import java.rmi.AlreadyBoundException;

public class RegistryDriver extends UnicastRemoteObject implements Registry
{

public RegistryDriver () throws RemoteException
    {
    }
    
public Remote lookup (String name) throws RemoteException, NotBoundException, AccessException
    {
	return theRegistry.lookup(name);
    }

public void bind (String name, Remote obj) throws RemoteException, AlreadyBoundException, AccessException
    {
	theRegistry.bind(name, obj);
    }

public void unbind (String name) throws RemoteException, AccessException
    {
	theRegistry.unbind(name);
    }

public void rebind (String name, Remote obj) throws RemoteException, AccessException
    {
	theRegistry.rebind(name, obj);
    }

public String[] list () throws RemoteException, AccessException
    {
	return theRegistry.list();
    }
    
public static void main (String args[])
    {
	String hostName = "tarry-f.ncl.ac.uk";
	String registryName = "ArjunaRegistry";
	boolean useContext = false;

	for (int i = 0; i < args.length; i++)
	{
	    if (args[i].compareTo("-host") == 0)
		hostName = args[i+1];
	    if (args[i].compareTo("-name") == 0)
		registryName = args[i+1];
	    if (args[i].compareTo("-context") == 0)
		useContext = true;
	    if (args[i].compareTo("-help") == 0)
	    {
		System.out.println("Usage: [-host <host name>] [-name <registry>] [-context] [-help]");
		System.exit(0);
	    }
	}

	if (useContext)
	    theRegistry = new ArjunaContextRegistry();
	else
	    theRegistry = new ArjunaRegistry();
	
	// Create and install a security manager
	
	System.setSecurityManager(new RMISecurityManager());

	try
	{
	    RegistryDriver obj = new RegistryDriver();
	    Naming.rebind("//"+hostName+"/"+registryName, obj);
	    System.out.println("Registry: "+registryName+" bound in registry");
	}
	catch (Exception e)
	{
	    System.out.println("Registry: "+registryName+" error:\n" + e.getMessage());
	    e.printStackTrace();
	}
    }

private static ArjunaRegistry theRegistry = null;
    
}
