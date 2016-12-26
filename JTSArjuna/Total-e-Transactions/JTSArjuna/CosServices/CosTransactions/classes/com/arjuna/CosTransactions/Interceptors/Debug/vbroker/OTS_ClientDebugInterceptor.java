/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ClientDebugInterceptor.java,v 1.1 2000/02/25 14:06:08 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interceptors.Debug.vbroker;

import com.arjuna.OrbCommon.ORBInterface;
import com.inprise.vbroker.interceptor.*;
import com.inprise.vbroker.IOP.*;
import com.inprise.vbroker.orb.*;
import org.omg.CORBA.portable.*;
import org.omg.CORBA.CompletionStatus;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;

public class OTS_ClientDebugInterceptor implements ClientRequestInterceptor
{

public void preinvoke_premarshal (org.omg.CORBA.Object target,
				  String operation,
				  ServiceContextListHolder sctx,
				  Closure closure)
{
    System.out.println("OTS_ClientDebugInterceptor.preinvoke_premarshal called for "+operation);

    closure.object = operation;
}

public void preinvoke_postmarshal (org.omg.CORBA.Object target,
				   com.inprise.vbroker.CORBA.portable.OutputStream buf,
				   Closure closure)
    {
	System.out.println("OTS_ClientDebugInterceptor.postmarshal called for "+(String) closure.object);
    }

public void postinvoke (org.omg.CORBA.Object target, ServiceContext[] sctx,
			com.inprise.vbroker.CORBA.portable.InputStream istr,
			org.omg.CORBA.Environment env, Closure closure)
    {
	System.out.println("OTS_ClientDebugInterceptor.postinvoke called for "+(String) closure.object);
    }

public void exception_occurred (org.omg.CORBA.Object target,
				org.omg.CORBA.Environment env,
				Closure closure)
    {
	System.out.println("OTS_ClientDebugInterceptor.exception_occurred called for "+(String) closure.object);
    }

};
