/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: strlist.cc,v 1.5 1993/03/18 15:43:27 n048z Exp $
 *
 */

#ifndef STDLIB_H_
#include <System/stdlib.h>
#endif

#ifndef STDIO_H_
#include <System/stdio.h>
#endif

#ifndef MALLOC_H_
#include <System/malloc.h>
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

#ifndef ASSERT_H_
#include <System/assert.h>
#endif

#include "defs.h"
#include "strlist.h"

#if defined(DEBUG) && !defined(DEBUG_H_)
#include <Common/Debug.h>
#endif


/***
 * str_list constructor
 *
 * A str_list is used to manage a list of strings.
 * Inputs:
 *	max-size	- initial maxumum size of list
 *			  (will be expanded dynamically as needed)
 */

str_list::str_list(int max_size)
{
	edge = new char *[max_size];

	int i = 0;
		/* initialize the edge vector to nulls
		 */
	while (i < max_size)
	{
		edge[i] = (char *)null;
		i++;
	}
	edge_max = max_size;
	edge_count = 0;
}




/***
 * destructor
 *	delete all strings and the edge vector itself
 */

str_list::~str_list()
{
	char ** p = edge;
	int i = 0;

		/* delete the strings
		 */
	while (i < edge_count)
	{
		delete *p;
		p++;
		i++;
	}

		/* and delete the edge vector
		 */
	delete edge;
}



/***
 * How many strings in the list?
 */

int str_list::count()
{
	return (edge_count);
}



/***
 * merge_str
 *	Ensure that a given string is present in the string
 *	vector. (i.e.  add it only if not already present)
 *
 *	A linear search of an unordered list is used because
 *	the average list length is expected to be small.
 *
 * Inputs:
 *	str	- string to add
 */

void str_list::merge_str(char * str)
{
	extern boolean strequ(char *, char *);

	/* First look through the strings already present.
	 * If the input is already there then change nothing.
	 */

	int i = 0;
	while (i < edge_count && edge[i] != (char *)null)
		if (strequ(str, edge[i]))
			return;
		else
			i++;

	add_str(str);
}



/***
 * Build a gather-array for use with the gather-write call (writev())
 * to write all the strings in this str_list.
 *
 * Outputs:
 *	len 	- pointer to integer that will be set to the total number
 *		  of bytes to be written
 *	count	- number of entries
 *
 * Returns the vector of struct iovec's needed by writev (which must
 * be freed by the caller)
 */

struct iovec * str_list::get_iovec(int * totalsz, int * count)
{
	struct iovec * piov = new struct iovec[edge_count];
	int nbytes = 0;

	for (int i = 0; i < edge_count; i++)
	{
		piov[i].iov_base = (caddr_t)edge[i];
		nbytes +=
		piov[i].iov_len = strlen(edge[i]) + 1;
	}

	*count = edge_count;
	*totalsz = nbytes;
	return (piov);
}



/***
 * Add a new string to a str_list
 *	The string edge vector may be grown here, and a panic
 *	exit will be taken if there is no memory for this
 *	(a very unlikely event)
 *
 * Inputs:
 *	str	- string to add
 */

void str_list::add_str(char * str)
{
	char * memerr  = "str_list::add_str: out of memory (%d)\n";
#ifndef hpux
	assert(edge_max >= edge_count);
#endif

	/* Is there enough room to add it?
	 */

	if (edge_max == edge_count)
	{
		/* No-- increase the size
		 */
		int newsize = edge_max + Size_increment;
		char ** newedge;

		newedge = (char **)realloc((malloc_t) edge, newsize * sizeof(char *));
		if (newedge == (char **)null)
		{
			/* We didn't have the memory.  Bail out.
			 */
		    fprintf(stderr, memerr, 1);
		    exit(1);
		}
		else
		{
			/* Everything ok, make sure the new space is
			 * initialized & update the object's state.
			 */
			edge = newedge;

			for (int i = edge_max; i < newsize; i++)
				edge[i]  = (char *)null;

			edge_max = newsize;
		}
	}

	if ((edge[edge_count++] = strdup(str)) ==  (char *)null)
	{
	    fprintf(stderr, memerr, 2);
	    exit(1);
	}
}




/***
 * Get a specific string from a str_list, indexed by position
 */

char * str_list::get_str(int i)
{
	if (i >= 0 && i < edge_count)
		return (edge[i]);
	else
		return ((char *)null);
}



/***
 * Have the string list print itself
 */

void str_list::print()
{
	boolean  first = true;
	printf("[");
	for (int i = 0; i < edge_count; i++)
	{
		if (first)
			first = false;
		else
			printf(", ");
		printf("\"%s\"", edge[i]);
	}
	printf("]");
}
