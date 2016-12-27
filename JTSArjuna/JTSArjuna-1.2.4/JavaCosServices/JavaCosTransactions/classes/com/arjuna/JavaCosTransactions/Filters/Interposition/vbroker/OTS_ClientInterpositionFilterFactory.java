/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientInterpositionFilterFactory.java,v 1.1 1999/09/10 15:01:17 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.Interposition.vbroker;

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
