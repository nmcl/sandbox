#include <iostream.h>
#include <Process.h>

#include "MachineShop.h"

void main()
{
    GNU_Thread::Initialize(); // Initialize the threads package.

    MachineShop m; // Start up the real main body of the simulation.
    m.Await(); // Suspend main's thread (NOTE: this MUST be done by all applications.)
}
