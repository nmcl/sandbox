#ifndef Common_
#define Common_

enum boolean { false, true };
enum lockcode { unlocked, read, write };

// Parameters controlling size of the simulation

const int TotalMachines = 8;
const int TotalObjects = 8;
const int TotalTransactions = 8;
const double MinTransactionTime = 1.0;
const double MaxTransactionTime = 10.0;

const double Delta = 1.0; // Delay period before attempting to re-acquire locks

// Define other simulation parameters
const int ReadWriteRatio = 10;

#endif

