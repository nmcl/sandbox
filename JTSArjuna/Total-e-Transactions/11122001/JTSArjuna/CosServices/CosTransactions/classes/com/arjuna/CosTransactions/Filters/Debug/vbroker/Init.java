/*
 * Copyright (C) 1996, 1997, 1998, 1999, 2000,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Init.java,v 1.1.2.1.2.1 2000/11/21 12:03:49 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Debug.vbroker;

import com.arjuna.ArjunaCommon.Common.*;
import java.util.*;
import com.visigenic.vbroker.interceptor.*;
import com.visigenic.vbroker.orb.*;

import org.omg.CORBA.ORBPackage.InvalidName;
import org.omg.CORBA.INITIALIZE;

public class Init extends ServiceInit
{
    
public void init (org.omg.CORBA.ORB orb, Properties properties)
    {
	if (DebugController.enabled())
	    System.out.println("Installing OTS_DebugPropagation Interceptors");

        try
        {
            ChainBindInterceptor bind = ChainBindInterceptorHelper.narrow(orb.resolve_initial_references("ChainBindInterceptor"));
            bind.add(new OTS_ClientDebugBindFilter());
        }
        catch (InvalidName e)
        {
            throw new INITIALIZE("Bind interceptor is not installed: "+e);
        }

        try
        {
            ChainClientInterceptorFactory clientFactory = ChainClientInterceptorFactoryHelper.narrow(orb.resolve_initial_references("ChainClientInterceptorFactory"));
            clientFactory.add(new OTS_ClientDebugFilterFactory());
        }
        catch (InvalidName e)
        {
            throw new INITIALIZE("Client interceptor factory is not installed: "+e);
        }

        try
        {
            ChainServerInterceptorFactory serverFactory = ChainServerInterceptorFactoryHelper.narrow(orb.resolve_initial_references("ChainServerInterceptorFactory"));
            serverFactory.add(new OTS_ServerDebugFilterFactory());
        }
        catch (InvalidName e)
        {
            throw new INITIALIZE("Server interceptor factory is not installed: "+e);
        }
    }
    
};
