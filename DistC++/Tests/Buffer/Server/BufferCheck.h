/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BufferCheck.h,v 1.1 1997/09/25 15:31:46 nmcl Exp $
 */

#ifndef BUFFERCHECK_H_
#define BUFFERCHECK_H_

#include <Common/Buffer.h>

class BufferCheck
{
public:
    BufferCheck (int&);
    virtual ~BufferCheck ();

    virtual void Test (Buffer, Buffer&);
};

#endif
