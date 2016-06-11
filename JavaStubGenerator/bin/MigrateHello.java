/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MigrateHello.java,v 1.1 1997/07/31 09:13:05 nmcl Exp $
 */

import JavaStubGenerator.migrate.Migrate;
import java.rmi.Remote;

import java.rmi.RemoteException;

public interface MigrateHello extends Migrate
{
    
public String sayHello () throws RemoteException;

public int foobar (long a) throws RemoteException;

};
