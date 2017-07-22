#include <iostream.h>
#include "Voting.h"

Voting::Voting (long RQ, long WQ, double RT) : Protocol (RQ/2+1, WQ/2+1, RT)
{
    name = "Quorum Concensus";
}

Voting::~Voting () {}

void Voting::SetWriteQuorum (long WQ)
{
    if (WQ < 3)
	WQ = 3;    // Always have at least 3 replicas for voting to function efficiently

    WriteQuorum = WQ/2+1;
    SetReadQuorum(WQ);

    cout << "Write Quorum is " << WriteQuorum << "\n" << flush;
    cout << "Read Quorum is " << ReadQuorum << "\n" << flush;
}

void Voting::SetReadQuorum (long RQ) { ReadQuorum = RQ/2+1; }

long Voting::GetMinimum () { return 3; }
