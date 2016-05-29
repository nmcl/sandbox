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

/* Client just generates requests for lookup table */

class Client : public Process
{
public:
    Client (RandomStream* ad);
    ~Client ();

    void Body ();
private:
    RandomStream* arrivalDist;
};

#endif
