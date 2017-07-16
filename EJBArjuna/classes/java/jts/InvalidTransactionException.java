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

public class InvalidTransactionException extends RemoteException
{
    
public InvalidTransactionException ()
    {
	super();
    }

public InvalidTransactionException (String msg)
    {
	super(msg);
    }

};
