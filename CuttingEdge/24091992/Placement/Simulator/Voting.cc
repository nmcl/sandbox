#include "Voting.h"

Voting::Voting (long RQ, long WQ, double RT) : Protocol (RQ/2+1, WQ/2+1, RT)
{
    name = "Quorum Concensus";
}

Voting::~Voting () {}
