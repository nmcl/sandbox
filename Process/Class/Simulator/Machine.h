#ifndef Machine_
#define Machine_

#include "List.h"

class Stats
{
public:
    Stats();
    Stats(Stats*);
    ~Stats();

    double MTTFMean, MTTFStdDev, MTTRMean, MTTRStdDev;
    int ID, Speed;
};


class Machine
{
public:
    Machine(Stats*);
    ~Machine();

    TList* ActiveTransactions;    
    Stats* machine_info;
    boolean Available;
};

#endif

