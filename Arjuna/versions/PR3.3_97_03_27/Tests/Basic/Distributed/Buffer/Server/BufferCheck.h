/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BufferCheck.h,v 1.1 1993/11/11 15:26:04 nmcl Exp $
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
