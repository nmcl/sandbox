/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Abort.h,v 1.4 1993/04/02 10:20:27 ngdp Exp $
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

typedef Boolean (*rpc_abort_handler) ();

extern rpc_abort_handler set_handler(rpc_abort_handler);

extern void rpc_abort ();

#endif
