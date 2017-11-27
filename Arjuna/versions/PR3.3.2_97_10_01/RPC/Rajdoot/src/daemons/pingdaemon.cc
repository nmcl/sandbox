/*
 * Copyright (C) 1994
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: pingdaemon.cc,v 1.14.6.1 1996/10/10 12:26:15 ngdp Exp $
 */

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef STDIO_H_
#  include <System/stdio.h>
#endif

#ifndef SIGNAL_H_
#  include <System/signal.h>
#endif

#ifndef  SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifdef SYSV

#ifndef SYS_TIMES_H_
#  include <System/sys/times.h>
#endif

#ifndef LIMITS_H_
#  include <System/limits.h>
#endif

#endif

#ifndef UNISTD_H_
#   include <System/unistd.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef NETBD_H_
#  include <System/netdb.h>
#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef FSTREAM_H_
#   include <System/fstream.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef PING_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif


int main (int argc, char** argv)
{
    struct sockaddr_in sockAddr;
    char outputFile[OBJECT_NAME_SIZE], errorFile[OBJECT_NAME_SIZE];
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
    
    sprintf(outputFile, "pingdaemon_out_%d", daemonPid);
    sprintf(errorFile, "pingdaemon_err_%d", daemonPid);

    ofstream target1(outputFile);
    if (!target1)
	printf("pingdaemon: Cannot open file %s\n", outputFile);
    else
    {
	if (debugOn)
	    cout = target1;
	else
	{
	    target1.close();
	    ::unlink(outputFile);
	}
    }
    
    ofstream target2(errorFile);
    if (!target2)
	printf("pingdaemon: Cannot open file %s\n", errorFile);
    else
    {
	if (debugOn)
	    cerr = target2;
	else
	{
	    target2.close();
	    ::unlink(errorFile);
	}
    }

    if (pingSelf)
    {
	char host[MAXHOSTNAMELEN+1];
	Uint32 dummy = 0;
	
	::memset(host, '\0', MAXHOSTNAMELEN+1);
	::gethostname(host, MAXHOSTNAMELEN);

	if (hostName == (char*) 0)
	    hostName = host;
	
	if (ping(hostName, dummy, TRUE))  // force a ping to occur, even if for own machine
        {
            cout << "Successfully pinged host " << hostName << "." << endl;
            return 0;
        }
        else
            cout << "No ping response from host " << hostName << "." << endl;

        if (::strcmp(hostName, host) != 0)
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
		int length = sizeof(struct sockaddr), res;

		res = recvfrom(s, (char*) &receivedTime, sizeof(receivedTime), 0, (struct sockaddr*) &from, &length);

		if (res >= 0)
		{
#ifdef DEBUG
		    debug_stream.lock();
		    
		    struct hostent* hp = gethostbyaddr((char*) &from.sin_addr, sizeof(from.sin_addr), AF_INET);

		    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;

		    if (hp)
			debug_stream << "pingdaemon: received message from machine " << hp->h_name << endl;
		    else
			debug_stream << "pingdaemon: gethostbyaddr error." << endl;
		    
		    debug_stream.unlock();
		    
#endif
		    if (sendto(s, (char*) &receivedTime, res, 0, (struct sockaddr*) &from, sizeof(from)) < 0)
			error_stream << WARNING << "pingdaemon: could not reply to inquiry. Errno " << errno << endl;
#ifdef DEBUG
		    else
		    {
			debug_stream.lock();
			debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
			debug_stream << "pingdaemon: sent reply." << endl;
			debug_stream.unlock();
		    }
#endif
		    // check if this is a terminate message.
#ifdef SYSV
		    if (receivedTime == TERMINATE_CODE)
		    {
			close(s);

#ifndef hpux
			return 0;
#else
			kill(daemonPid, SIGKILL);
#endif			
		    }
#else
		    if ((receivedTime.tv_sec == TERMINATE_CODE) && (receivedTime.tv_usec == TERMINATE_CODE))
		    {
			close(s);
			return 0;
		    }
#endif
		}
		else
		    error_stream << WARNING << "pingdaemon: error in receiving. Errno " << errno << endl;
 	    }
	}
	else
	    error_stream << FATAL << "pingdaemon: could not bind." << endl;
    }
    else
	error_stream << FATAL << "pingdaemon: could not get socket." << endl;

    return -1;
}

	    
		
