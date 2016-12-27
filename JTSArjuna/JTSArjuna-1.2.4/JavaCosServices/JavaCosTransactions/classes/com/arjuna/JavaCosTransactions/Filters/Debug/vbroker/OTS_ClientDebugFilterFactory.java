/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientDebugFilterFactory.java,v 1.1 1999/09/10 14:57:59 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.Debug.vbroker;

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
