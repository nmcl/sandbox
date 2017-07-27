#include "AC.h"

AC::AC (long RQ, long WQ, double RT) : Protocol(RQ, WQ, RT)
{
    name = "AvailableCopies";
}

AC::~AC () {}
