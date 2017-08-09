/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientDebugFilterFactory.java,v 1.2 1998/07/06 13:28:56 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.Debug;

import com.visigenic.vbroker.interceptor.*;

/*
 * We want one filter per object.
 */

public class OTS_ClientDebugFilterFactory implements ClientInterceptorFactory
{

public ClientInterceptor create (org.omg.CORBA.Object object)
    {
	if (_theFilter == null)
	    _theFilter = new OTS_ClientDebugFilter(object);

	return _theFilter;
    }

private OTS_ClientDebugFilter _theFilter = null;
    
};
