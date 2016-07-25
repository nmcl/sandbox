/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BufferCheck.h,v 1.3 1993/03/18 15:05:53 n048z Exp $
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
