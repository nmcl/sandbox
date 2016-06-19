/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef PROTOCOL_H_
#include "Protocol.h"
#endif


Protocol::Protocol (long RQ = 1, long WQ = ALL, double RT = 0)
                                                             : ReadQuorum(RQ),
							       WriteQuorum(WQ),
							       RecoveryTime(RT),
							       name(0)
{
}

Protocol::~Protocol () {}
