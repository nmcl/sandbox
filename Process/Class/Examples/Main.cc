#include <iostream.h>
#include <Process.h>

#include "MachineShop.h"

main()
{
    LWP_Thread::Initialize();

    MachineShop m;
    m.Await();
}
