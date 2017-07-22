#ifndef VOTING_H_
#define VOTING_H_

#include "Protocol.h"

/* This is the Quorum Concensus replication protocol. */

class Voting: public Protocol
{
public:
    Voting (long, long, double); // Parameters are: ReadQuorum, WriteQuorum, Repair Time
    ~Voting ();

    void SetWriteQuorum (long);
    void SetReadQuorum (long);
    long GetMinimum ();
};

#endif
