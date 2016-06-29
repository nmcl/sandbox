/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BufferCheck.h,v 1.1 1997/06/09 19:52:28 nmcl Exp $
 */

#ifndef BUFFERCHECK_H_
#define BUFFERCHECK_H_

#include <Common/Buffer.h>

class BufferCheck
{
public:
    BufferCheck(int&);
    ~BufferCheck();

    void Test(Buffer, Buffer&);
};

#endif
