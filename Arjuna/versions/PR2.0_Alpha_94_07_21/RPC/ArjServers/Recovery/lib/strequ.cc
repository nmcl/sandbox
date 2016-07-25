/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: strequ.cc,v 1.3 1993/03/18 15:43:24 n048z Exp $
 *
 */

#include  "defs.h"

#if defined(DEBUG) && !defined(DEBUG_H_)
#include <Common/Debug.h>
#endif


/***
 * test two strings for equality
 */

boolean strequ(char * s1, char * s2)
{
	while (*s1 != '\0')
		if (*s1++ != *s2++)
			return (false);
	return (*s1 == *s1);
}

