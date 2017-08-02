/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HelloImpl.java,v 1.1 1997/05/30 18:26:43 nmcl Exp $
 */

import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;

import java.rmi.RemoteException;

public class HelloImpl extends UnicastRemoteObject implements Hello
{
    
public HelloImpl (String s) throws RemoteException
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
	    HelloImpl obj = new HelloImpl("HelloServer");
	    Naming.rebind("//tarry-f.ncl.ac.uk/HelloServer", obj);
	    System.out.println("HelloServer bound in registry");
	}
	catch (Exception e)
	{
	    System.out.println("HelloImpl err: " + e.getMessage());
	    e.printStackTrace();
	}
    }

private String name;

}
