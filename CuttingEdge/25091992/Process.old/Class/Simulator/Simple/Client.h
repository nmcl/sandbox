#ifndef CLIENT_
#define CLIENT_

#include <Process.h>
#include <Random.h>
#include "Object.h"

class Client : public Process
{
private:
    ExponentialStream* ArrivalTime;
    ExponentialStream* ProcessTime;
    UniformStream* WaitTime;

public:
    Client(long);
    ~Client();

    void MakeCall(lock_code);
    void Body();

    long ClientID;
};

#endif
