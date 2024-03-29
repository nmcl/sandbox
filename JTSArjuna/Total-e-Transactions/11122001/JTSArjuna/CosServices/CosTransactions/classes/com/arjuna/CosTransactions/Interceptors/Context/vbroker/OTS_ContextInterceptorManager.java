/*
 * Copyright (C) 1998, 1999, 2000,
 *
 * Arjuna Solutions Limited,
 * Newcastle upon Tyne,
 * Tyne and Wear,
 * UK.  
 *
 * $Id: OTS_ContextInterceptorManager.java,v 1.1 2000/02/25 14:06:03 nmcl Exp $
 */

package com.arjuna.CosTransactions.Interceptors.Context.vbroker;

import java.lang.Thread;
import java.util.Hashtable;

/*
 * The latest version of VisiBroker has no way of turning filters off
 * once they are registered. This presents a problem when doing narrow
 * to a remote object within an interceptor for the purposes of context
 * propagation:
 *
 * (a) the server does narrow to see if the destination object is a
 *     transactional object and so whether it expects a context. Narrow
 *     is implemented as an RPC (_is_a) on the ORB object.
 * (b) this causes the server interceptor to become a client and use
 *     the client interceptor.
 * (c) the client interceptor does narrow on the destination object
 *     to see if it should transmit a context to it(!)
 * (d) this causes the client filter to start a new round of RPC
 *     intercepting, and we go into an infinite loop!
 *
 * So, this class allows us to selectively disable the filtering
 * on a per thread basis.
 */
 
public class OTS_ContextInterceptorManager
{

public static synchronized boolean isDisabled (Thread t)
    {
	return _disabledList.contains(t);
    }

public static synchronized void disable (Thread t)
    {
	_disabledList.put(t, t);  // doesn't matter what we put
    }

public static synchronized void enable (Thread t)
    {
	_disabledList.remove(t);
    }
    
private static Hashtable _disabledList = new Hashtable();
    
};
