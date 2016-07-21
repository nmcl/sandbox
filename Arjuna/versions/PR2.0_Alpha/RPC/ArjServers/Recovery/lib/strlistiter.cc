/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: strlistiter.cc,v 1.4 1993/03/18 15:43:30 n048z Exp $
 * 
 */

#ifndef STDIO_H_
#include <System/stdio.h>
#endif

#ifndef STDLIB_H_
#include <System/stdlib.h>
#endif

#ifndef ASSERT_H_
#include <System/assert.h>
#endif

#include "defs.h"
#include "strlist.h"
#include "strlistiter.h"

#if defined(DEBUG) && !defined(DEBUG_H_)
#include <Common/Debug.h>
#endif


/***
 * Build a str_list iterator.
 *
 * Inputs:
 *	sl		- pointer to str_list to be iterated over
 *	startindex	- position at which to start (normally 0)
 */

str_list_iterator::str_list_iterator(str_list * sl, int startindex)
{
	psl = sl;
	index = (startindex >= 0 ? startindex : 0);
}




str_list_iterator::~str_list_iterator()
{
}



/***
 * Return the next string in the str_list, null on end of list.
 */

char * str_list_iterator::str_next()
{
	if (psl == (str_list *)null || index >= psl->edge_count)
		return ((char *)null);
	else
	{
#ifndef hpux
		assert(psl->edge != (char **)null);
#endif
		return (psl->edge[index++]);
	}
}

