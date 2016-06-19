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

    virtual void SetWriteQuorum (long);
    virtual void SetReadQuorum (long);
    virtual long GetMinimum ();

    virtual Opcode PerformOperation (lock_code, Client*);
    virtual boolean CheckQuorum (lock_code, Client*);
   
private:
    Opcode unlockGroup (Client*);
    Opcode lockGroup (Client*, lock_code, long);
    void checkGroup (long&, long&, Client*, lock_code);
};

#endif
