/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef SERVER_H_
#define SERVER_H_

#ifndef PROCESS_H_
#include <ClassLib/Process.h>
#endif

#ifndef RANDOM_H_
#include <ClassLib/Random.h>
#endif

#ifndef BOOLEAN_H_
#include <Commom/Boolean.h>
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif

/* Instances of this class are nameserver objects. */

class Server : public Machine
{
public:
    Server (long id, RandomStream* sd);
    ~Server ();

    void setLost();
    void Body ();  // Look up name and return address to network
private:
    Boolean lost;
    RandomStream* serverDist;
    UniformStream* probGenerator;
    double findProbability;  // Probability that name sought is on this server
};

#endif
