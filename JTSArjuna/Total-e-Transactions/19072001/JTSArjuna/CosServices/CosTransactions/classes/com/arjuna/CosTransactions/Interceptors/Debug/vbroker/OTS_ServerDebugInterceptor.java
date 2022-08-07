/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerDebugInterceptor.java,v 1.1 2000/02/25 14:06:09 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interceptors.Debug.vbroker;

import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.inprise.vbroker.interceptor.*;
import com.inprise.vbroker.IOP.*;
import com.inprise.vbroker.orb.*;
import org.omg.CORBA.portable.*;
import org.omg.CORBA.CompletionStatus;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;

public class OTS_ServerDebugInterceptor implements ServerRequestInterceptor
{

public void preinvoke (org.omg.CORBA.Object target, String operation,
		       ServiceContext[] sctx,
		       com.inprise.vbroker.CORBA.portable.InputStream buf,
		       Closure closure)
    {
	System.out.println("OTS_ServerDebugInterceptor.preinvoke called for "+operation);

	closure.object = operation;
    }

public void postinvoke_premarshal (org.omg.CORBA.Object target,
				   ServiceContextListHolder sctx,
				   org.omg.CORBA.Environment env,
				   Closure closure)
    {
	System.out.println("OTS_ServerDebugInterceptor.postinvoke_premarshal called for "+(String) closure.object);
    }

public void postinvoke_postmarshal (org.omg.CORBA.Object target,
				    com.inprise.vbroker.CORBA.portable.OutputStream buf,
				    Closure closure)
    {
	System.out.println("OTS_ServerDebugInterceptor.postinvoke_postmarshal called for "+(String) closure.object);
    }

public void exception_occurred (org.omg.CORBA.Object target,
				org.omg.CORBA.Environment env,
				Closure closure)
    {
	System.out.println("OTS_ServerDebugInterceptor.exception_occurred called for "+(String) closure.object);
    }

};
