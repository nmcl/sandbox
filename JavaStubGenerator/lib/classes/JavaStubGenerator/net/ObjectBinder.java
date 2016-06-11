/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjectBinder.java,v 1.1 1997/07/31 09:13:36 nmcl Exp $
 */

package JavaStubGenerator.net;

import JavaStubGenerator.stub.StubException;
import java.rmi.RemoteException;

public interface ObjectBinder
{

public Object bind (String objectName) throws RemoteException, StubException;

public void unbind (Object obj) throws RemoteException, StubException;

}
