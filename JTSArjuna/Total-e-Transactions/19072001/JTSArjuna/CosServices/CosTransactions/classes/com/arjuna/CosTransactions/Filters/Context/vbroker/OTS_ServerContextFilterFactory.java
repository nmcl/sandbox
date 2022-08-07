/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Tyne and Wear,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerContextFilterFactory.java,v 1.2 2000/04/14 15:40:06 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Context.vbroker;

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
