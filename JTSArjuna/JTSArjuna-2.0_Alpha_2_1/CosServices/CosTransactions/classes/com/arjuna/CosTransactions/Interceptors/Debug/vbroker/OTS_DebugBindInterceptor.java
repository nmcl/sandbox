/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_DebugBindInterceptor.java,v 1.1 2000/02/25 14:06:08 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interceptors.Debug.vbroker;

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

public class OTS_DebugBindInterceptor implements BindInterceptor
{

public OTS_DebugBindInterceptor ()
    {
	System.out.println("OTS_DebugBindInterceptor created.");
    }

public IORValue bind (IORValue ior, org.omg.CORBA.Object object,
		      boolean rebind, Closure closure)
    {
	System.out.println("OTS_DebugBindInterceptor - bind "+object);

        return ior;
    }
    
public IORValue bind_failed (IORValue ior, org.omg.CORBA.Object object, Closure closure)
    {
	System.out.println("OTS_DebugBindInterceptor - bind_failed "+object);

	return ior;
    }	    

public void bind_succeeded (IORValue ior, org.omg.CORBA.Object object,
			    int index, InterceptorManagerControl control,
			    Closure closure)
    {
	System.out.println("OTS_DebugBindInterceptor - bind_succeeded "+object);
    }

public void exception_occurred (IORValue ior, org.omg.CORBA.Object object,
				org.omg.CORBA.Environment env, Closure closure)
    {
	System.out.println("OTS_DebugBindInterceptor - exception_occurred "+object);
    }
    
};
