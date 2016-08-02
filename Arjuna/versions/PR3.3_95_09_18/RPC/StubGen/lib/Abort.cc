/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Abort.cc,v 1.3 1996/04/15 13:25:35 nmcl Exp $
 */

static const char RCSid[] = "$Id: Abort.cc,v 1.3 1996/04/15 13:25:35 nmcl Exp $";

/*
 * Handle aborting of client side of RPC. This just does a
 * kill(getpid(), SIGQUIT) currently and relies on Arjuna cleanup
 */

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef ABORT_H_
#  include <StubGen/Abort.h>
#endif


rpcAbortHandler defaultHandler = 0;

rpcAbortHandler setHandler ( rpcAbortHandler newHandler )
{
    rpcAbortHandler oldHandler = defaultHandler;
    
    defaultHandler = newHandler;
    
    return oldHandler;
    
}

void rpcAbort ( RPC_Status rpcStat, Int32 svrStat )
{
    if ((defaultHandler == 0) || (*defaultHandler)(rpcStat,svrStat) == TRUE)
#ifndef WIN32
	kill(getpid(), SIGQUIT);
#else
	abort();
#endif
}
