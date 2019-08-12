/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ClientInterpositionBindFilter.java,v 1.1.2.1.2.1 2000/12/13 15:40:24 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Interposition.vbroker;

import com.arjuna.ArjunaCommon.Common.*;
import com.arjuna.OrbCommon.ORBInterface;
import com.arjuna.CosTransactions.*;
import org.omg.CosTransactions.*;
import com.arjuna.ArjunaOTS.*;
import com.visigenic.vbroker.interceptor.*;
import com.visigenic.vbroker.IOP.*;
import org.omg.CORBA.BOA;
import org.omg.CORBA.CompletionStatus;

import org.omg.CORBA.SystemException;
import org.omg.CORBA.BAD_PARAM;
import org.omg.CORBA.UNKNOWN;

public class OTS_ClientInterpositionBindFilter implements BindInterceptor
{

public OTS_ClientInterpositionBindFilter ()
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.CONSTRUCTORS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ClientInterpositionBindFilter created.");
	}
    }
	    
public boolean bind (IORHolder ior, org.omg.CORBA.Object object, Closure closure)
    {
        return false;
    }

public boolean bind_failed (IORHolder ior, org.omg.CORBA.Object object, Closure closure)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ClientInterpositionBindFilter - bind failed.");
	}
	
	return false;
    }

public void bind_succeeded (IOR ior, org.omg.CORBA.Object object, Closure closure)
    {
	if (DebugController.enabled())
	{
	    DebugController.controller().println(DebugLevel.FUNCTIONS, VisibilityLevel.VIS_PUBLIC,
					       (FacilityCode.FAC_OTS | FacilityCode.FAC_FILTER), "OTS_ClientInterpositionBindFilter - bind succeeded.");
	}
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
