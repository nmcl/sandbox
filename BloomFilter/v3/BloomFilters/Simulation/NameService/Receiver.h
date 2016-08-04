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

/* Instances of this class are clients who simulate receiving messages from
   the name server and sending messages to the actual recipient. In the
   simulation, the recipient is the Results object */

class Receiver : public Process
{
public:
    Receiver (RandomStream* sd);
    ~Receiver ();

    void Body (); 
private:
    RandomStream* receiverDist;
};

#endif
