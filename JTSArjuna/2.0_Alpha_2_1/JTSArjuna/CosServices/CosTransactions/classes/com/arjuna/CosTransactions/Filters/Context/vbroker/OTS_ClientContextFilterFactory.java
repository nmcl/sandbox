/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Tyne and Wear,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ClientContextFilterFactory.java,v 1.2 2000/04/14 15:40:06 nmcl Exp $
 */

package com.arjuna.CosTransactions.Filters.Context.vbroker;

import com.visigenic.vbroker.interceptor.*;

/*
 * We want one filter per object.
 */

public class OTS_ClientContextFilterFactory implements ClientInterceptorFactory
{

public ClientInterceptor create (org.omg.CORBA.Object object)
    {
	return new OTS_ClientContextFilter(object);
    }

};
