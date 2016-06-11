/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Migrate.java,v 1.1 1997/07/31 09:13:33 nmcl Exp $
 */

package JavaStubGenerator.migrate;

import java.io.*;

import JavaStubGenerator.stub.StubException;
import java.rmi.RemoteException;
import java.io.IOException;

public interface Migrate
{

public boolean migrateLocal () throws StubException;
public boolean beginMigrate () throws RemoteException, StubException;
public boolean endMigrate (String newAddress) throws RemoteException, StubException;

}
