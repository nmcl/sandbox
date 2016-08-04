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

#ifndef QUEUE_H_
#include "Queue.h"
#endif

#ifndef MACHINE_H_
#include "Machine.h"
#endif

extern Queue* networkQueue;
extern Queue* resultsQueue;

/* Instances of this class are recipients with a small prob. of being erroneous */

class Server : public Machine
{
public:
    Server (long id, double fhp);
    ~Server ();
    void setLost();
    void Body ();  // Return message for broadcasting if we've got a false hit
private:
    Boolean lost;
    UniformStream* probGenerator;
    double falseHitProb;  // Probability that name sought isn't on this server
};

#endif
