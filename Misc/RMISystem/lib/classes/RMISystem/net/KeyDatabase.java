/*
 * Copyright (C) 1997,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: KeyDatabase.java,v 1.2 1997/06/08 19:18:28 nmcl Exp $
 */

package RMISystem.net;

import java.rmi.Remote;

import java.util.NoSuchElementException;
import java.io.IOException;
import java.rmi.RemoteException;
import java.rmi.AccessException;

/**
 * Instances of this class hold keys which can be used to verify the identity
 * of another object. The key space is represented as a tuple:
 *
 * < object name, key >
 *
 */

public interface KeyDatabase extends Remote
{

    /**
     * Get the key for the specified object name.
     */
    
public String getKey (String objectName) throws RemoteException, AccessException, NoSuchElementException, IOException;

    /**
     * Validate the specified key.
     */

public boolean validateKey (String key) throws RemoteException, AccessException, IOException;

};
