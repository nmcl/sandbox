/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id$
 */

#ifndef APPLET_H_
#define APPLET_H_

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#include <PortAddress.h>
#include <RelPort.h>

class ostream;

class Applet
{
public:
    Applet (ReliablePort& port, int outcome);
    ~Applet ();

    Boolean prepare ();
    Boolean abort ();
    Boolean commit (int cash);

private:
    Boolean transmitOutcome (int outcome);
    
    ReliablePort& _port;
    int cashAmount;
    int _result;
};

#endif





























