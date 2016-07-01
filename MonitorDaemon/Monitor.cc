#include <stdio.h>
#include <iostream.h>
#include <memory.h>
#include <strstream.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

#ifndef MONITOR_H_
#  include "Monitor.h"
#endif

class MachineList
{
public:
    MachineList ();
    ~MachineList ();

    ostream& print (ostream&) const;
    
    char* machineName;
    int portNumber;
    MachineList* next;
};

MachineList::MachineList ()
			 : machineName(NULL),
			   portNumber(-1),
			   next(NULL)
{
}

MachineList::~MachineList ()
{
    if (machineName)
	::delete [] machineName;
    if (next)
	delete next;
}

ostream& MachineList::print (ostream& strm) const
{
    strm << "Machine: " << machineName << endl;
    strm << "Port: " << portNumber << endl;

    if (next)
	next->print(strm);

    return strm;
}



Monitor::Monitor ()
		 : output(defaultFile, ios::app),
		   error(errorFile, ios::app),
		   fileName(NULL),
		   hostName(NULL),
		   head(NULL),
		   sock(-1),
		   probeTime(PROBE_INTERVAL*60),
		   reduced(FALSE)
{
    configure();
}

Monitor::Monitor (const char* file)
		 : output(file, ios::app),
		   error(errorFile, ios::app),
		   fileName(NULL),
		   hostName(NULL),
		   head(NULL),
		   sock(-1),
		   probeTime(PROBE_INTERVAL*60),
		   reduced(FALSE)		   
{
    configure();
}

Monitor::~Monitor ()
{
    if (fileName)
	::delete [] fileName;
    if (hostName)
	::delete [] hostName;
    if (head)
	delete head;
    if (msgSock >= 0)
	::close(msgSock);
    if (sock >= 0)
	::close(sock);
}

Boolean Monitor::configure ()
{
    /*
     * First set up some variables we will need throughout the
     * lifetime of this run.
     */
    
    fileName = ::new char[::strlen(defaultFile)+1];
    ::strcpy(fileName, defaultFile);

    hostName = ::new char[MAXHOSTNAMELEN+1];
    ::memset(hostName, '\0', MAXHOSTNAMELEN+1);
    ::gethostname(hostName, MAXHOSTNAMELEN);

    /*
     * Output the time at which we start running - assume this is the
     * recovery time for the machine.
     */
    
    timeval currTime;

    if (::gettimeofday(&currTime, NULL) == -1)
    {
	error << "Could not gettimeofday." << endl;
	return FALSE;
    }

    output << "Recovery: " << hostName << " [ " << currTime.tv_sec
	   << ", " << currTime.tv_usec << " ] " << endl;

    /*
     * Now read in the configuration file, which indicates the nodes
     * we need to monitor.
     */
    
    ifstream input(configurationFile);

    if (!input)
    {
	error << "Could not read configuration file " << configurationFile << endl;
	return FALSE;
    }

    MachineList* trail = (MachineList*) 0;

    do
    {
	trail = new MachineList;
	trail->machineName = ::new char[MAXHOSTNAMELEN+1];
	::memset(trail->machineName, '\0', MAXHOSTNAMELEN+1);
	
	input >> trail->machineName >> trail->portNumber;

	if (trail->portNumber != -1)
	{
	    trail->next = head;
	    head = trail;
	}
	else
	    delete trail;
	
    } while (input.peek() != EOF);

#ifdef DEBUG
    head->print(cout);
#endif

    /*
     * Now get the socket to listen for requests on.
     */
    
    sock = ::socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
	return FALSE;
    else
    {
	struct sockaddr_in sockAddr;

	sockAddr.sin_addr.s_addr = INADDR_ANY;
	sockAddr.sin_family      = AF_INET;
	sockAddr.sin_port        = htons(MONITOR_SOCKET);
	if (::bind(sock, (struct sockaddr *) &sockAddr, sizeof(sockAddr)) < 0)
	    return FALSE;
	else
	    ::listen(sock, 5);
    }
    
    return TRUE;
}

