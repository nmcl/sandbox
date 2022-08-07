/*
 * Copyright (C) 2001,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ContextPropagationManager.javatmpl,v 1.1.2.2.4.4.2.1.50.1 2001/08/01 14:20:37 jcoleman Exp $
 */

package com.arjuna.CosTransactions;

import com.arjuna.ArjunaCommon.Common.PropertyManager;
import com.arjuna.ArjunaCommon.Common.FatalError;
import com.arjuna.ArjunaCommon.Common.ErrorStream;
import com.arjuna.OrbCommon.ORBInterface;

/**
 * Registers the appropriate filter with the ORB.
 *
 * @author Mark Little (mark@arjuna.com)
 * @version $Id: OTS_ContextPropagationManager.javatmpl,v 1.1.2.2.4.4.2.1.50.1 2001/08/01 14:20:37 jcoleman Exp $
 * @since JTS 2.1.
 */

public class OTS_ContextPropagationManager
{
    
public OTS_ContextPropagationManager ()
    {
	String contextMode = PropertyManager.getProperty(OTS_Environment.OTS_CONTEXT_PROP_MODE);
	boolean interposition = true;

	if (contextMode != null)
	{
	    if (contextMode.equals("CONTEXT"))
		interposition = false;
	    else
	    {
		if (contextMode.equals("NONE"))
		    return;
	    }
	}
	

	    





	if (interposition)
	    PropertyManager.setProperty("org.omg.PortableInterceptor.ORBInitializerClass.com.arjuna.CosTransactions.Interceptors.Interposition.orbix2000.OTS_InterpositionORBInitializerImpl", "com.arjuna.CosTransactions.Interceptors.Interposition.orbix2000.OTS_InterpositionORBInitializerImpl");
	else
	    PropertyManager.setProperty("org.omg.PortableInterceptor.ORBInitializerClass.com.arjuna.CosTransactions.Interceptors.Context.orbix2000.OTS_ContextORBInitializerImpl", "com.arjuna.CosTransactions.Interceptors.Context.orbix2000.OTS_ContextORBInitializerImpl");

    }

}

