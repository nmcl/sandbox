#ifndef _Bus_
#define _Bus_
#include "common.h"
#include <iostream.h>

// InterfaceList is a list of the BusInterfaces which have been connected to the bus.

struct InterfaceList
{
    class BusInterface *this_int;
    InterfaceList *next_int;
};

class Bus
{
    struct InterfaceList *inlst, *head;
public:
    Bus();
    ~Bus();

    void connect(class BusInterface*);      // Connect a BusInterface to the bus
    void disconnect(class BusInterface*);   // Disconnect the BusInterface
    int send(addr_r, opcode, int, class BusInterface*, boolean*); // Send message
    int multi_send(addr_r, opcode, int, class BusInterface*, boolean*, InterfaceList**); // Multicast send
};
#endif
