#ifndef AC_
#define AC_

#include "Protocol.h"

/* This is the Available Copies replication protocol. */

class AC : public Protocol
{
public:
    AC (long, long, double); // Parameters are: ReadQuorum, WriteQuorum, RepairTime.
    ~AC ();
};

#endif
