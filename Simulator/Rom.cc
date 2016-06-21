#include "Rom.h"

using std::cout;
using std::cerr;

Rom::Rom(BusInterface *b, int priority) : Memory(b, priority)
{
}

Rom::~Rom()
{
}

int Rom::read(addr_r location)
{
    return(Memory::read(location));
}

void Rom::write(int value, addr_r location)
{
    cerr << "Error trying to write to read only memory!\n";
}
