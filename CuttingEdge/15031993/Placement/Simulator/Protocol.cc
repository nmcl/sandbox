#include <iostream.h>
#include "Protocol.h"

Protocol::Protocol (long RQ = 1, long WQ = ALL, double RT = 0)
{
    ReadQuorum = RQ;
    WriteQuorum = WQ;
    RecoveryTime = RT;
    name = 0;
}

Protocol::~Protocol () {}
