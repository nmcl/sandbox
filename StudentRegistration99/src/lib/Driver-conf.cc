#include <System/sys/param.h>
#include <System/stdio.h>
#include <System/signal.h>
#include <System/sys/types.h>
#include <System/netinet/in.h>
#include <System/sys/socket.h>
#include <System/netdb.h>
#include <System/sys/time.h>
#include <System/stdlib.h>

extern int tcpl, fromlen;
extern struct sockaddr_in from;

void ConfigNet( int Port )
{
	struct sockaddr_in to;
	struct in_addr nulladdr;
	int one = 1;

	nulladdr.s_addr = 0;

	if ((tcpl = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror ("TCP socket");
		exit (1);
	}

	if( setsockopt( tcpl, SOL_SOCKET, SO_KEEPALIVE, (char*) &one, sizeof( one )) < 0 )
	{
		perror( "Setsockopt" );
		exit( 1 );
	}

	to.sin_addr.s_addr = htonl ( INADDR_ANY );
        to.sin_port = htons ( Port );
        to.sin_family = AF_INET;

        if (bind (tcpl, (struct sockaddr *)&to, sizeof (to)) < 0)
        {
                perror ("TCP Bind failed.");
                exit( 2 );
        }

	if( listen( tcpl, 5 ) == -1 )
	{
		perror( "Failed to set the number of connections queued" );
		exit( 1 );
	}

	from.sin_family = AF_INET;
	fromlen = MAXHOSTNAMELEN;
}

