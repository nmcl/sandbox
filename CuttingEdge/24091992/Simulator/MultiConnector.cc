#include "MultiConnector.h"

MultiConnector::MultiConnector(BusInterface *b, int p) : Connector(b, p)
{
}

MultiConnector::~MultiConnector()
{
}

int MultiConnector::read(addr_r range)
{
    return(bi->read(ON_BUS, range));
}

void MultiConnector::write(int value, addr_r range)
{
    bi->write(ON_BUS, value, range);
}

data_buffer MultiConnector::multi_read(addr_r range)
{
    return(bi->multi_read(ON_BUS, range));
}

void MultiConnector::multi_write(int value, addr_r range)
{
    bi->write(ON_BUS, value, range);
}
