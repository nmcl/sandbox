/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef VOTING_H_
#define VOTING_H_

#ifndef AC_H_
#include "AC.h"
#endif



/* This is the Quorum Concensus replication protocol. */

class Voting: public AC
{
public:
    Voting (long, long, double); // Parameters are: ReadQuorum, WriteQuorum, Repair Time
    ~Voting ();

    void SetWriteQuorum (long);
    void SetReadQuorum (long);
    long GetMinimum ();
};

#endif
