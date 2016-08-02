/*
 * Copyright (C) 1993-1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: common.cc,v 1.32 1996/11/23 09:46:01 nmcl Exp $
 */

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef  SYS_TIME_H_
#  include <System/sys/time.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifdef SYSV

#ifndef SYS_TIMES_H_
#  include <System/sys/times.h>
#endif

#ifndef LIMITS_H_
#  include <System/limits.h>
#endif

#endif

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#ifndef NETINET_IN_H_
#  include <System/netinet/in.h>
#endif

#ifndef SYS_UIO_H_
#  include <System/sys/uio.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef SYS_SOCKET_H_
#  include <System/sys/socket.h>
#endif

#ifndef SYS_SELECT_H_
#  include <System/sys/select.h>
#endif

#ifndef NETDB_H_
#  include <System/netdb.h>
#endif

#ifndef ARPA_INET_H_
#  include <System/arpa/inet.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifdef __linux__
#  ifndef ERRNO_H_
#    include <System/errno.h>
#  endif
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef RPC_H_
#  include "rpc.h"
#endif

#ifndef RAJDOOT_RPCUSER_H_
#  include <RPC/Rajdoot/rpcuser.h>
#endif


static const char RCSid[] = "$Id: common.cc,v 1.32 1996/11/23 09:46:01 nmcl Exp $";


#ifdef SYSV
double clockTick = (1.0/sysconf(_SC_CLK_TCK));
#endif

int _rpcerr;		    /* Error diagnostic variable set by rpc mechanism */
struct RPC_SN  h_lastrpcsn; /* Sequence number of last received call
			       for use from server routines */


Uint32 get_machine_id ()
{
    struct hostent *a;
    struct in_addr b;
    char name[MAXHOSTNAMELEN+1];
    static Uint32 toReturn = 0;

    if (toReturn == 0)
    {
	::memset(name, '\0', MAXHOSTNAMELEN+1);
	gethostname(name, MAXHOSTNAMELEN);
	a = gethostbyname(name);
	::memcpy((caddr_t) &b, a->h_addr, a->h_length);
	toReturn = (Uint32) b.s_addr;
    }
    
    return toReturn;
}

#ifdef NEED_SENDMSG

extern "C"
{

int sendmsg (SOCKET s, const struct msghdr* msg, unsigned int flags)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "Within sendmsg" << endl;
    debug_stream.unlock();
#endif

  int len = 0, rcode = 0, bufPtr = 0;

  // calculate size of packet
  
  for (int j = 0; j < msg->msg_iovlen; j++)
    len += msg->msg_iov[j].iov_len;

  char* buf = ::new char[len];

  for (int i = 0; i < msg->msg_iovlen; i++)
  {
      if (msg->msg_iov[i].iov_len > 0)
      {
	  ::memcpy(buf+bufPtr, msg->msg_iov[i].iov_base, msg->msg_iov[i].iov_len);
	  bufPtr += msg->msg_iov[i].iov_len;
      }
  }

  if (msg->msg_accrightslen > 0)
  {
    ::memcpy(buf+bufPtr, (char*) msg->msg_accrights, msg->msg_accrightslen);
    bufPtr += msg->msg_accrightslen;
  }

  rcode = sendto(s, buf, len, flags, (struct sockaddr *) msg->msg_name, msg->msg_namelen);

#ifndef __GNUG__
  ::delete [] buf;
#else
  ::delete buf;
#endif

#ifdef DEBUG
  debug_stream.lock();
    
  debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
  debug_stream << "returning " << rcode << endl;
  debug_stream.unlock();  
#endif

  return rcode;
}

int recvmsg (SOCKET s, struct msghdr* msg, unsigned int flags)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "Within recvmsg" << endl;
    debug_stream.unlock();    
#endif

  int len = 0, rcode, i;
  int bufPtr = 0, sizeLeft = 0;

  len = msg->msg_accrightslen;

  for (i = 0; i < msg->msg_iovlen; i++)
      len += msg->msg_iov[i].iov_len;

  char* recBuf = ::new char[len];

  // attempt to read data
  rcode = recvfrom(s, recBuf, len, flags, (struct sockaddr*) msg->msg_name, &msg->msg_namelen);

  if (rcode <= 0)
  {
#ifndef __GNUG__
      ::delete [] recBuf;
#else
      ::delete recBuf;
#endif    
      return rcode;
  }

  sizeLeft = rcode;

  for (i = 0; (i < msg->msg_iovlen) && (sizeLeft); i++)
  {
      int copySize = msg->msg_iov[i].iov_len;

      if (bufPtr+copySize > rcode)
	  copySize = rcode-bufPtr;

      ::memcpy(msg->msg_iov[i].iov_base, recBuf+bufPtr, copySize);
      bufPtr += copySize;
      sizeLeft -= copySize;
  }

  if ((msg->msg_accrightslen > 0) && (sizeLeft == msg->msg_accrightslen))
  {
      ::memcpy(msg->msg_accrights, recBuf+bufPtr, msg->msg_accrightslen);
      bufPtr += msg->msg_accrightslen;
  }
  else
  {
      if ((msg->msg_accrightslen > 0) || (sizeLeft > 0))
      {
	  errno = EFAULT;
	  rcode = -1;
      }
  }

#ifndef __GNUG__
  ::delete [] recBuf;
#else
  ::delete recBuf;
#endif

  return rcode;
}

};

#endif

/*
 * The following routines are used to "ping" a given machine to determine
 * whether it is available. They return FALSE if a failure is detected.
 * If during the setup of these routines any error is encountered
 * (e.g., cannot setup the socket) then we return TRUE to allow our
 * timeout and retries at the RPC level to continue and at least give
 * us some measure of protection.
 */

