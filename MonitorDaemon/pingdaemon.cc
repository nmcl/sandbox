/*
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: pingdaemon.cc,v 1.10 1994/12/15 12:12:48 nmcl Exp $
 */


#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef SYSV

#include <sys/times.h>
#include <limits.h>

#endif

#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/param.h>
#include <string.h>
#include <iostream>
#include <fstream>

#ifndef COMMON_H_
#  include "Common.h"
#endif

using namespace std;

int main (int argc, char** argv)
{
    struct sockaddr_in sockAddr;
    char outputFileName[OBJECT_NAME_SIZE], errorFileName[OBJECT_NAME_SIZE];
#ifndef hpux
    int daemonPid = getpid();
#else
    pid_t daemonPid = getpid();
#endif
#ifdef SYSV
    clock_t receivedTime = 0;
#else    
    struct timeval receivedTime;
#endif    
    int s = socket(AF_INET, SOCK_DGRAM, 0);
    Boolean debugOn = FALSE, pingSelf = FALSE;
    char* hostName = (char*) 0;
    
    if (chdir("/tmp"))
	cerr << "pingdaemon: failed to chdir to /tmp" << endl;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-d") == 0)
	    debugOn = TRUE;
	if (::strcmp(argv[i], "-ping") == 0)
	    pingSelf = TRUE;
	if (::strcmp(argv[i], "-host") == 0)
	    hostName = argv[i+1];
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: pingdaemon [-d] [-ping [-host] ] [-help]" << endl;
	    return 0;
	}
    }
    
    sprintf(outputFileName, "pingdaemon_out_%d", daemonPid);
    sprintf(errorFileName, "pingdaemon_err_%d", daemonPid);

    std::ofstream target1(outputFileName);
    std::ofstream target2(errorFileName);

    if (!target1)
	printf("pingdaemon: Cannot open file %s\n", outputFileName);
    else
    {
	if (debugOn)
	  std::cout.rdbuf(target1.rdbuf());
	else
	{
	    target1.close();
	    ::unlink(outputFileName);
	}
    }
	
    if (!target2)
	printf("pingdaemon: Cannot open file %s\n", errorFileName);
    else
    {
	if (debugOn)
	  std::cerr.rdbuf(target2.rdbuf());
	else
	{
	    target2.close();
	    ::unlink(errorFileName);
	}
    }

    if (pingSelf)
    {
	char host[MAXHOSTNAMELEN+1];
	long dummy = 0;
	
	::memset(host, '\0', MAXHOSTNAMELEN+1);
	::gethostname(host, MAXHOSTNAMELEN);

	if (hostName == (char*) 0)
	    hostName = host;

	if (ping(hostName, dummy))
	{
	    cout << "Host " << hostName << " already has a pingdaemon running." << endl;
	    return 0;
	}
	else
	    cout << "No pingdaemon running on host " << hostName << "." << endl;

	if (::strcmp(hostName, host) == 0)
	    cout << "Starting pingdaemon." << endl;
	else
	    return 1;
    }
	
    if (s >= 0)
    {
	sockAddr.sin_addr.s_addr = INADDR_ANY;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(PING_SOCKET);

	if (bind(s, (struct sockaddr*) &sockAddr, sizeof(sockAddr)) >= 0)
	{
	    for (;;)
	    {
		struct sockaddr_in from;
		socklen_t length = sizeof(struct sockaddr);
		ssize_t res;
		
		res = recvfrom(s, (char*) &receivedTime, sizeof(receivedTime), 0, (struct sockaddr*) &from, &length);
		
		if (res >= 0)
		{
#ifdef DEBUG
		    struct hostent* hp = gethostbyaddr((char*) &from.sin_addr, sizeof(from.sin_addr), AF_INET);

		    if (hp)
			cout << "pingdaemon: received message from machine " << hp->h_name << endl;
		    else
			cout << "pingdaemon: gethostbyaddr error." << endl;
		    
#endif			
		    if (sendto(s, (char*) &receivedTime, res, 0, (struct sockaddr*) &from, sizeof(from)) < 0)
			cerr << "pingdaemon: could not reply to inquiry. Errno " << errno << endl;
#ifdef DEBUG
		    else
			cout << "pingdaemon: sent reply." << endl;
#endif
		    // check if this is a terminate message.
#ifdef SYSV
		    if (receivedTime == TERMINATE_CODE)
			return 0;
#else
		    if ((receivedTime.tv_sec == TERMINATE_CODE) && (receivedTime.tv_usec == TERMINATE_CODE))
			return 0;
#endif
		}
		else
		    cerr << "pingdaemon: error in receiving. Errno " << errno << endl;
 	    }
	}
	else
	    cerr << "pingdaemon: could not bind." << endl;
    }
    else
	cerr << "pingdaemon: could not get socket." << endl;

    return -1;
}

	    
		
