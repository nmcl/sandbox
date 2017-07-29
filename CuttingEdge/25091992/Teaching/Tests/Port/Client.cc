#include <iostream.h>

#include "Port.h"

main()
{
    PortAddress serveraddress(4000);
    if (!serveraddress.Valid())
    {
	cerr << "Error. Invalid server address\n";
	return 0;
    }

    Port myport;
    if (!myport.Valid())
    {
	cerr << "Error. Invalid Port\n";
	return 0;
    }

    Buffer buff;
    Port::Outcome res;
    const char* message = "Hello World";

    buff.pack(message);
    res = myport.SendMessage(serveraddress, buff);
}
