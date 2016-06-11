/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MigrateObjectBinder.java,v 1.1 1997/07/31 09:13:34 nmcl Exp $
 */

package JavaStubGenerator.migrate;

import JavaStubGenerator.rmi.RMIClientObjectBinder;
import JavaStubGenerator.net.ObjectBinder;
import java.rmi.*;

import JavaStubGenerator.stub.StubException;
import java.rmi.NotBoundException;
import java.rmi.RemoteException;
import java.net.MalformedURLException;
import java.io.IOException;

public class MigrateObjectBinder implements ObjectBinder
{

public Object bind (String objectName) throws RemoteException, StubException
    {
	return null;
    }

public void unbind (Object obj) throws RemoteException, StubException
    {
    }
 
}
