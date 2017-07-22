#ifndef CLIENT_
#define CLIENT_

#include <Process.h>
#include <Random.h>
#include "Object.h"

/* Instances of this class are the clients which use the replicated objects. */

class Client : public Process
{
public:
    Client (long);
    ~Client ();

    void MakeCall (lock_code);
    void Body ();

    long ClientID;

private:
    ExponentialStream* ArrivalTime;
    ExponentialStream* ProcessTime;
    UniformStream* WaitTime;
};

#endif
