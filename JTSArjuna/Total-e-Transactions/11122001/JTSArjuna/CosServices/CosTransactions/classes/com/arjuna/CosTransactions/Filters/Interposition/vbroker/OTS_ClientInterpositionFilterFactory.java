/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ClientInterpositionFilterFactory.java,v 1.1 2000/02/25 14:05:57 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Interposition.vbroker;

import com.visigenic.vbroker.interceptor.*;

/*
 * We want one filter per object.
 */

public class OTS_ClientInterpositionFilterFactory implements ClientInterceptorFactory
{

public ClientInterceptor create (org.omg.CORBA.Object object)
    {
	return new OTS_ClientInterpositionFilter(object);
    }
    
};
