/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Utility.cc,v 1.3 1995/07/03 14:30:05 ngdp Exp $
 */

/*
 * Stub generator utility routines
 *
 */

#include <strstream.h>
#include <unistd.h>
#include <stdlib.h>

#ifndef STCOMMONT_H_
#  include "StCommonT.h"
#endif

#ifndef STRING_H_
#  include "String.h"
#endif

#ifndef UTILITY_H_
#  include "Utility.h"
#endif

static const char RCSid[] = "$Id: Utility.cc,v 1.3 1995/07/03 14:30:05 ngdp Exp $";

/*
 * Scan through PATH environment variable looking for program.
 */

char *const locateProg ( const char *progName )
{
    static const int pathBufferSize = 1024;
    static char pathBuffer[pathBufferSize]; /* default init to 0 */
    char *path = ::getenv("PATH");
    char *foundPath = 0;
    char *start = path;
    char *end = 0;
    Boolean lastElem = FALSE;
    Boolean foundProg = FALSE;

    /* Check do not already have a full path */

    if (progName[0] == '/')
    {
	::strcpy(pathBuffer, progName);
	if (::access(pathBuffer, X_OK) == 0)
	{
	    foundPath = new char[::strlen(pathBuffer) + 1];
	    ::strcpy(foundPath, pathBuffer);
	    return foundPath;
	}
    }

    /*
     * Assume that progname is relative name
     * walk down path scanning for ':'
     */

    while ((start != 0) && (!lastElem))
    {
	int len;

	/* find next ':' */

	if ((end = ::strchr(start, ':')))
	{
	    len = end-start;
	}
	else
	{
	    /* None left this must be the last element of the PATH */
	    lastElem  = TRUE;
	    len = strlen(start);
	}

	if (len > 0)
	{
	    ::strncpy(pathBuffer, start, len);

	    /* add a `/` if needed */

	    if (pathBuffer[len-1] != '/')
	    {
		pathBuffer[len++] = '/';
	    }
	}

	pathBuffer[len] = 0;
	::strcat(pathBuffer, progName);

	if (::access(pathBuffer, X_OK) == 0)
	{
	    foundProg = TRUE;
	    break;
	}

	/* step on to next path element */

	start = end + 1;
    }

    if (!foundProg)
	return 0;

    foundPath = new char[::strlen(pathBuffer) + 1];
    ::strcpy(foundPath, pathBuffer);
    return foundPath;
}

/*
 * Deliberatley use our own buffer here to ensure it gets deleted
 */

String constructName ( const String& root, long number )
{
    char buffer [32];
    ostrstream tss(buffer, 32);
    String sp;

    tss << number << "_" << ends;

    /* convert tss.str to a string and add to root */

    sp = root + tss.str();

    return sp;
}

/*
 * Hashing routine taken from the Dragon book (Aho et al).
 */

long hashIt ( const String& name )
{
    register const char *cp = name;
    register unsigned long g, h = 0;

    while (cp && (*cp != '\0'))
    {
        h = (h << 4) + (*cp);
        if ((g = h & 0xf0000000))
        {
            h = h ^ (g >> 24);
            h = h ^ g;
        }
        cp++;
    }
    return h;
}

#ifndef HAVE_BOOLEAN_TYPE
ostream& operator<< ( ostream& s, Boolean b )
{
    s << (b != FALSE ? "TRUE" : "FALSE");
    return s;
}
#endif
