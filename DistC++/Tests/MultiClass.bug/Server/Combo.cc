/*
 * Copyright (C) 1997,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyBuffer.cc,v 1.1 1997/09/25 15:31:53 nmcl Exp $
 */

#define DUMMYBUFFER_SERVER
#define BUFFERCHECK_SERVER

#include <System/iostream.h>
#include <Thread/Thread.h>
#include "DummyBuffer_stub.h"

DummyBuffer::DummyBuffer(int& res)
			: BufferCheck(res)
{
    res = 0;
}

DummyBuffer::~DummyBuffer()
{
}

void DummyBuffer::Test (Buffer Received, Buffer& ToSend)
{
    ToSend.pack('b');
    ToSend.pack(-9999);
    ToSend.pack(6666);
    ToSend.pack(-123456);
    ToSend.pack(78910);
    ToSend.pack(-12);
    ToSend.pack(24);
    ToSend.pack((float) 1.2);
    ToSend.pack((double) 2.402);
    ToSend.pack(FALSE);
    ToSend.packNonMappedString("DummyBuffer");
}
