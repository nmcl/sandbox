/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: common.cc,v 1.19 1995/03/09 12:35:09 ngdp Exp $
 */

#include <iostream>
#include <string.h>
#include <sys/time.h>
#include <unistd.h>

#ifdef SYSV

#include <sys/times.h>
#include <limits.h>

#endif

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/uio.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <memory.h>
#include <errno.h>
#include <stdlib.h>

#ifndef COMMON_H_
#  include "Common.h"
#endif

#ifdef SYSV
double clockTick = (1.0/sysconf(_SC_CLK_TCK));
#endif

using namespace std;

/*
 * The following routines are used to "ping" a given machine to determine
 * whether it is available. They return FALSE if a failure is detected.
 * If during the setup of these routines any error is encountered
 * (e.g., cannot setup the socket) then we return TRUE to allow our
 * timeout and retries at the RPC level to continue and at least give
 * us some measure of protection.
 */

Boolean ping (const char* hostName, long& timeTaken, int portNumber)
{
#ifdef DEBUG
    cout << "Boolean ping ( " << hostName << " )" << endl;
#endif
    
    static int sock = -1;
    static char* myHostName = (char*) 0;
    struct sockaddr_in sockAddr;
    struct hostent* hp = 0;
    Boolean result = TRUE;
    int retry = PING_RETRY;
#ifdef SYSV
    clock_t currentTime = 0, startTime = 0, receivedTime = 0;
    struct tms Clockbuffer;
#else    
    struct timeval currentTime, startTime, receivedTime;
#endif
    long responseTime = 0;

    if (sock == -1)
	sock = socket(AF_INET, SOCK_DGRAM, 0);

    timeTaken = 0;
    
    if (myHostName == (char*) 0)
    {
	myHostName = ::new char[MAXHOSTNAMELEN+1];
	::memset(myHostName, '\0', MAXHOSTNAMELEN+1);
	::gethostname(myHostName, MAXHOSTNAMELEN);
    }

    // check cached name is in same format as name given
    
    if (::strstr(hostName, ".") != 0)  // name in dot format
    {
	if (::strstr(myHostName, ".") == 0)
	{
	    hp = gethostbyname(myHostName);
	    ::strcpy(myHostName, hp->h_name);
	}
    }
    else
    {
	if (::strstr(myHostName, ".") != 0)
	{
	    ::memset(myHostName, '\0', MAXHOSTNAMELEN+1);
	    ::gethostname(myHostName, MAXHOSTNAMELEN);
	}
    }

#ifdef DEBUG
    cout << "ping: my host: " << myHostName << endl;
#endif	

    if (::strcmp(myHostName, hostName) == 0)  // can always ping own machine!
    {
#ifdef DEBUG
      cout << "ping: being asked to ping own machine. Returning TRUE." << endl;
#endif	
	return TRUE;
    }
    
    if (sock >= 0)
    {
	hp = gethostbyname((char*) hostName);

	if (hp)
	{
	    Boolean ignoreSend = FALSE; // if we receive an "old" message then we will not resend to avoid flooding
#ifdef SYSV
	    startTime = times(&Clockbuffer);
#else    
	    gettimeofday(&startTime, 0);
#endif
	    ::memset((void *) &sockAddr, '\0', sizeof(struct sockaddr_in));
	    ::memcpy((char *) &sockAddr.sin_addr, (char *) hp->h_addr, hp->h_length);
	    sockAddr.sin_family = AF_INET;
	    sockAddr.sin_port = htons(portNumber);

	    do
	    {
		if ((sendto(sock, (char*) &startTime, sizeof(startTime), 0,
			   (struct sockaddr*) &sockAddr, sizeof(struct sockaddr_in)) == sizeof(startTime)) ||
		    (ignoreSend))
		{
		    struct timeval waitTime;
		    fd_set inmask;
		    int res;
		    
		    waitTime.tv_sec = (int) (PING_WAIT/1000);
		    waitTime.tv_usec = (PING_WAIT - waitTime.tv_sec*1000)*1000;
		
		    FD_ZERO(&inmask);
		    FD_SET(sock, &inmask);
#ifdef hpux
		    res = select(FD_SETSIZE, (int *) &inmask, 0, 0, &waitTime);
#else
		    res = select(FD_SETSIZE, &inmask, 0, 0, &waitTime);
#endif
		    ignoreSend = FALSE;
		    
		    if (res > 0)
		    {
			struct sockaddr_in from;
			socklen_t length = 0;

			if (recvfrom(sock, (char*) &receivedTime, sizeof(receivedTime), 0, (struct sockaddr*) &from, &length) >= 0)
			{
#ifdef SYSV
			    if (receivedTime == startTime)
			    {
				currentTime = times(&Clockbuffer) - startTime;
				responseTime = currentTime * clockTick * 1000000;
#ifdef DEBUG				

				cout << "ping: time to get reply " << responseTime << " microseconds." << endl;
#endif
				result = TRUE;
			    }
#else
			    if ((receivedTime.tv_sec == startTime.tv_sec) && (receivedTime.tv_usec == startTime.tv_usec))
			    {
				gettimeofday(&currentTime, 0);
				responseTime = (((currentTime.tv_sec * 1000000) + currentTime.tv_usec) -
						((startTime.tv_sec * 1000000) + startTime.tv_usec));
#ifdef DEBUG				

				cout << "ping: time to get reply " << responseTime << " microseconds." << endl;
#endif
				result = TRUE;
			    }
#endif
			    else
			    {
#ifdef DEBUG			    
				cout << "ping: received invalid response." << endl;
#endif
				retry++;  // this is probably from an earlier attempt, so ignore it.
				ignoreSend = TRUE;  // don't resend to try to cut down on "old" messages
			    }
			}
			else
			    cerr << "ping: receive problem. Errno " << errno << endl;
		    }
		    else
		    {
#ifdef DEBUG
			cout << "ping: did not receive response from machine " << hostName << endl;
#endif
			result = FALSE;
		    }
		}
		else
		    cerr << "ping: send problem. Errno " << errno << endl;
		
	    } while ((!result) && (--retry > 0));
	}
	else
	    cerr << "ping: could not get host information for " << hostName << ". Errno " << errno << endl;
    }
    else
	cerr << "ping: could not get socket. Errno " << errno << endl;

#ifdef DEBUG
    cout << "ping: returning " << ((result) ? "TRUE" : "FALSE") << endl;
#endif

#ifdef SYSV
    currentTime = times(&Clockbuffer) - startTime;
    timeTaken = currentTime * clockTick * 1000000;
#else
    gettimeofday(&currentTime, 0);
    timeTaken = (((currentTime.tv_sec * 1000000) + currentTime.tv_usec) - ((startTime.tv_sec * 1000000) + startTime.tv_usec));
#endif
    
    return result;
}
