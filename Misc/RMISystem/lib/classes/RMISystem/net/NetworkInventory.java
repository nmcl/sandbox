/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NetworkInventory.java,v 1.3 1997/06/08 19:18:28 nmcl Exp $
 */

package RMISystem.net;

import java.rmi.Remote;

import java.lang.SecurityException;
import java.lang.ClassNotFoundException;
import java.rmi.RemoteException;
import java.rmi.AccessException;

/**
 * Instances of this class represent repositories for classes. An inventory can load a
 * class definition (byte array) from the local disk, for example. In addition, inventories
 * can co-operate to supply class definitions to each other across the network.
 */

public interface NetworkInventory extends Remote
{

    /**
     * Return the specified class definition. How this is performed is
     * implementation specific.
     */
    
public byte[] loadClass (String className) throws RemoteException, AccessException, ClassNotFoundException, SecurityException;

    /**
     * Retrieve the specified class definition from the inventory residing on the
     * specified host.
     */

public void retrieveClass (String className, String hostName, String inventoryName) throws RemoteException, AccessException, ClassNotFoundException, SecurityException;

};
