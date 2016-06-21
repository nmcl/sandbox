#ifndef _BusInterface_
#define _BusInterface_
#include "common.h"
#include <iostream>

class BusInterface
{
    boolean active;        // current status of this device
    class Connector *device;     // device connected to this interface
public:
    addr_r start, end;     // address range to monitor
    int identifier;        // identify a specific interface

    BusInterface(addr_r, addr_r, boolean, class Bus* = 0);
    BusInterface();
    ~BusInterface();

    void set_status(boolean);           // set status (alive/dead);
    boolean get_status();               // get status
    void set_range(addr_r, addr_r);     // change/set address range

    void connect();                     // connect interface to bus
    void disconnect();                  // disconnect interface
    void add_bus(class Bus*);           // specify bus but do not connect yet

    void add_device(Connector*, int);   // add device to interface
    void delete_device();               // remove device

    int read(direction, addr_r);        // send read request
    void write(direction, int, addr_r); // send write request
    data_buffer multi_read(direction, addr_r);

    static class Bus *bs;               // the bus to connect interface to
    static int unique_id;               // id to base identifier on
    int priority;
    boolean multicast;
};
#endif
#ifdef _INIT_
#undef _INIT_
    class Bus *BusInterface::bs = 0;
    int BusInterface::unique_id = 0;
#endif
