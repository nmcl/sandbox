/*
 * Copyright (C) 1996, 1997, 1998, 1999,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ClientContextFilterFactory.java,v 1.1 1999/09/10 14:54:09 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.Context.vbroker;

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