Boolean Monitor::monitor (int iterations)
{
    if (!output)
    {
	error << "Could not open output file." << endl;
	return FALSE;
    }

    int index = 0;

    while (index != iterations)
    {
	timeval currTime;

	if (::gettimeofday(&currTime, NULL) == -1)
	{
	    error << "Could not gettimeofday." << endl;
	    return FALSE;
	}

	MachineList* trail = head;

#ifdef DEBUG	
	cout << "monitoring" << endl;
#endif	
	
	while (trail)
	{
	    long timeTaken = 0;

	    output << "Time: [ " << currTime.tv_sec << ", " << currTime.tv_usec << " ] ";
	    output << "< " << trail->machineName << ", ";

	    if (ping(trail->machineName, timeTaken, trail->portNumber))
		output << "ALIVE >" << endl;
	    else
		output << "DEAD >" << endl;

	    trail = trail->next;
	}

	if (!probe(probeTime))
	    return FALSE;

	if (iterations > 0)
	    iterations++;

	if (!reduced)
	{
	    /*
	     * Check to see if we need to reduce the probe interval.
	     */
	
	    struct stat buff;

	    if (::stat(fileName, &buff) == 0)
	    {
		if (buff.st_size >= FILESIZE)
		{
		    probeTime *= 2;
		    reduced = TRUE;
		}
	    }
	}
    }

    return TRUE;
}

Boolean Monitor::probe (int waitTime)
{
    int timeout = ((waitTime > 0) ? waitTime : 3600);
    struct timeval toWait;

    toWait.tv_sec = timeout;
    toWait.tv_usec = 0;

    fd_set inmask;
    int res;

    FD_ZERO(&inmask);
    FD_SET(sock, &inmask);
    
#ifdef hpux
    res = select(FD_SETSIZE, (int *) &inmask, 0, 0, &toWait);
#else
    res = select(FD_SETSIZE, &inmask, 0, 0, &toWait);
#endif

    if (res < 0)
	return FALSE;

    if (res == 0)
	return TRUE;

    /*
     * If we have a message then deal with it and start monitoring
     * again.
     */
    
    struct sockaddr_in from;
    int length = sizeof(struct sockaddr_in);
    int message = -1;

    msgSock = accept(sock, (struct sockaddr*) &from, &length);
    res = recv(msgSock, (char*) &message, sizeof(message), 0);

    if (res < 0)
	return FALSE;

#ifdef DEBUG
    cout << "Received message: " << message << endl;
#endif    
    
    switch (message)
    {
    case TERMINATE_CODE:
	{
	    int reply = OK;
	    
	    send(msgSock, (char*) &reply, sizeof(reply), 0);
	    ::close(msgSock);
	    ::close(sock);
	    
	    exit(0);
	}
	break;
    case TRANSMIT_DATA:
	{
#ifdef DEBUG
	    cout << "Transmitting data." << endl;
#endif	    
	    if (!transmitData())
	    {
		msgSock = -1;
		return FALSE;
	    }
	    else
		msgSock = -1;
	}
	break;
    default:
	return FALSE;
    }

    return TRUE;
}

Boolean Monitor::transmitData () const
{
    struct stat buf;

    if (stat(fileName, &buf) == -1)
	return FALSE;

    int fd = ::open(fileName, O_RDONLY);

    if (fd == -1)
	return FALSE;

    char* data = ::new char[buf.st_size];
    int res = ::read(fd, data, (int) buf.st_size);
    Boolean result = FALSE;

    if (res == buf.st_size)
    {
	if (send(msgSock, (char*) &buf.st_size, sizeof(buf.st_size), 0) == sizeof(buf.st_size))
	{
	    if (send(msgSock, data, (int) buf.st_size, 0) == buf.st_size)
		result = TRUE;
	}
    }

    ::close(fd);
    ::close(msgSock);
    
    ::delete [] data;
    
    return result;
}
