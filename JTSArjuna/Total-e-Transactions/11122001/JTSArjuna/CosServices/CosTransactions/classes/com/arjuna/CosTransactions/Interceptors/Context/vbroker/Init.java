/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: Init.java,v 1.1 2000/02/25 14:06:02 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interceptors.Context.vbroker;

import java.util.*;
import com.inprise.vbroker.interceptor.*;
import com.inprise.vbroker.PortableServerExt.*;
import org.omg.CORBA.ORB.*;
import com.inprise.vbroker.orb.*;

import org.omg.CORBA.INITIALIZE;

public class Init implements ServiceLoader
{

public void init (org.omg.CORBA.ORB orb)
    {
	InterceptorManagerControl control = null;

	try
	{
	    control = InterceptorManagerControlHelper.narrow(orb.resolve_initial_references("VisiBrokerInterceptorControl"));
	}
	catch (Exception e)
	{
	    e.printStackTrace();
	    
	    throw new INITIALIZE(e.toString());
	}

        try
        {
            BindInterceptorManager bindManager = (BindInterceptorManager) control.get_manager("Bind");

            bindManager.add(new OTS_ContextBindInterceptor());
        }
        catch (Exception e)
        {
	    e.printStackTrace();

            throw new INITIALIZE("Bind interceptor is not installed: "+e);
        }

        try
        {
            POALifeCycleInterceptorManager poaManager = (POALifeCycleInterceptorManager) control.get_manager("POALifeCycle");

            poaManager.add(new OTS_ContextPOALifeCycleInterceptor());
        }
        catch (Exception e)
        {
	    e.printStackTrace();

            throw new INITIALIZE("POALifeCycle interceptor is not installed: "+e);
        }
    }
    
public void init_complete (org.omg.CORBA.ORB orb)
    {
    }
    
public void shutdown (org.omg.CORBA.ORB orb)
    {
    }

};
