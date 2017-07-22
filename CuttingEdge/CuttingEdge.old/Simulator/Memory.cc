#include "Memory.h"

Memory::Memory(BusInterface *b, int priority) : Connector(b, priority)
{
    for (int i = 0; i < MEMORY_SIZE; i++)
	contents[i] = -1;
}

Memory::~Memory()
{
}

int Memory::read(addr_r location)
{
    return(contents[location]);
}

void Memory::write(int value, addr_r location)
{
    contents[location] = value;
}

