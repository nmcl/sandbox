/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArjunaKeyDatabase.java,v 1.2 1997/06/08 19:18:28 nmcl Exp $
 */

package RMISystem.net;

import java.util.Hashtable;
import java.io.*;
import java.rmi.Remote;
import java.rmi.*;
import java.rmi.server.UnicastRemoteObject;
import java.util.Enumeration;

import java.io.IOException;
import java.io.FileNotFoundException;
import java.util.NoSuchElementException;
import java.lang.InstantiationException;
import java.lang.ClassNotFoundException;
import java.lang.IllegalAccessException;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.rmi.AccessException;
import java.net.MalformedURLException;

/**
 * A specific implementation of the KeyDatabase.
 * Some of the public methods are only available to users on the same
 * machine as the object.
 */

public class ArjunaKeyDatabase extends UnicastRemoteObject implements KeyDatabase
{

    /**
     * Create the database with the default name "/tmp/defaultDBName".
     */
    
public ArjunaKeyDatabase () throws RemoteException
    {
	_dbName = "/tmp/defaultDBName";
	_keyStore = null;
    }

    /**
     * Create the database with the specified name.
     */
    
public ArjunaKeyDatabase (String dbName) throws RemoteException
    {
	_dbName = dbName;
	_keyStore = null;
    }

    /**
     * Given an objectname, return the current key.
     */
    
public synchronized String getKey (String objectName) throws RemoteException, AccessException, NoSuchElementException, IOException
    {
	if (!loadDB())
	    throw(new IOException("Error loading keys."));

	if (_keyStore.isEmpty())
	    throw(new NoSuchElementException("Key database is empty."));

	String s = (String) _keyStore.get(objectName);

	if (s == null)
	    throw(new NoSuchElementException(objectName));
	else
	    return s;
    }

    /**
     * Is the key valid, i.e., do we know about it?
     */
    
public synchronized boolean validateKey (String keyToValidate) throws RemoteException, AccessException, IOException
    {
	if (!loadDB())
	    throw(new IOException("Error loading keys."));

	if (_keyStore.isEmpty())
	    throw(new NoSuchElementException("Key database is empty."));

	Enumeration elements = _keyStore.keys();

	while (elements.hasMoreElements())
	{
	    String objectname = (String) elements.nextElement();
	    String key = (String) _keyStore.get(objectname);
	    
	    if (key.compareTo(keyToValidate) == 0)
		return true;
	}

	/*
	 * Not found, so key must be invalid.
	 */
	
	return false;
    }
    
    /*
     * Methods not available via the network.
     */

    /**
     * Zero the database.
     */
    
public synchronized void removeAllEntries () throws IOException
    {
	_keyStore = null;

	if (!saveDB())
	    throw(new IOException("Error saving null key store."));
    }

    /**
     * Setup an association between the object name and the key.
     */
    
public synchronized void setKey (String objectName, String key) throws IOException
    {
	if (!loadDB())
	    throw(new IOException("Error loading keys."));

	_keyStore.put(objectName, key);
	
	if (!saveDB())
	    throw(new IOException("Error saving keys."));
    }

    /**
     * Remove an association given the object name.
     */
    
public synchronized void removeObjectName (String objectName) throws IOException
    {
	if (!loadDB())
	    throw(new IOException("Error loading keys."));

	_keyStore.remove(objectName);
	
	if (!saveDB())
	    throw(new IOException("Error saving keys."));
    }

    /**
     * Remove an association given the key.
     */
    
public synchronized void removeKey (String keyToRemove) throws IOException
    {
	if (!loadDB())
	    throw(new IOException("Error loading keys."));

	Enumeration elements = _keyStore.keys();

	while (elements.hasMoreElements())
	{
	    String objectname = (String) elements.nextElement();
	    String key = (String) _keyStore.get(objectname);
	    
	    if (key.compareTo(keyToRemove) == 0)
	    {
		_keyStore.remove(objectname);

		if (!saveDB())
		    throw(new IOException("Error saving keys."));

		return;
	    }
	}
    }

    /**
     * List all of the entries within the database.
     */
    
public synchronized void list (PrintStream strm)
    {
	strm.print("Key database: ");
	
	if (_keyStore.isEmpty())
	    strm.println("EMPTY");
	else
	{
	    strm.println();

	    Enumeration elements = _keyStore.keys();

	    while (elements.hasMoreElements())
	    {
		String objectname = (String) elements.nextElement();
		String key = (String) _keyStore.get(objectname);

		strm.println("< "+objectname+", "+key+" >");
	    }
	}
    }
    
private boolean loadDB ()
    {
	_keyStore = new Hashtable();
	
	File f = new File(_dbName);
	boolean result = false;

	if (f.exists())
	{
	    try
	    {
		FileInputStream i = new FileInputStream(_dbName);
		ObjectInput o = new ObjectInputStream(i);
		int number = o.read();

		if (number > 0)
		{
		    for (int j = 0; j < number; j++)
		    {
			String objectname = (String) o.readObject();
			String key = (String) o.readObject();

			_keyStore.put(objectname, key);
		    }
		}

		result = true;
	    }
	    catch (FileNotFoundException e)
	    {
	    }
	    catch (IOException e)
	    {
	    }
	    catch (ClassNotFoundException e)
	    {
	    }
	}
	else
	    result = true;

	return result;
    }

private boolean saveDB ()
    {
	boolean result = false;

	try
	{
	    FileOutputStream f = new FileOutputStream(_dbName);
	    ObjectOutput s = new ObjectOutputStream(f);	
	    int number = _keyStore.size();
	    Enumeration elements = _keyStore.keys();
	
	    s.write(number);

	    while (elements.hasMoreElements())
	    {
		String objectname = (String) elements.nextElement();
		String key = (String) _keyStore.get(objectname);

		s.writeObject(objectname);
		s.writeObject(key);
	    }

	    result = true;
	}
	catch (IOException e)
	{
	}

	return result;
    }
    
private String _dbName;
private Hashtable _keyStore;
    
};
