/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ClientDebugFilterFactory.java,v 1.1 2000/02/25 14:05:47 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Debug.vbroker;

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
