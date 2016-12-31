/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.
 *
 * $Id: OTS_InterpositionBindInterceptor.java,v 1.1 2000/02/25 14:06:13 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interceptors.Interposition.vbroker;

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.inprise.vbroker.interceptor.*;
import com.inprise.vbroker.IOP.*;
import org.omg.CORBA.CompletionStatus;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;

public class OTS_InterpositionBindInterceptor implements BindInterceptor
{

public OTS_InterpositionBindInterceptor ()
    {
    }

public IORValue bind (IORValue ior, org.omg.CORBA.Object object,
		      boolean rebind, Closure closure)
    {
        return ior;
    }
    
public IORValue bind_failed (IORValue ior, org.omg.CORBA.Object object, Closure closure)
    {
	return ior;
    }

public void bind_succeeded (IORValue ior, org.omg.CORBA.Object object,
			    int index, InterceptorManagerControl control,
			    Closure closure)
    {
	ClientRequestInterceptorManager clientManager = (ClientRequestInterceptorManager) control.get_manager("ClientRequest");
	clientManager.add(new OTS_ClientInterpositionInterceptor());
    }

public void exception_occurred (IORValue ior, org.omg.CORBA.Object object,
				org.omg.CORBA.Environment env, Closure closure)
    {
    }
    
};
