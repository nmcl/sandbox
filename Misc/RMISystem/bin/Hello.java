/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Hello.java,v 1.1 1997/05/30 18:26:42 nmcl Exp $
 */

import java.rmi.Remote;

import java.rmi.RemoteException;

public interface Hello extends Remote
{
    String sayHello() throws RemoteException;
};
