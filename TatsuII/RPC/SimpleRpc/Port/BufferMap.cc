/*
 * Copyright 1995, 1996,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: BufferMap.cc,v 1.1 1997/06/09 19:52:05 nmcl Exp $
 */

#ifndef BUFFERMAP_H_
#  include "BufferMap.h"
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

BufferMap::BufferMap ()
		     : bufferPtr(0),
		       sizeLeft(0),
		       buffer(0),
		       bufferSize(0)
{
}

BufferMap::~BufferMap ()
{
    if (buffer)
	::delete [] buffer;
}
