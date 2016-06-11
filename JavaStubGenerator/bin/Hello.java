/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Hello.java,v 1.1 1997/07/31 09:13:03 nmcl Exp $
 */

import java.rmi.Remote;

import java.rmi.RemoteException;

public interface Hello extends Remote
{
    
public String sayHello () throws RemoteException;

public int foobar (long a) throws RemoteException;

};
