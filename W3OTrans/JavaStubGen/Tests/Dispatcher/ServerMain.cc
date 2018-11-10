/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ServerDispatcher.cc,v 1.1 1994/07/01 10:53:49 ngdp Exp $
 */

#include <Config/Configure.h>

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef SERVERDISPATCHER_H_
#  include <StubGen/ServerDispatcher.h>
#endif

#ifndef THEATREDISPATCHER_H_
#  include "TheatreDispatcher.h"
#endif


int main (int argc, char** argv)
{
    ServerDispatcher mainDispatcher;
    TheatreDispatcher base1;

    cout << "mainDispatcher id: " << mainDispatcher.dispatcherName() << endl;
    cout << "TheatreDispatcher id: " << base1.dispatcherName() << endl;

    mainDispatcher.initialise(argc, argv);
    mainDispatcher.addDispatcher(&base1);

    mainDispatcher.begin();
    mainDispatcher.end();
}
