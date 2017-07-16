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

public class HeuristicCommitException extends RemoteException
{
    
public HeuristicCommitException ()
    {
	super();
    }

public HeuristicCommitException (String msg)
    {
	super(msg);
    }

};
