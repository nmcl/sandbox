/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArjunaRegistry.java,v 1.4 1997/06/08 19:18:28 nmcl Exp $
 */

package RMISystem.rmi;

import java.lang.Runtime;
import java.lang.Process;
import java.rmi.Remote;
import java.rmi.registry.Registry;
import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;

import java.io.IOException;
import java.lang.InterruptedException;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.rmi.AccessException;
import java.rmi.AlreadyBoundException;

/**
 * This implementation of the Registry enables objects to be registered
 * with a local naming service. Whenever a request for an object is
 * received and a server for that object does not already exist, this
 * Registry will determine where the server code is an start it up. The
 * server is reference counted, and will only be terminated when all clients
 * have indicated that they are finished with it. (Failures of clients are
 * treated as implicit dereferences.)
 *
 * The mapping between object name and Java executable flexible. The
 * executable can be located via the default CLASSPATH, or can be
 * searched for via a search path.
 */

public class ArjunaRegistry implements Registry
{

    /**
     * Create a registry which searches for executables only in
     * the CLASSPATH.
     */
    
public ArjunaRegistry ()
    {
	_searchPath = null;  // assume all objects will be accessible via CLASSPATH
	_list = new RegistryList();
	_nameService = new NameMapping();
    }

    /**
     * Create a registry which searches for executables in the
     * provided search path. It is assumed that all references
     * found within the executable can be resolved via the
     * CLASSPATH.
     */
    
public ArjunaRegistry (String[] searchPath)
    {
	_searchPath = searchPath;
	_list = new RegistryList();
	_nameService = new NameMapping();
    }

    /**
     * Return a reference to the specified object. If the server within which the object
     * resides is not currently executing, then it will be found and started.
     */
    
public Remote lookup (String name) throws RemoteException, NotBoundException, AccessException
    {
	System.out.println("lookup ( "+name+" )");

	Remote ptr = _list.find(name);

	if (ptr == null)
	{
	    /*
	     * Not present, so create and add to list.
	     */

	    ptr = create(name);

	    if (ptr == null)
		throw(new NotBoundException("Could not find/create object."));
	}
	
	return ptr;
    }

    /**
     * Bind the object to the specified name.
     */
    
public void bind (String name, Remote obj) throws RemoteException, AlreadyBoundException, AccessException
    {
	System.out.println("bind ( "+name+", obj )");

	this.rebind(name, obj);
    }

    /**
     * Unbind an object from the specified name.
     */
    
public void unbind (String name) throws RemoteException, AccessException
    {
	System.out.println("unbind ( "+name+" )");

	_list.unref(name);
    }

    /**
     * Discard any previous association between object and name in favour
     * of the one provided.
     */
    
public void rebind (String name, Remote obj) throws RemoteException, AccessException
    {
	System.out.println("rebind ( "+name+", obj )");

	_list.replace(name, obj);
    }

    /**
     * List all of the entries in the registry. Note: this will only be
     * those servers/objects which have been started, and not the objects
     * registered in the (external) naming service.
     */
    
public String[] list () throws RemoteException, AccessException
    {
	System.out.println("list ()");

	return _list.list();
    }

    /**
     * Rather than simply allow any string to be exec-ed (!) we search
     * through our name mapping database. If the object name is present
     * then we start up the corresponding java executable, otherwise we
     * do nothing.
     */
    
protected Remote create (String name) throws AccessException
    {
	String javaExec = _nameService.getJavaCodeName(name);

	if (javaExec == null) // not present
	    return null;
	
	try
	{
	    System.out.println("ArjunaRegistry.create - execing: java "+javaExec);
	    
	    Process proc = Runtime.getRuntime().exec("java "+javaExec);

	    if (proc == null)
		throw(new AccessException("ArjunaRegistry.create - exec of: java "+javaExec+" failed"));

	    int retry = ArjunaRegistry.BIND_RETRY;
	    Remote theObject = null;

	    do
	    {
		try
		{
		    Thread.sleep(ArjunaRegistry.BIND_TIMEOUT);
		}
		catch (InterruptedException e)
		{
		}

		System.out.println("ArjunaRegistry.create - searching again.");
		
		theObject = _list.find(name);

		if (theObject != null)
		    return theObject;
		
	    } while (--retry > 0);

	    return null;
	}
	catch (IOException e)
	{
	    throw(new AccessException(e.toString()));
	}
    }

protected RegistryList _list;
protected String[] _searchPath;
protected NameMapping _nameService;

private static final int BIND_RETRY = 10;
private static final int BIND_TIMEOUT = 10000; // milliseconds
    
};
