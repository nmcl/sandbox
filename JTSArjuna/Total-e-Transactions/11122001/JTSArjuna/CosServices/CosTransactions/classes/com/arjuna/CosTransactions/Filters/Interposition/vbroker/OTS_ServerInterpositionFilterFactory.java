/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ServerInterpositionFilterFactory.java,v 1.1 2000/02/25 14:05:59 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Interposition.vbroker;

import com.visigenic.vbroker.interceptor.*;

/*
 * Unlike the client filter, we can get away with
 * one filter for the entire process.
 */

public class OTS_ServerInterpositionFilterFactory implements ServerInterceptorFactory
{

public ServerInterceptor create (com.visigenic.vbroker.IOP.TaggedProfile profile)
    {
        if (_theFilter == null)
        {
            _theFilter = new OTS_ServerInterpositionFilter();
        }
	
        return _theFilter;
    }

private OTS_ServerInterpositionFilter _theFilter = null;
    
};