Boolean ping (Int32 logHostName, Uint32& timeTaken, Boolean forcePing)
{
#ifdef DEBUG
    debug_stream.lock();
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "Boolean ping ( " << logHostName << " )" << endl;
    debug_stream.unlock();
#endif

    struct host_address hostAddr;
    Boolean result = TRUE;

    hostAddr.log_host = logHostName;
    hostAddr.real_host = (char*) 0;

    if (getrealhost(&hostAddr) == SUCCEEDED)
	result = ping(hostAddr.real_host, timeTaken, forcePing);
    else
	error_stream << "ping: could not resolve host name." << endl;

    if (hostAddr.real_host)
#ifndef GCC_DELETE_BUG
	delete [] hostAddr.real_host;
#else
    	delete hostAddr.real_host;
#endif
    
    return result;
}
    
Boolean ping (const char* hostName, Uint32& timeTaken, Boolean forcePing)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "Boolean ping ( " << hostName << " )" << endl;
    debug_stream.unlock();
#endif
    
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
    debug_stream.lock();
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ping: my host: " << myHostName << endl;
    debug_stream.unlock();
#endif	

    cout << myHostName << " pinging " << hostName << endl;
    
    if ((::strcmp(myHostName, hostName) == 0) && (!forcePing))  // can always ping own machine!
    {
#ifdef DEBUG
	debug_stream.lock();
	debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
	debug_stream << "ping: being asked to ping own machine. Returning TRUE." << endl;
	debug_stream.unlock();
#endif	
	return TRUE;
    }

    /*
     * Since ping should be used only in exceptional circumstances,
     * we can afford to create a new socket each time. This helps
     * get round the problem of outstanding ping replies from earlier
     * attempts.
     */
    
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    
    if (sock >= 0)
    {
	hp = gethostbyname((char *)hostName);

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
	    sockAddr.sin_port = htons(PING_SOCKET);

	    do
	    {
		if ((sendto(sock, (char*) &startTime, sizeof(startTime), 0,
			   (struct sockaddr*) &sockAddr, sizeof(struct sockaddr_in)) == sizeof(startTime)) ||
		    (ignoreSend))
		{
		    struct timeval waitTime;
		    fd_set inmask;
		    int res;

		    ignoreSend = FALSE;

		    /*
		     * We should really keep track of how much of this time
		     * is used by select. This can be useful if we receive an
		     * "old" reply.
		     */
		    
		    waitTime.tv_sec = (int) (PING_WAIT/1000);
		    waitTime.tv_usec = (PING_WAIT - waitTime.tv_sec*1000)*1000;
		
		    FD_ZERO(&inmask);
		    FD_SET(sock, &inmask);
		    
#ifdef hpux
		    res = select(FD_SETSIZE, (int *) &inmask, 0, 0, &waitTime);
#else
		    res = select(FD_SETSIZE, &inmask, 0, 0, &waitTime);
#endif

		    if (res > 0)
		    {
			struct sockaddr_in from;
			int length = 0;

#ifndef SYSV			
			receivedTime.tv_sec = receivedTime.tv_usec = 0;
#endif			

			if (recvfrom(sock, (char*) &receivedTime, sizeof(receivedTime), 0, (struct sockaddr*) &from, &length) >= 0)
			{
#ifdef SYSV
			    if (receivedTime == startTime)
			    {
				currentTime = times(&Clockbuffer) - startTime;
				responseTime = currentTime * clockTick * 1000000;
#ifdef DEBUG
				debug_stream.lock();
				

				debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
				debug_stream << "ping: time to get reply " << responseTime << " microseconds." << endl;
				debug_stream.unlock();
				
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
				debug_stream.lock();
				

				debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
				debug_stream << "ping: time to get reply " << responseTime << " microseconds." << endl;
				debug_stream.unlock();
				
#endif
				result = TRUE;
			    }
#endif
			    else
			    {
#ifdef DEBUG
				debug_stream.lock();
				
				debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
				debug_stream << "ping: received invalid response." << endl;
				debug_stream.unlock();
				
#endif
				ignoreSend = TRUE;  // don't resend to try to cut down on "old" messages
			    }
			}
			else
			    error_stream << WARNING << "ping: receive problem. Errno " << errno << endl;
		    }
		    else
		    {
#ifdef DEBUG
			debug_stream.lock();
			
			debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
			debug_stream << "ping: did not receive response from machine " << hostName << endl;
			debug_stream.unlock();
#endif
			result = FALSE;
		    }
		}
		else
		    error_stream << WARNING << "ping: send problem. Errno " << errno << endl;
		
	    } while ((!result) && (--retry > 0));
	}
	else
	    error_stream << WARNING << "ping: could not get host information for " << hostName << ". Errno " << errno << endl;
    }
    else
	error_stream << WARNING << "ping: could not get socket. Errno " << errno << endl;

#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_BASIC_RPC << VIS_PUBLIC;
    debug_stream << "ping: returning " << ((result) ? "TRUE" : "FALSE") << endl;
    debug_stream.unlock();
    
#endif

#ifdef SYSV
    currentTime = times(&Clockbuffer) - startTime;
    timeTaken = (Uint32) (currentTime * clockTick * 1000000);
#else
    gettimeofday(&currentTime, 0);
    timeTaken = (Uint32) (((currentTime.tv_sec * 1000000) + currentTime.tv_usec) - ((startTime.tv_sec * 1000000) + startTime.tv_usec));
#endif

    close(sock);

#ifdef DEBUG
    if (!result)
    	error_stream << WARNING << "ping: failed for machine " << hostName << endl;
#endif
    
    return result;
}
