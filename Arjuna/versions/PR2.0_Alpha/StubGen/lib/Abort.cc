/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Abort.cc,v 1.6 1993/05/06 14:49:36 nsmw Exp $
 */

static const char RCSid[] = "$Id: Abort.cc,v 1.6 1993/05/06 14:49:36 nsmw Exp $";

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


rpc_abort_handler default_handler = 0;

rpc_abort_handler set_handler ( rpc_abort_handler new_handler)
{
    rpc_abort_handler old_handler = default_handler;
    
    default_handler = new_handler;
    
    return old_handler;
    
}

void rpc_abort ()
{
    if ((default_handler == 0) || (*default_handler)() == TRUE)
	kill(getpid(), SIGQUIT);
}
