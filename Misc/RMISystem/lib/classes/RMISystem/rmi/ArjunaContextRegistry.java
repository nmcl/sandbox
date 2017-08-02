/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ArjunaContextRegistry.java,v 1.2 1997/06/08 19:18:28 nmcl Exp $
 */

package RMISystem.rmi;

import java.rmi.Remote;
import java.rmi.*;

import java.io.IOException;
import java.lang.ClassNotFoundException;
import java.lang.InstantiationException;
import java.lang.IllegalAccessException;
import java.rmi.RemoteException;
import java.rmi.NotBoundException;
import java.rmi.AccessException;
import java.rmi.AlreadyBoundException;

/**
 * This implementation of the Registry loads all requested objects into
 * itself, rather than start a separate process for each, which the
 * ArjunaRegistry does by default. The code for objects can be searched
 * for in a provided search path, or assumed to be available in the
 * standard CLASSPATH.
 */

public class ArjunaContextRegistry extends ArjunaRegistry
{

    /**
     * Search for all classes via CLASSPATH.
     */
    
public ArjunaContextRegistry ()
    {
	super();
    }

    /**
     * Search for all classes via the provided search path.
     */
    
public ArjunaContextRegistry (String[] searchPath)
    {
	super(searchPath);
    }

    /**
     * Rather than exec new java runtimes we simply load the class
     * locally and pass back a reference to it within our current
     * context. As such, we override the ArjunaRegistry version
     * of create.
     */
    
protected Remote create (String name) throws AccessException
    {
	String javaExec = _nameService.getJavaCodeName(name);

	if (javaExec == null) // not present
	    return null;

	try
	{
	    System.out.println("ArjunaContextRegistry.create - searching for "+javaExec);
	    
	    Class c = Class.forName(javaExec);

	    /*
	     * Object *must* have a default constructor or
	     * this will fail!
	     */
	    
	    Object o = c.newInstance();

	    System.out.println("ArjunaContextRegistry.create - created "+javaExec);
	    
	    this.rebind(name, (Remote) o);

	    return (Remote) o;
	}
	catch (ClassNotFoundException e)
	{
	    throw(new AccessException(e.toString()));
	}
	catch (InstantiationException e)
	{
	    throw(new AccessException(e.toString()));
	}
	catch (IllegalAccessException e)
	{
	    throw(new AccessException(e.toString()));
	}
	catch (RemoteException e)
	{
	    // shouldn't happen!
	    
	    return null;
	}
    }
    
};
