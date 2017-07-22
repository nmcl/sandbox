#include "Connector.h"

Connector::Connector(BusInterface *b, int priority)
{
    bi = b;
    bi->add_device(this, priority);
}

Connector::~Connector()
{
}
