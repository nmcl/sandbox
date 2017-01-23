/*
 * Copyright (C) 1999,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BasicThreadFilter.java,v 1.1 1999/09/10 13:32:49 nmcl Exp $
 */

package com.arjuna.OrbCommon.Filters.orbix;

import org.omg.CORBA.ORB;
import org.omg.CORBA.Request;
import IE.Iona.OrbixWeb._CORBA ;
import IE.Iona.OrbixWeb._OrbixWeb ;
import IE.Iona.OrbixWeb.Features.ThreadFilter ;

import org.omg.CORBA.SystemException;

public class BasicThreadFilter extends ThreadFilter
{

public BasicThreadFilter ()
    {
	super() ;
    }
  
public boolean inRequestPreMarshal (Request req)
    {
	ThreadWorker t = new ThreadWorker(req);

	t.start();
	t = null;

        return false ;
    }

};

class ThreadWorker extends Thread
{

public ThreadWorker (Request r)
    {
	_request = r;
    }

public void run ()
    {
	_CORBA.Orbix.continueThreadDispatch(_request);
    }

private Request _request;
    
};
