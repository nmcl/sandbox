/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientInterpositionFilterFactory.java,v 1.2 1998/07/06 13:29:01 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.Interposition;

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
