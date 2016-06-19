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

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef PROCESS_H_
#  include <ClassLib/Process.h>
#endif

#ifndef RANDOM_H_
#  include <ClassLib/Random.h>
#endif

#ifndef OBJECT_H_
#  include "Object.h"
#endif


/* Instances of this class are the clients which use the replicated objects. */

class Client : public Process
{
public:
    Client (long, double);
    ~Client ();

    Boolean MakeCall (lock_code);
    void Body ();

    double OperationsAttempted ();
    double OperationsCompleted ();

    double GetResponseTime ();

private:
    ExponentialStream* ArrivalTime;
    ExponentialStream* ProcessTime;
    UniformStream* WaitTime;
    Draw* OperationType;
    double NumberOfOperations, OperationsPerformed;
    double ResponseTime, intervalOperations;
    double totalRT, intervalRT;

public:
    long ClientID;
};

#endif
