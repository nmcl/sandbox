/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Abort.h,v 1.5 1993/06/17 15:17:00 ngdp Exp $
 */

#ifndef ABORT_H_
#define ABORT_H_

/*
 *
 * Handle Rpc aborts. 
 *     
 */

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

typedef Boolean (*rpcAbortHandler) ();

extern rpcAbortHandler setHandler(rpcAbortHandler);

extern void rpcAbort ();

#endif
