/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef AC_H_
#define AC_H_

#ifndef PROTOCOL_H_
#include "Protocol.h"
#endif


/* This is the Available Copies replication protocol. */

class AC : public Protocol
{
public:
    AC (long, long, double); // Parameters are: ReadQuorum, WriteQuorum, RepairTime.
    virtual ~AC ();

    void SetWriteQuorum (long);
    void SetReadQuorum (long);
    long GetMinimum ();
};

#endif
