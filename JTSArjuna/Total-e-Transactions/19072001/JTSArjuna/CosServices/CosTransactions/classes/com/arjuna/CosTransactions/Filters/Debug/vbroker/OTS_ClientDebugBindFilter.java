/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ClientDebugBindFilter.java,v 1.1.2.1 2000/11/02 16:54:19 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Debug.vbroker;

import com.arjuna.OrbCommon.ORBInterface;
import com.visigenic.vbroker.interceptor.*;
import com.visigenic.vbroker.IOP.*;
import org.omg.CORBA.BOA;
import org.omg.CORBA.CompletionStatus;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;

public class OTS_ClientDebugBindFilter implements BindInterceptor
{

public OTS_ClientDebugBindFilter ()
    {
	System.out.println("OTS_ClientDebugBindFilter created.");
    }
	    
public boolean bind (IORHolder ior, org.omg.CORBA.Object object, Closure closure)
    {
        return false;
    }

public boolean bind_failed (IORHolder ior, org.omg.CORBA.Object object, Closure closure)
    {
	System.out.println("OTS_ClientDebugBindFilter - bind failed.");
	
	return false;
    }

public void bind_succeeded (IOR ior, org.omg.CORBA.Object object, Closure closure)
    {
	System.out.println("OTS_ClientDebugBindFilter - bind succeeded.");
    }

public boolean rebind (IORHolder ior, org.omg.CORBA.Object object, Closure closure)
    {
        return false;
    }

public boolean rebind_failed (IORHolder ior, org.omg.CORBA.Object object, Closure closure)
    {
        return false;
    }

public void rebind_succeeded (IOR ior, org.omg.CORBA.Object object, Closure closure)
    {
    }

public void exception_occurred (IOR ior, org.omg.CORBA.Object object, org.omg.CORBA.Environment env, Closure closure)
    {
    }
    
};
