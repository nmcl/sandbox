/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArjunaInventory.java,v 1.3 1997/06/08 19:18:28 nmcl Exp $
 */

package RMISystem.net;

import java.util.Hashtable;
import java.rmi.Remote;
import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;
import java.io.FileInputStream;
import java.io.File;

import java.io.IOException;
import java.io.FileNotFoundException;
import java.lang.ClassNotFoundException;
import java.lang.InstantiationException;
import java.lang.IllegalAccessException;
import java.lang.SecurityException;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.rmi.AccessException;
import java.net.MalformedURLException;

/**
 * An implementation of the NetworkInventory interface.
 */
 
public class ArjunaInventory extends UnicastRemoteObject implements NetworkInventory
{

    /**
     * Create with a minimum search path of /tmp only.
     */
    
public ArjunaInventory () throws RemoteException
    {
	_cache = new Hashtable();
	_searchPath = new String[1];
	_searchPath[0] = "/tmp";
    }

    /**
     * Create with the specified search path. Note: no attempt is made to
     * ensure this is valid.
     */
    
public ArjunaInventory (String[] path) throws RemoteException
    {
	_cache = new Hashtable();
	_searchPath = path;
    }

    /**
     * Return the byte array representing the specified class, or raise an appropriate
     * exception if it is not present.
     * All class bytes are cached, and all subsequent requests for the same class will
     * be fielded from the (volatile) cache.
     */
    
public byte[] loadClass (String className) throws RemoteException, AccessException, ClassNotFoundException, SecurityException
    {
	if (className == null)
	    return null;

	ClassStore c = (ClassStore) _cache.get(className);

	if (c == null)
	{
	    System.out.println("ArjunaInventory.loadClass - searching for "+className);

	    FileInputStream fstream = findClass(className+".class");
	    
	    if (fstream != null)
	    {
		try
		{
		    if (fstream.available() == 0)
			throw(new ClassNotFoundException(className));

		    byte[] b = new byte[fstream.available()];

		    fstream.read(b);
		    c = new ClassStore(b);
		
		    _cache.put(className, c);
		    System.out.println("ArjunaInventory.loadClass - found "+className);
		}
		catch (IOException e)
		{
		    throw(new ClassNotFoundException(className));
		}
	    }
	    else
		System.out.println("ArjunaInventory.loadClass - did not find "+className);		
	}
	else
	    System.out.println("ArjunaInventory.loadClass - class "+className+" in cache.");

	if (c == null)
	    return null;
	else
	    return c.classBytes();
    }

    /**
     * Instruct this inventory instance to fetch the specified class from the
     * specified inventory. Note: currently no attempt is made to ensure that these
     * two inventories are different instances.
     */
    
public void retrieveClass (String className, String hostName, String inventoryName) throws RemoteException, AccessException, ClassNotFoundException, SecurityException
    {
	if (className == null)
	    throw(new ClassNotFoundException());

	ClassStore c = (ClassStore) _cache.get(className);

	if (c != null)
	{
	    System.out.println("ArjunaInventory.retrieveClass - found "+className+" in cache.");
	    return;
	}
	
	NetworkInventory inventory = null;
	
	try
	{
	    inventory = (NetworkInventory) Naming.lookup("//"+hostName+"/"+inventoryName);
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

	if (inventory == null)
	    throw(new ClassNotFoundException(className));
	
	byte[] b = inventory.loadClass(className);

	if (b != null)
	{
	    c = new ClassStore(b);
	    _cache.put(className, c);
	    
	    System.out.println("ArjunaInventory.retrieveClass - found "+className);
	}
	else
	    throw(new ClassNotFoundException(className));
    }
    
    /*
     * Search through the search path for the class and return a stream to
     * allow it to be loaded.
     */
    
private FileInputStream findClass (String className) throws ClassNotFoundException
    {
	File f = null;

	/*
	 * Look in cwd first.
	 */

	f = new File(className);

	if (!f.exists())
	{
	    if (_searchPath == null)
		throw(new ClassNotFoundException());
	    else
	    {
		boolean found = false;
	    
		for (int i = 0; (i < _searchPath.length) && (!found); i++)
		{
		    f = new File(_searchPath[i]+File.separator+className);

		    if (f.exists())
			found = true;
		}

		if (!found)
		    throw(new ClassNotFoundException(className));
	    }
	}

	/*
	 * Found it!
	 */

	try
	{
	    return new FileInputStream(f);
	}
	catch (FileNotFoundException e)
	{
	    throw(new ClassNotFoundException(className));
	}
    }
    
private Hashtable _cache;
private String[] _searchPath;
    
};
