/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerDebugFilterFactory.java,v 1.1 2000/02/25 14:05:48 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Debug.vbroker;

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
