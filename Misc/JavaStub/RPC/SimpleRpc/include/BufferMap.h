/*
 * Copyright 1995, 1996,
 * Department of Computing Science, University of Newcastle upon Tyne, UK.
 *
 * $Id: BufferMap.h,v 1.2 1996/04/04 08:51:48 nmcl Exp $
 */

#ifndef BUFFERMAP_H_
#define BUFFERMAP_H_

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

class BufferMap
{
public:
    BufferMap ();
    ~BufferMap ();

    const char*  bufferPtr;
    size_t       sizeLeft;
    char*        buffer;
    size_t       bufferSize;
};

#endif