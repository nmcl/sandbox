/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: dir.cc,v 1.4 1993/03/18 15:43:09 n048z Exp $
 *
 */

#ifndef SYS_TYPES_H_
#include <System/sys/types.h>
#endif

#ifndef DIRENT_H_
#include <System/dirent.h>
#endif

#ifndef SYS_STAT_H_
#include <System/sys/stat.h>
#endif

#ifndef hpux
#ifndef OSFCN_H_
#include <System/osfcn.h>
#endif
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


#include "defs.h"
#include "strlist.h"

#if defined(DEBUG) && !defined(DEBUG_H_)
#include <Common/Debug.h>
#endif


	/* This shouldn't be necessary, but apparently
	 * the CC version of <sys/dir.h> on some machines
	 * won't  have it.
	 */
#ifndef S_ISREG
#define	S_ISREG(m)	(((m)&S_IFMT) == S_IFREG)
#endif


/***
 * Read all entries in a specified directory and return a str_list
 * containing either full pathnames or just filenames for all regular
 * files listed in the directory (depending on option selected.)
 *
 * Inputs:
 *	path	- pointer to path of directory (optionally terminated
 *		  by a '/')
 *	fullpath - true => supply  full pathnames, false => only entry names
 *
 * Returns null if an error occurred, a pointer to the new str_list
 * otherwise.
 */

str_list * dir(char * path, boolean fullpath)
{
	DIR * dirp;

	struct dirent * ent;

	char bfr[PATHBFRSZ];
	str_list * psl = new str_list;

	if ((dirp = opendir(path)) == null)
	{
		delete psl;
		return ((str_list *)null);
	}

	while ((ent = readdir(dirp)) != null)
	{
		char * p = path;
		char * q = bfr;
		char * qlim = bfr + PATHBFRSZ - 1;

			/* Copy the path & ensure it ends with '/'
			 */
		while (q < qlim && *p != '\0')  *q++ = *p++;
		if (q > bfr && q[-1] != '/')  *q++ = '/';

			/* append the entry name
			 */
		p = ent->d_name;
		while (q < qlim && *p != '\0') *q++ = *p++;
		*q = '\0';

			/* include only regular files in our list
			 */
		struct stat statbuf;
		int i = stat(bfr, &statbuf);
#ifdef DEBUG
		debug_stream << FUNCTIONS << FAC_RPC << VIS_PUBLIC;
		debug_stream << "dir: fullpathname is " << bfr << "\n" << flush;
#endif
		if (i >= 0 && S_ISREG(statbuf.st_mode))
		{
			if (fullpath)
				psl->add_str(bfr);
			else
				psl->add_str(ent->d_name);
		}
	}
	closedir(dirp);
	return (psl);
}

