/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RMIServerObjectBinder.java,v 1.1 1997/07/31 09:13:38 nmcl Exp $
 */

package JavaStubGenerator.rmi;

import JavaStubGenerator.net.ObjectBinder;
import java.rmi.*;

import JavaStubGenerator.stub.StubException;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.net.MalformedURLException;
import java.lang.ClassNotFoundException;
import java.lang.InstantiationException;
import java.lang.IllegalAccessException;
import java.io.IOException;

public class RMIServerObjectBinder implements ObjectBinder
{

    /*
     * This will eventually be done a la ObjectName, where we have the object
     * pick itself apart. For the moment, since all we need is the host and
     * object name, we can do that ourselves.
     */
    
public Object bind (String objectName) throws RemoteException, StubException
    {
	try
	{
	    Class objectClass = Class.forName(objectName);

	    /*
	     * At the moment all classes must have a default constructor.
	     * However, we can remove this restriction using JavaGandiva
	     * techniques.
	     */
	     
	    return objectClass.newInstance();
	}
	catch (ClassNotFoundException e1)
	{
	    throw new StubException(e1.toString());
	}
	catch (InstantiationException e2)
	{
	    throw new StubException(e2.toString());
	}
	catch (IllegalAccessException e3)
	{
	    throw new StubException(e3.toString());
	}
    }

public void unbind (Object obj) throws RemoteException, StubException
    {
	// nothing to do.
    }
 
}
