/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: phone.cc,v 1.4 1993/03/18 15:43:19 n048z Exp $
 *
 */

#ifndef SYS_TYPES_H_
#include <System/sys/types.h>
#endif

#ifndef SYS_SOCKET_H_
#include <System/sys/socket.h>
#endif

#ifndef STDLIB_H_
#include <System/stdlib.h>
#endif

#ifndef OSFCN_H_
#include <System/osfcn.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef NETINET_IN_H_
#include <System/netinet/in.h>
#endif

#ifndef NETDB_H_
#include <System/netdb.h>
#endif

#ifndef MEMORY_H_
#include <System/memory.h>
#endif

#include "defs.h"
#include "connection.h"
#include "phone.h"

#if defined(DEBUG) && !defined(DEBUG_H_)
#include <Common/Debug.h>
#endif

/***
 * Set up a stream socket for communication, either incoming or
 * outgoing.  The abstraction is that this a telephone: i.e. an object
 * which can make or receive calls, and a call is a circuit connecting
 * two parties.
 *
 * To receive calls the constructor must be given the port number to
 * accept calls on and a size for the listen queue backlog. If these
 * are ommitted then the bind() and listen() won't be done and calls
 * can be made but not received.
 *
 * This and all operations in this file uses byte-stream protocol.
 */

phone::phone(int port, int bcklg)
{
	struct sockaddr_in sa;
	char * errsig = "phone::phone";

	sd = portno = backlog = -1;

	sd = socket(AF_INET, SOCK_STREAM, 0);

	if (sd == -1)
		perror(errsig);


		/* If the caller supplied a port then bind
		 * it to the socket.
		 */
	if (port != -1)
	{
	        memset((char *)&sa, '\0', sizeof(sa));
		sa.sin_family = AF_INET;
		sa.sin_port = htons(port);
		sa.sin_addr.s_addr = htonl(INADDR_ANY);

		if (bind(sd,(struct sockaddr *)&sa,sizeof(sa)) == -1)
			perror(errsig);
		else
			portno = port;
	}


		/* If the caller supplied a listen backlog then
		 * set it.
		 */
	if (bcklg != -1)
	{
	    if (listen(sd, bcklg) == -1)
		perror(errsig);
	    else
		backlog = bcklg;
	}
}



phone::~phone()
{
	if (sd != -1)
		close(sd);
}



/***
 * Answer a phone-- wait for a caller and return a connection to
 * him when he calls.
 *
 * uses byte stream protocol
 */

connection * phone::answer()
{
	struct sockaddr_in sa;
	struct hostent *hp = (struct hostent *)null;
	int salen = sizeof(sa);
	char * errsig = "phone::answer";


	if (sd != -1 && portno != -1 && backlog != -1)
	{
		struct sockaddr * psa = (struct sockaddr *)&sa;
		int sdnew = accept(sd, psa, &salen);

		if (sdnew >= 0)
		{
			int callerport;
			char * callername;

			hp = gethostbyaddr((char *)&sa.sin_addr,
					sizeof(struct in_addr),
					sa.sin_family);

			if (hp != null)
				callername = hp->h_name;
			else
				callername = "?";

			callerport = (int)ntohs(sa.sin_port);

			return (new connection(sdnew, callername, callerport));
		}
		else
			perror(errsig);
	}

	return ((connection *)null);
}



/***
 * Make a call on a phone-- given a destination machine and port
 * establish a connection with the server listening.
 *
 * uses byte stream protocol
 */

connection * phone::call(char * dstname, int dstport)
{
	struct sockaddr_in sa;
	struct hostent *hp;
	char * errsig = "phone::call";

	if (sd == -1)
		sd = socket(AF_INET, SOCK_STREAM, 0);

	if (sd == -1 || (hp = gethostbyname(dstname)) == (struct hostent *)null)
	{
		perror(errsig);
		return ((connection *)null);
	}

	memset((char *)&sa, '\0', sizeof(sa));
	sa.sin_family = htons(hp->h_addrtype);
	sa.sin_port = htons(dstport);
	memcpy((char *)&sa.sin_addr.s_addr, (char *)hp->h_addr, hp->h_length);

	if (connect(sd, (struct sockaddr *)&sa, sizeof(sa)) == -1)
	{
		perror(errsig);
		return ((connection *)null);
	}


	/* Our copy of the socket descriptor must be reset now
	 * because by passing it on to a connection we are releasing
	 * ownership of it.  The connection destructor
	 * will close it without our regaining control which
	 * will render our sd value invalid as a handle.
	 */

	connection * pc = new connection(sd, dstname, dstport);
	sd = -1;
	return (pc);
}

