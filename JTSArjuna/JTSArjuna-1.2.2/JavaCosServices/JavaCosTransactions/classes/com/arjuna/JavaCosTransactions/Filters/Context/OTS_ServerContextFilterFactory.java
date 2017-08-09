/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ServerContextFilterFactory.java,v 1.3 1998/07/06 13:28:53 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.Context;

import com.visigenic.vbroker.interceptor.*;

/*
 * Unlike the client side filter, we can get away with
 * a single filter for the entire process.
 */

public class OTS_ServerContextFilterFactory implements ServerInterceptorFactory
{

public ServerInterceptor create (com.visigenic.vbroker.IOP.TaggedProfile profile)
    {
        if (_theFilter == null)
        {
            _theFilter = new OTS_ServerContextFilter();
        }
	
        return _theFilter;
    }

private OTS_ServerContextFilter _theFilter = null;
    
};
