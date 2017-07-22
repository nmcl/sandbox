#include <iostream.h>

#include "Port.h"

main()
{
    PortAddress myaddress(4000);
    PortAddress from;

    if (!myaddress.Valid())
    {
	cerr << "Error. Invalid PortAddress\n";
	return 0;
    }

    Port myport(myaddress);
    if (!myport.Valid())
    {
	cerr << "Error. Invalid Port\n";
	return 0;
    }

    Buffer buff;
    Port::Outcome res;
    char* message;

    res = myport.ReceiveMessage(from, buff);
    buff.unpack(message);

    cerr << "Server received " << message << "\n";
}
