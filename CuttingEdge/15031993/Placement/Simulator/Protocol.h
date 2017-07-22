#ifndef PROTOCOL_
#define PROTOCOL_

#define ALL -1

enum lock_code { unlocked, read, write };

/* This is the base class for all replication protocols. */

class Protocol
{
public:
    ~Protocol ();

    long ReadQuorum; // What is a Read Quorum?
    long WriteQuorum; // What is a Write Quorum?
    double RecoveryTime; // What is the recovery time for a failed replica (being brough up-to-date)?
    char* name; // Name of replication protocol, e.g., "Available Copies"

    virtual void SetWriteQuorum (long) = 0;
    virtual void SetReadQuorum (long) = 0;
    virtual long GetMinimum () = 0;

protected:
    Protocol (long, long, double);
};

#endif
