/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: filestrings.cc,v 1.3 1993/03/18 15:43:15 n048z Exp $
 *
 */

#ifndef SYS_TYPES_H_
#include <System/sys/types.h>
#endif

#ifndef OSFCN_H_
#include <System/osfcn.h>
#endif

#ifndef STDLIB_H_
#include <System/stdlib.h>
#endif

#ifndef STDIO_H_
#include <System/stdio.h>
#endif

#ifndef ASSERT_H_
#include <System/assert.h>
#endif

#ifndef STRING_H_
#include <System/string.h>
#endif

#ifndef FCNTL_H_
#include <System/fcntl.h>
#endif


#include "defs.h"
#include "strlist.h"

#if defined(DEBUG) && !defined(DEBUG_H_)
#include <Common/Debug.h>
#endif

#define	BFRSZ	2048

/***
 * Read a series of null-terminated strings from a file.
 *
 * Inputs:
 *	path	- path of file to read
 *
 * Returns pointer to a new str_list containing the file contents if
 * successful, returns null otherwise.
 */

str_list * file_strings(char * path)
{
	FILE * f = fopen(path, "r");
	char bfr[BFRSZ];

	if (f == (FILE *)null)
		return ((str_list *)null);

	str_list * psl = new str_list;

	do
	{
		char * p = bfr;
		char * plim = bfr + BFRSZ - 1;
		char c = fgetc(f);
		int i = 0;

		while (c == '\0' && !feof(f))
			c = fgetc(f);

		while (c != '\0' && !feof(f) && p < plim)
		{
			*p++ = c;
			c = fgetc(f);
			i++;
		}

		*p++ = '\0';

		if (i > 0)
		{
		    printf("adding %s\n", bfr);  
		    psl->add_str(bfr);
		}

	} while (!feof(f));

	fclose(f);
	return (psl);
};

#undef	BFRSZ

