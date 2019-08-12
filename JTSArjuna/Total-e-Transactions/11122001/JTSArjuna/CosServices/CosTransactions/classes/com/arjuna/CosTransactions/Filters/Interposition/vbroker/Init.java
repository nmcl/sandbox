/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Init.java,v 1.1.2.1.2.1.18.1 2000/12/13 15:40:23 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Interposition.vbroker;

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
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "Installing OTS_InterpositionPropagation Interceptors");
	}

        try
        {
            ChainBindInterceptor bind = ChainBindInterceptorHelper.narrow(orb.resolve_initial_references("ChainBindInterceptor"));
            bind.add(new OTS_ClientInterpositionBindFilter());
        }
        catch (InvalidName e)
        {
            throw new INITIALIZE("Bind interceptor is not installed: "+e);
        }

        try
        {
            ChainClientInterceptorFactory clientFactory = ChainClientInterceptorFactoryHelper.narrow(orb.resolve_initial_references("ChainClientInterceptorFactory"));
            clientFactory.add(new OTS_ClientInterpositionFilterFactory());
        }
        catch (InvalidName e)
        {
            throw new INITIALIZE("Client interceptor factory is not installed: "+e);
        }

        try
        {
            ChainServerInterceptorFactory serverFactory = ChainServerInterceptorFactoryHelper.narrow(orb.resolve_initial_references("ChainServerInterceptorFactory"));
            serverFactory.add(new OTS_ServerInterpositionFilterFactory());
        }
        catch (InvalidName e)
        {
            throw new INITIALIZE("Server interceptor factory is not installed: "+e);
        }
    }
    
};
