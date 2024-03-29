/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ServerDebugFilterFactory.java,v 1.1 1999/09/10 14:58:02 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.Debug.vbroker;

import com.visigenic.vbroker.interceptor.*;

/*
 * Unlike the client side filter, we can get away with
 * a single filter for the entire process.
 */

public class OTS_ServerDebugFilterFactory implements ServerInterceptorFactory
{

public ServerInterceptor create (com.visigenic.vbroker.IOP.TaggedProfile profile)
    {
        if (_theFilter == null)
        {
            _theFilter = new OTS_ServerDebugFilter();
        }
	
        return _theFilter;
    }

private OTS_ServerDebugFilter _theFilter = null;
    
};
