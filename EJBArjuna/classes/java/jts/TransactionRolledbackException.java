/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

package java.jts;

import java.rmi.RemoteException;

public class TransactionRolledbackException extends RemoteException
{
    
public TransactionRolledbackException ()
    {
	super();
    }

public TransactionRolledbackException (String msg)
    {
	super(msg);
    }

};
