/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: connection.cc,v 1.4 1993/03/18 15:43:07 n048z Exp $
 *
 */

#ifndef STDIO_H_
#include <System/stdio.h>
#endif

#ifndef STDLIB_H_
#include <System/stdlib.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef SYS_TYPES_H_
#include <System/sys/types.h>
#endif

#ifndef SYS_UIO_H_
#include <System/sys/uio.h>
#endif

#ifndef UNISTD_H_
#include <System/unistd.h>
#endif


#include "defs.h"
#include "strlist.h"
#include "connection.h"

#if defined(DEBUG) && !defined(DEBUG_H_)
#include <Common/Debug.h>
#endif

/***
 * To initialize a connection we just want to record the stream
 * descriptor and record the party we're talking to.  Don't allocate
 * a netbuffer until we know we need one.
 */

connection::connection(int xsd, char * callername, int callerport)
{
	sd = xsd;
	if (sd != -1)
	{
		rem_name = strdup(callername);
		rem_port = callerport;
	}
	else
	{
		rem_name = (char *)null;
		rem_port = -1;
	}

	netbfr = (char *)null;
	netbfrsz = 0;
}


/***
 * To destroy a connection close the stream and free the
 * netbuffer, if any.
 */

connection::~connection()
{
	if (sd != -1)
		close(sd);

	if (netbfr != (char *)null)
	{
		delete netbfr;
		netbfrsz = 0;
	}

	if (rem_name != (char *)null)
		delete rem_name;
}


/***
 * Just map a connection::cread onto a normal system read
 */

char * connection::cread(int * size)
{
	if (netbfr == (char *)null)
	{
		netbfr = new char[NETBFRSZ];
		netbfrsz = NETBFRSZ;
	}

	if (netbfr == (char *)null)
	{
	    fprintf(stderr, "connection: panic! Out of memory\n");
	    exit(1);
	}

	int i = read(sd, netbfr, netbfrsz);

	if (i < 0)
	    perror("connection: cread(char*,int)->int");

	*size = i;
	return (i > 0 ? netbfr : (char *)null);
}



/***
 * Read a buffer-full, break it into null-terminated strings,
 * and build a str_list out of these.
 */

str_list * connection::cread()
{
	if (netbfr == (char *)null)
	{
		netbfr = new char[NETBFRSZ];
		netbfrsz = NETBFRSZ;
	}

	if (netbfr == (char *)null)
	{
	    fprintf(stderr, "connection: panic! Out of memory\n");
	    exit(1);
	}

	int rsz = read(sd, netbfr, netbfrsz);

	if (rsz < 0)
	    perror("cread()->str_list");


	if (rsz > 0)
	{
		/* Break out the strings in the buffer
		 * and stick them all in a str_list.
		 */
		str_list * psl = new str_list();
		char * p = netbfr;
		char * limitp = p + rsz;

		while (p < limitp)
		{
			char * q = p;
			while (q < limitp && *q != '\0')
				q++;

			if (*q == '\0')
				psl->add_str(p);
			p = q + 1;
		}

		return (psl);
	}
	else
		return ((str_list *)null);
}



/***
 * Just map a connection::cwrite onto a normal system write
 */

int connection::cwrite(char * bfr, int size)
{
	int i = write(sd, bfr, size);

	if (i < size)
	    perror("cwrite(char *,int)->int");

	return (i);
}



/***
 * Write a str_list into a stream.
 *
 * user a gather-write
 *
 * Returns true if completely successful.
 */

boolean connection::cwrite(str_list * psl)
{
	int sz, n;
	struct iovec * piov = psl->get_iovec(&sz, &n);

	boolean result = (sz == writev(sd, piov, n));
	delete piov;

	return (result);
}


/***
 * Return the name of the machine at the other end of
 * the wire.
 */

char * connection::rname()
{
	return (rem_name);
}


/***
 * Return the number of the port our communication
 * partner is using.
 */

int connection::rport()
{
	return (rem_port);
}

