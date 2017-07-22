#include <iostream.h>
#include "AC.h"

AC::AC (long RQ, long WQ, double RT) : Protocol(RQ, WQ, RT)
{
    name = "AvailableCopies";
}

AC::~AC () {}

void AC::SetWriteQuorum (long  WQ)
{
    WriteQuorum = WQ;
    SetReadQuorum(1);

    cout << "Write Quorum is " << WriteQuorum << "\n" << flush;
    cout << "Read Quorum is " << ReadQuorum << "\n" << flush;
}

void AC::SetReadQuorum (long RQ) { ReadQuorum = RQ; }

long AC::GetMinimum () { return 1; }
