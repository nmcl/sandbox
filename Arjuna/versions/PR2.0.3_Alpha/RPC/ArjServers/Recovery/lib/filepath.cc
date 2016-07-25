/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: filepath.cc,v 1.3 1993/03/18 15:43:12 n048z Exp $
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

#if defined(DEBUG) && !defined(DEBUG_H_)
#include <Common/Debug.h>
#endif

/***
 * Construct a proper pathname given a directory & filename.
 * The caller must delete the returned string;
 *
 * Inputs:
 *	dirstr	- directory path
 *	namestr	- filename
 * Returns full path.
 */

char * filepath(char * dirstr, char * namestr)
{
	int j = strlen(dirstr);
	int k = strlen(namestr);
	char * path = new char[j + k + 2];
	char * p = path;
	char * q = dirstr;

	while (*q != '\0') *p++ = *q++;
	if (j > 0 && p[-1] != '/') *p++ = '/';
	q = namestr;
	while (*q != '\0') *p++ = *q++;
	*p = '\0';

	return (path);
}

