/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: shadow.cc,v 1.4 1993/03/18 15:43:22 n048z Exp $
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

#ifndef ASSERT_H_
#include <System/assert.h>
#endif

#include "defs.h"

#if defined(DEBUG) && !defined(DEBUG_H_)
#include <Common/Debug.h>
#endif

/***
 * Given a path, return another path with the file renamed
 * to a shadow name.  The shadow file name is constructed
 * from the old one bracketed by given characters.
 *
 * Inputs:
 *	path	- path of file whose shadow path is to be built
 *	c1	- prefix character
 *	c2	- suffix character
 *
 * Returns newly constructed path string.
 */

char * shadow(char * path, char c1, char c2)
{
	int n = strlen(path);
	char * spath = new char[n + 3];

	if (spath == (char *)null)
	{
	    fprintf(stderr, "Shadow: Out of memory\n");
	    exit(1);
	}

	char* pfile = strrchr(path, '/');
	pfile = (pfile == (char *)null ? path : pfile + 1);

	char * p = spath;
	while (path < pfile)
		*p++ = *path++;
	*p++ = c1;
	while (*path != '\0')
		*p++ = *path++;
	*p++ = c2;
	*p++ = '\0';
	return (spath);
}

