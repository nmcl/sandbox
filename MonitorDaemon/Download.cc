#include <string.h>
#include <stdio.h>
#include <iostream>
#include <memory.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/param.h>

#ifndef COMMON_H_
#  include "Common.h"
#endif

using namespace std;

int main (int argc, char** argv)
{
    char* hostName = (char*) 0;
    int port = MONITOR_SOCKET;
    int command = TRANSMIT_DATA;
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " -host <host name> [-port <port number>] [-help]" << endl;
	    return 0;
	}
	if (::strcmp(argv[i], "-host") == 0)
	    hostName = argv[i+1];
	if (::strcmp(argv[i], "-port") == 0)
	    port = atoi(argv[i+1]);
    }

    if (!hostName)
    {
	cerr << "Error. No host name given." << endl;
	return -1;
    }

    struct hostent *hp = gethostbyname(hostName);

    if (hp == NULL)
    {
	cerr << "Error. Cannot resolve host " << hostName << endl;
	return -2;
    }

    struct sockaddr_in sockAddr;

    ::memset((void *) &sockAddr, '\0', sizeof(struct sockaddr_in));
    ::memcpy((char *) &sockAddr.sin_addr, (char *) hp->h_addr, hp->h_length);
    sockAddr.sin_family = AF_INET;
    sockAddr.sin_port = htons(port);
    
    int length = sizeof(struct sockaddr_in);
    int sock = ::socket(AF_INET, SOCK_STREAM, 0);

    if (::connect(sock, (struct sockaddr*) &sockAddr, length) == 0)
    {
	if (send(sock, (char*) &command, sizeof(command), 0) == sizeof(command))
	{
	    off_t msgSize = 0;
	    
	    if (recv(sock, (char*) &msgSize, sizeof(msgSize), 0) == sizeof(msgSize))
	    {
		char* msg = ::new char[msgSize+1];
		char* ptr = msg;
		int count = 0;

		::memset(msg, '\0', (int) (msgSize+1));
		
		while (count < msgSize)
		{
		    int r = recv(sock, ptr, (int) (msgSize - count), 0);

		    if (r > 0)
		    {
			count += r;
			ptr += r;
		    }
		    else
		    {
			cerr << "Error when receiving data." << endl;
			break;
		    }
		}

		if (count == msgSize)
		    cout << msg << endl;
		
		::delete [] msg;
	    }
	    else
		cerr << "Error when receiving setup information." << endl;
	}
	else
	    cerr << "Error. Could not send message." << endl;

	::close(sock);
    }
    else
	cerr << "Error. Could not connect to monitor." << endl;

    return 0;
}

	
		    
