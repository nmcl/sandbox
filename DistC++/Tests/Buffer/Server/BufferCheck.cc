/*
 * Copyright (C) 1993-1998,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BufferCheck.cc,v 1.3 1998/11/20 08:28:56 nmcl Exp $
 */

#define BUFFERCHECK_SERVER

#include <System/iostream.h>

#include "BufferCheck_stub.h"

BufferCheck::BufferCheck(int& res)
{
    res = 0;
}

BufferCheck::~BufferCheck()
{
}

void BufferCheck::Test (Buffer Received, Buffer& ToSend)
{
    cout << "BufferCheck::Test" << endl;
    
    char char_test;
    int int_test;
    unsigned int uint_test;
    long long_test;
    unsigned long ulong_test;
    short short_test;
    unsigned short ushort_test;
    float float_test;
    double double_test;
    Boolean boolean_test;
    char *string_test = NULL;

    Received.unpack(char_test);
    Received.unpack(int_test);
    Received.unpack(uint_test);
    Received.unpack(long_test);
    Received.unpack(ulong_test);
    Received.unpack(short_test);
    Received.unpack(ushort_test);
    Received.unpack(float_test);
    Received.unpack(double_test);
    Received.unpack(boolean_test);
    Received.unpack(string_test);

    ToSend.pack(char_test);
    ToSend.pack(int_test);
    ToSend.pack(uint_test);
    ToSend.pack(long_test);
    ToSend.pack(ulong_test);
    ToSend.pack(short_test);
    ToSend.pack(ushort_test);
    ToSend.pack(float_test);
    ToSend.pack(double_test);
    ToSend.pack(boolean_test);
    ToSend.packMappedString(string_test);

    if (string_test)
	::delete [] string_test;
}
