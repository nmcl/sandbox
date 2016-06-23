/*
 * Copyright 1995, 1996-1998,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: BufferMap.cc,v 1.2 1998/11/20 08:28:42 nmcl Exp $
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
