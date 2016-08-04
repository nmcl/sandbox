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

class Server : public Machine
{
public:
    Server (long id);
    ~Server ();

    void setLost();
    void Body ();  
private:
    Boolean lost;
};

#endif
