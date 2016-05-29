/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef RECEIVER_H_
#define RECEIVER_H_

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
extern Queue* receiverQueue;

/* Instances of this class are ient who simulate receiving messages from
   the name server and sending messages to the actual recipient. In the
   simulation, the recipient is the Results object */

class Receiver : public Process
{
public:
    Receiver (long id, RandomStream* sd);
    ~Receiver ();

    void setLost();
    void Body (); 
private:
    RandomStream* receiverDist;
};

#endif
