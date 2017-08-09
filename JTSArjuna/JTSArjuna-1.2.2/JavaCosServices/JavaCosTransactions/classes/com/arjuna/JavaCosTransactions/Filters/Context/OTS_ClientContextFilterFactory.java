/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientContextFilterFactory.java,v 1.3 1998/07/06 13:28:50 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.Context;

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
