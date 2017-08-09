/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ServerInterpositionFilterFactory.java,v 1.2 1998/07/06 13:29:03 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.Interposition;

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
