#define _INIT_
#include "BusInterface.h"
#include "Bus.h"
#include "Connector.h"

BusInterface::BusInterface()
{
    start = 0;
    end = 0;
    active = false;
    identifier = 0;
    multicast = false;
}

BusInterface::BusInterface(addr_r from, addr_r to, boolean status, Bus *given_bus)
{
    start = from;
    end = to;
    active = status;
    unique_id++;
    identifier = unique_id; 
    multicast = false;

    if (given_bus != 0)
	bs = given_bus;
}

BusInterface::~BusInterface()
{
    // do nothing for now
}

void BusInterface::set_range(addr_r from, addr_r to)
{
    start = from;
    end = to;
}

void BusInterface::set_status(boolean status) { active = status; }

boolean BusInterface::get_status() { return(active); }

void BusInterface::add_bus(Bus* given_bus) { bs = given_bus; }

void BusInterface::connect() { bs->connect(this); }

void BusInterface::disconnect() { bs->disconnect(this); }

void BusInterface::add_device(Connector *unit_name, int prty)
{
    device = unit_name;
    priority = prty;
}

int BusInterface::read(direction d, addr_r memory_addr)
{
    boolean ok = false;

    if (d == ON_BUS)
	return(bs->send(memory_addr, READ, 0, this, &ok));
    else
	return(device->read(memory_addr));
}

void BusInterface::write(direction d, int value, addr_r memory_addr)
{
    boolean ok = false;

    if (d == ON_BUS)
	bs->send(memory_addr, WRITE, value, this, &ok);
    else
	device->write(value, memory_addr);
}

data_buffer BusInterface::multi_read(direction d, addr_r adr)
{
    InterfaceList *list = 0;
    data_buffer *head_list = 0, *ptr;
    int value;
    boolean ok;

    do
    {
	value = bs->multi_send(adr, READ, 0, this, &ok, &list);

	if (ok)
	{
	    if (head_list == 0)
	    {
		head_list = new data_buffer;
		ptr = head_list;
	    }
	    else
	    {
		ptr->next_buffer = new data_buffer;
		ptr = ptr->next_buffer;
	    }

	    ptr->next_buffer = 0;
	    ptr->value = value;
	}
    } while (list != 0);

    return(*head_list);
}

		    
