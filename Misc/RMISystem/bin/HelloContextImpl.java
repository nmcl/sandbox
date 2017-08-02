/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HelloContextImpl.java,v 1.1 1997/05/31 14:21:28 nmcl Exp $
 */

import java.rmi.*;
import java.rmi.registry.Registry;
import java.rmi.server.UnicastRemoteObject;

import java.rmi.RemoteException;

public class HelloContextImpl extends UnicastRemoteObject implements Hello
{

public HelloContextImpl () throws RemoteException
    {
	super();
	name = null;
    }
    
public HelloContextImpl (String s) throws RemoteException
    {
	super();
	name = s;
    }

public String sayHello () throws RemoteException
    {
	return "Hello World!";
    }

private String name;

}
