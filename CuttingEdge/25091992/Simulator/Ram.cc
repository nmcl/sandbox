#include "Ram.h"

Ram::Ram(BusInterface *b, int priority) : Memory(b, priority)
{
}

Ram::~Ram()
{
}

int Ram::read(addr_r location)
{
    return (Memory::read(location));
}

void Ram::write(int value, addr_r location)
{
    Memory::write(value, location);
}
