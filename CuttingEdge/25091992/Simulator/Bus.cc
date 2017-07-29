#include "BusInterface.h"
#include "Bus.h"

Bus::Bus()
{
    head = 0;
    inlst = 0;
}

Bus::~Bus()
{
    // do nothing yet
}

void Bus::connect(BusInterface *interface)
{
    if (head == 0)
    {
	inlst = new InterfaceList;
	head = inlst;
    }
    else
    {
	inlst->next_int = new InterfaceList;
	inlst = inlst->next_int;
    }

    inlst->next_int = 0;
    inlst->this_int = interface;
}

void Bus::disconnect(BusInterface *interface)
{
    InterfaceList *ptr = head, *trail;

    while (ptr->this_int->identifier != interface->identifier)
    {
	trail = ptr;
	ptr = ptr->next_int;
    }

    if (ptr->next_int != 0)
	trail->next_int = ptr->next_int;

    delete(ptr);
}

int Bus::send(addr_r send_to, opcode operation, int buffer, BusInterface *from, boolean *ok)
{
    InterfaceList *ptr = head;
    int prty = -1;

    while (ptr != 0)
    {
	if ((ptr->this_int->priority > prty) && (ptr->this_int->identifier != from->identifier))
	    prty = ptr->this_int->priority;
	ptr = ptr->next_int;
    }

    *ok = false;
    if (prty != -1)
    {
	ptr = head;
	while (ptr != 0)
	{
	    if ((send_to >= ptr->this_int->start) &&
		(send_to <= ptr->this_int->end))
	    {
		if ((operation == READ) && (ptr->this_int->priority == prty) &&
		    (ptr->this_int->identifier != from->identifier))
		{
		    *ok = true;
		    return(ptr->this_int->read(OFF_BUS, send_to));
		}
		else if ((operation == WRITE) && (ptr->this_int->identifier != from->identifier))
		{
		    *ok = true;
		    ptr->this_int->write(OFF_BUS, buffer, send_to);
		}
	    }
	    ptr = ptr->next_int;
	}
    }

    return(true);
}

int Bus::multi_send(addr_r send_to, opcode operation, int buffer, BusInterface *from, boolean *ok, InterfaceList **ptr)
{
    int value;

    if (*ptr == 0)
	*ptr = head;

    *ok = false;
    while (ptr != 0)
    {
	if ((send_to >= (*ptr)->this_int->start) &&
	    (send_to <= (*ptr)->this_int->end))
	{
	    if ((operation == READ) && ((*ptr)->this_int->identifier != from->identifier))
	    {
		*ok = true;

		value = (*ptr)->this_int->read(OFF_BUS, send_to);
		(*ptr) = (*ptr)->next_int;
		return(value);
	    }
	    else if ((operation == WRITE) && ((*ptr)->this_int->identifier != from->identifier))
	    {
		*ok = true;
		(*ptr)->this_int->write(OFF_BUS, buffer, send_to);
	    }
	}
	(*ptr) = (*ptr)->next_int;
    }

    return(true);
}
