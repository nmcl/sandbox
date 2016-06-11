/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RMIClientObjectBinder.java,v 1.1 1997/07/31 09:13:37 nmcl Exp $
 */

package JavaStubGenerator.rmi;

import JavaStubGenerator.net.ObjectBinder;
import java.rmi.*;

import JavaStubGenerator.stub.StubException;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.net.MalformedURLException;
import java.io.IOException;

public class RMIClientObjectBinder implements ObjectBinder
{

    /*
     * This will eventually be done a la ObjectName, where we have the object
     * pick itself apart. For the moment, since all we need is the host and
     * object name, we can do that ourselves.
     */
    
public Object bind (String objectName) throws RemoteException, StubException
    {
	Object obj = null;
	String objName = null;
	String hostName = null;
	int index = objectName.indexOf(RMIClientObjectBinder.separator);

	if (index == -1)
	    throw new StubException("String "+objectName+" format illegal.");

	hostName = objectName.substring(0, index);
	objName = objectName.substring(index+1);
	
	try
	{
	    obj = Naming.lookup("//"+hostName+"/"+objName);
	}
	catch (NotBoundException e1)
	{
	    throw new StubException(e1.toString());
	}
	catch (MalformedURLException e2)
	{
	    throw new StubException(e2.toString());
	}

	return obj;
    }

public void unbind (Object obj) throws RemoteException, StubException
    {
	// nothing to do.
    }

private static final char separator = ':';
 
}
