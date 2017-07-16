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

public class HeuristicMixedException extends RemoteException
{
    
public HeuristicMixedException ()
    {
	super();
    }

public HeuristicMixedException (String msg)
    {
	super(msg);
    }

};
