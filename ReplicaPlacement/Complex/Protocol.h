/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef PROTOCOL_H_
#define PROTOCOL_H_

#define ALL -1

enum lock_code { unlocked, read, write };

/*
 * This is the base class for all replication protocols.
 * It is assumed that the minimum number of replicas allowed is enough
 * to tolerate 1 failure.
 */

class Protocol
{
public:
    virtual ~Protocol ();

    long ReadQuorum; // What is a Read Quorum?
    long WriteQuorum; // What is a Write Quorum?
    double RecoveryTime; // What is the recovery time for a failed replica (being brought up-to-date)?
    char* name; // Name of replication protocol, e.g., "Available Copies"

    virtual void SetWriteQuorum (long) = 0;
    virtual void SetReadQuorum (long) = 0;
    virtual long GetMinimum () = 0;

protected:
    Protocol (long, long, double);
};

#endif
