/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#ifndef NETWORK_H_
#define NETWORK_H_

#ifndef PROCESS_H_
#include <ClassLib/Process.h>
#endif

#ifndef RANDOM_H_
#include <ClassLib/Random.h>
#endif

enum NetworkType { LOCAL, LAN, WAN };

class Network : public Process
{
public:
    Network (RandomStream* nd, NetworkType wn);
    ~Network ();

    void Body ();
private:
    RandomStream* networkDist;
    NetworkType whichNetwork;
};

#endif
