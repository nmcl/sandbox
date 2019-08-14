/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ContextPOALifeCycleInterceptor.java,v 1.1 2000/02/25 14:06:04 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interceptors.Context.vbroker;

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.inprise.vbroker.interceptor.*;
import com.inprise.vbroker.PortableServerExt.*;
import com.inprise.vbroker.IOP.*;
import org.omg.CORBA.CompletionStatus;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;

public class OTS_ContextPOALifeCycleInterceptor implements POALifeCycleInterceptor
{
public void create (org.omg.PortableServer.POA poa,
		    org.omg.CORBA.PolicyListHolder policies_holder,
		    IORValueHolder iorTemplate,
		    InterceptorManagerControl control)
    {
	/*
	 * Could install ServerRequestInterceptor on specific poa.
	 * Until the OTS attribute is added, we will eventually require
	 * transactional contexts to have a specific name format,
	 * e.g., "...Transactional..."
	 */

	ServerRequestInterceptorManager manager = 
	    (ServerRequestInterceptorManager) control.get_manager("ServerRequest");
      
	manager.add(new OTS_ServerContextInterceptor());
  }
  
public void destroy (org.omg.PortableServer.POA poa)
    {
    }

};
