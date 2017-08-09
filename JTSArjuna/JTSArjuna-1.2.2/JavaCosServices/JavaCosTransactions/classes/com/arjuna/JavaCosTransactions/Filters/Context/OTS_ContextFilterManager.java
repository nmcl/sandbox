/*
 * Copyright (C) 1996, 1997, 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: OTS_ContextFilterManager.java,v 1.2 1998/07/06 13:28:51 nmcl Exp $
 */

package com.arjuna.JavaCosTransactions.Filters.Context;

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
 
public class OTS_ContextFilterManager
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
