/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HelloRegistryImpl.java,v 1.2 1997/05/31 13:32:00 nmcl Exp $
 */

import java.rmi.*;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

import java.rmi.RemoteException;

public class HelloRegistryImpl extends UnicastRemoteObject implements Hello
{
    
public HelloRegistryImpl (String s) throws RemoteException
    {
	super();
	name = s;
    }

public String sayHello () throws RemoteException
    {
	return  "Hello World!";
    }

public static void main (String args[])
    {
	// Create and install a security manager
	
	System.setSecurityManager(new RMISecurityManager());

	try
	{
	    HelloRegistryImpl obj = new HelloRegistryImpl("HelloRegistryServer");
	    Registry reg = (Registry) Naming.lookup("//tarry-f.ncl.ac.uk/ArjunaRegistry");
	    reg.rebind("HelloRegistryServer", obj);
	    System.out.println("HelloRegistryServer bound in registry");
	}
	catch (Exception e)
	{
	    System.out.println("HelloRegistryImpl err: " + e.getMessage());
	    e.printStackTrace();
	}
    }

private String name;

}
