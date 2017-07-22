/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef CLIENT_H_
#define CLIENT_H_

#ifndef PROCESS_H_
#include <Process.h>
#endif

#ifndef RANDOM_H_
#include <Random.h>
#endif

#ifndef OBJECT_H_
#include "Object.h"
#endif


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
    double rate;
    ExponentialStream* ArrivalTime;
    ExponentialStream* ProcessTime;
    UniformStream* WaitTime;
    Draw* OperationType;
};

#endif
