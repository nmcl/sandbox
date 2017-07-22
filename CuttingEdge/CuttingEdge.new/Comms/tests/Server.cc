#include <iostream.h>
#include <sysent.h>

#include <ServerRPC.h>

main(int argc, char** argv)
{
    ServerRPC Client;

    if (!Client.Initialise(argc, argv))
    {
	perror("INITIALISE: failed ");
	exit(0);
    }
}
