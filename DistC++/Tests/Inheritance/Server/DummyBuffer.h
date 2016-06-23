/*
 * Copyright (C) 1997,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyBuffer.h,v 1.2 1997/10/03 14:31:17 nmcl Exp $
 */

#ifndef DUMMYBUFFER_H_
#define DUMMYBUFFER_H_

#include <BufferCheck.h>

class DummyBuffer : public BufferCheck
{
public:
    DummyBuffer (int&);
    virtual ~DummyBuffer ();

    void Test (Buffer, Buffer&);
};

#endif
