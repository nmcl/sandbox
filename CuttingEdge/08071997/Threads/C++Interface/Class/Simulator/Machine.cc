#include "Common.h"
#include "Machine.h"

// Machine data
Stats::Stats(Stats* use)
{
    ID = use->ID;
    MTTFMean = use->MTTFMean;
    MTTFStdDev = use->MTTFStdDev;
    MTTRMean = use->MTTRMean;
    MTTRStdDev = use->MTTRStdDev;
    Speed = use->Speed;
}

Stats::Stats()
{
    ID = 0;
    MTTFMean = 0;
    MTTFStdDev = 0;
    MTTRMean = 0;
    MTTRStdDev = 0;
    Speed = 0;
}

Stats::~Stats() {}



// Machine characteristics
Machine::Machine(Stats *use)
{
    // Initialise Machine state
    machine_info = use;
    Available = true; // Machine "state" (e.g., up/down)
    ActiveTransactions = 0; // Current transactions
}

Machine::~Machine() {}
