/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NetworkClassLoader.java,v 1.3 1997/06/08 19:18:28 nmcl Exp $
 */

package RMISystem.net;

import java.rmi.*;
import java.util.Hashtable;
import java.lang.ClassLoader;

import java.lang.SecurityException;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.net.MalformedURLException;

/**
 * A network class loader. Instances of this class can be used to load classes
 * within another address space. This implementation make use of the NetworkInventory
 * class to perform its work.
 */

public class NetworkClassLoader extends ClassLoader
{

    /**
     * Create a class loader which obtains its classes from the
     * specified inventory on the specified (remote) machine.
     */
    
public NetworkClassLoader (String hostName, String inventoryName)
    {
	_cache = new Hashtable();
	_hostName = hostName;
	_inventoryName = inventoryName;

	_networkInventory = null;
	
	try
	{
	    _networkInventory = (NetworkInventory) Naming.lookup("//"+_hostName+"/"+_inventoryName);
	}
	catch (NotBoundException e)
	{
	}
	catch (RemoteException e)
	{
	}
	catch (MalformedURLException e)
	{
	}
    }

    /**
     * Load the specified class. If the class is a sysem class, or one which
     * can be resolved locally, then no network communication is performed.
     * If the resolve parameter is true then resolve all
     * references to other classes found within this new class definition.
     * Once obtained, a class definition is retained within a volatile cache and
     * subsequent requests are fielded by this.
     */
    
public synchronized Class loadClass (String className, boolean resolve) throws ClassNotFoundException
    {
	if (className == null)
	    throw(new ClassNotFoundException("Invalid class name: "+className));

	Class c = (Class) _cache.get(className);
	
	if (c == null)
	{
	    /*
	     * Save time - is it a system class?
	     */
	    
	    try
	    {
		c = findSystemClass(className);

		if (c != null)
		    return c;
	    }
	    catch (ClassNotFoundException e)
	    {
	    }
	    
	    if (_networkInventory == null)
		throw(new ClassNotFoundException("NetworkInventory "+_inventoryName+" on "+_hostName+" not set."));
	    
	    byte[] b = loadNetworkClass(className);

	    if (b != null)
	    {
		c = defineClass(className, b, 0, b.length);
		_cache.put(className, c);

		if (resolve)
		    resolveClass(c);
	    }
	}

	if (c == null)
	    throw(new ClassNotFoundException(className));
	else
	    return c;
    }

private byte[] loadNetworkClass (String className) throws ClassNotFoundException
    {
	if (_networkInventory == null)
	    return null;

	try
	{
	    return _networkInventory.loadClass(className);
	}
	catch (AccessException e)
	{
	}
	catch (RemoteException e)
	{
	}
	catch (ClassNotFoundException e)
	{
	}

	throw(new ClassNotFoundException(className));
    }
    
private Hashtable _cache;
private String _hostName;
private String _inventoryName;
private NetworkInventory _networkInventory;

};
