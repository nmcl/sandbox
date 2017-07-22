#ifndef CLIENT_
#define CLIENT_

#include <Process.h>
#include <Random.h>
#include "Object.h"

/* Instances of this class are the clients which use the replicated objects. */

class Client : public Process
{
public:
    Client (long, double);
    ~Client ();

    void MakeCall (lock_code);
    void Body ();
    double GetRate ();
    void SetRate (double);

    long ClientID;

private:
    double rate;
    ExponentialStream* ArrivalTime;
    ExponentialStream* ProcessTime;
    UniformStream* WaitTime;
    Draw* OperationType;
};

#endif
