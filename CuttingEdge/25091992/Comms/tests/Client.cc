#include <sysent.h>
#include <iostream.h>

#include <ClientRPC.h>

main()
{
    ClientRPC Service("Server");

    if (Service.Initiate() != Oper_Done)
    {
	perror("INITIATE: failed ");
	exit(0);
    }
    else
	cerr << "Got reply from server\n";
}

