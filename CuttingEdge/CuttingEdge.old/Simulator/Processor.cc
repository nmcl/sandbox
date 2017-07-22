#include "Processor.h"

Processor::Processor(BusInterface *b, int priority) : Connector(b, priority)
{
}

Processor::~Processor()
{
}

int Processor::read(addr_r range)
{
    return(bi->read(ON_BUS, range));
}

void Processor::write(int value, addr_r range)
{
    bi->write(ON_BUS, value, range);
}
