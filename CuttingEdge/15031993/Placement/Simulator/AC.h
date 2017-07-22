#ifndef AC_
#define AC_

#include "Protocol.h"

/* This is the Available Copies replication protocol. */

class AC : public Protocol
{
public:
    AC (long, long, double); // Parameters are: ReadQuorum, WriteQuorum, RepairTime.
    ~AC ();

    void SetWriteQuorum (long);
    void SetReadQuorum (long);
    long GetMinimum ();
};

#endif
