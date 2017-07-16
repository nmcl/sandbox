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

public class HeuristicRollbackException extends RemoteException
{
    
public HeuristicRollbackException ()
    {
	super();
    }

public HeuristicRollbackException (String msg)
    {
	super(msg);
    }

};
