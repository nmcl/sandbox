#include "Bus.h"
#include "BusInterface.h"
#include "Ram.h"
#include "Rom.h"
#include "Processor.h"
#include "MultiConnector.h"
#include <iostream.h>

main()
{
    Bus *bs = new Bus();
    BusInterface *if1 = new BusInterface(1, 1000, true, bs);
    BusInterface *if2 = new BusInterface(2001, 3000, true, bs);
    BusInterface *if3 = new BusInterface(1, 1000, true, bs);

    Ram *memory1 = new Ram(if1, 0);
    Ram *memory2 = new Ram(if3, 0);
    MultiConnector *cpu = new MultiConnector(if2, 1);

    if1->connect();
    if2->connect();
    if3->connect();

    memory1->write(4, 4);
    memory2->write(2, 4);
    cerr << "Ram memory 4 = " << memory1->read(4) << "\n";
    cerr << "Reading 2, got " << cpu->read(2) << "\n";

    data_buffer d = cpu->multi_read(4);
    cerr << "Reading 4, got " << d.value << "\n";
    cerr << "Reading 4, got " << d.next_buffer->value << "\n";
}
