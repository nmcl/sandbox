/*
 * Copyright (C) 1996, 1997,
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

#include <Common/Uid.h>

#include "Bank.h"

class TCPConnection;

class Applet
{
public:
    Applet (char* tid, TCPConnection* port, Bank::Outcome outcome);
    Applet (TCPConnection* port, ErrorStatus);
    ~Applet ();

    Boolean prepare ();
    Boolean abort ();
    Boolean commit (int cash);

private:
    Boolean transmitOutcome (BankActionStatus status);

    char* tid;
    TCPConnection* _port;
    int cashAmount;
    Bank::Outcome _result;
    ErrorStatus errorStatus;
};

#endif





























