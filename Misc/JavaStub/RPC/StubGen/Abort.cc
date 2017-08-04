/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Abort.cc,v 1.2 1995/01/20 09:17:09 ngdp Exp $
 */

static const char RCSid[] = "$Id: Abort.cc,v 1.2 1995/01/20 09:17:09 ngdp Exp $";

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
	kill(getpid(), SIGQUIT);
}
