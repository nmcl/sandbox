/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Abort.h,v 1.2 1995/01/20 10:18:00 ngdp Exp $
 */

#ifndef ABORT_H_
#define ABORT_H_

/*
 *
 * Handle Rpc aborts. 
 *     
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef CLSTATUS_H_
#  include <RPC/ClStatus.h>
#endif

typedef Boolean (*rpcAbortHandler) (RPC_Status, Int32);

extern rpcAbortHandler setHandler(rpcAbortHandler);

extern void rpcAbort (RPC_Status, Int32);

#endif
