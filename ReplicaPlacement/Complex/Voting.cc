/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

#include <iostream.h>

#ifndef VOTING_H_
#include "Voting.h"
#endif


Voting::Voting (long RQ, long WQ, double RT) : Protocol (RQ/2+1, WQ/2+1, RT)
{
    name = "Quorum Concensus";
}

Voting::~Voting () {}

void Voting::SetWriteQuorum (long WQ)
{
    if (WQ < 3)
	WQ = 3;

    WriteQuorum = WQ/2+1;
    SetReadQuorum(WQ);

    cout << "Write Quorum is " << WriteQuorum << "\n" << flush;
    cout << "Read Quorum is " << ReadQuorum << "\n" << flush;
}

void Voting::SetReadQuorum (long RQ) { ReadQuorum = RQ/2+1; }

long Voting::GetMinimum () { return 3; }
