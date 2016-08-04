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
#include <ClassLib/Process.h>
#endif

#ifndef RANDOM_H_
#include <ClassLib/Random.h>
#endif

#ifndef QUEUE_H_
#include "Queue.h"
#endif

/* Client just generates requests for service */

class Client : public Process
{
public:
    Client (RandomStream* ad, RandomStream* ws);
    ~Client ();

    void Body ();
private:
    RandomStream* arrivalDist;
    RandomStream* whichServer;
};

#endif
