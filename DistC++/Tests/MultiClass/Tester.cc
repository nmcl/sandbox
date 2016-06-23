/*
 * Copyright (C) 1997,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: Tester.cc,v 1.1 1997/10/03 14:31:59 nmcl Exp $
 */

#include <Config/Configure.h>

#include <System/unistd.h>
#include <System/sys/uio.h>
#include <System/protos.h>
#include <System/netinet/in.h>
#include <System/netdb.h>

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
#  include <System/iostream.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef CLIENTRPC_H_
#  include <StubGen/ClientRpc.h>
#endif

#ifndef CLIENTRPCMAN_H_
#  include <StubGen/ClientRpcMan.h>
#endif

#include "BufferCheck_stub.h"
#include "DummyBuffer_stub.h"

int main (int argc, char** argv)
{
    Buffer ToSend;
    Buffer Received1;
    Buffer Received2;    
    char char_test = 'a';
    int int_test = -1000;
    unsigned int uint_test = 1000;
    long long_test = -123456789;
    unsigned long ulong_test = 123456789;
    short short_test = -1;
    unsigned short ushort_test = 1;
    float float_test = 1234.5;
    double double_test = 1.23456;
    Boolean boolean_test = TRUE;
    char *string_test = "Testing";
    int res = -1;
    ClientRpc* cRPC1 = (ClientRpc*) 0;
    ClientRpcManager* rpcC1 = (ClientRpcManager*) 0;
    ClientRpc* cRPC2 = (ClientRpc*) 0;
    ClientRpcManager* rpcC2 = (ClientRpcManager*) 0;    
    char hostname[MAXHOSTNAMELEN+1];
    const char* servicename = "servercode";
    unsigned short dummyBuffer = 0;
    unsigned short bufferCheck = 0;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-dummyport") == 0)
	    dummyBuffer = (unsigned short) ::atoi(argv[i+1]);
	if (::strcmp(argv[i], "-bufferport") == 0)
	    bufferCheck = (unsigned short) ::atoi(argv[i+1]);
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " -dummyport <port> -bufferport <port> [-help]" << endl;
	    return 0;
	}
    }

    ::gethostname(hostname, MAXHOSTNAMELEN);
    
    cRPC1 = new ClientRpc(servicename);
    (cRPC1->control()).setHost(hostname);
    (cRPC1->control()).setPort(dummyBuffer);
    rpcC1 = new ClientRpcManager(cRPC1);

    cRPC2 = new ClientRpc(servicename);
    (cRPC2->control()).setHost(hostname);
    (cRPC2->control()).setPort(bufferCheck);
    rpcC2 = new ClientRpcManager(cRPC2);
    
    DummyBuffer* Use1 = new DummyBuffer(res, rpcC1);
    BufferCheck* Use2 = new BufferCheck(res, rpcC2);
    
    cout << "packing char " << char_test << "\n" << flush;
    cout << "packing (signed) int " << int_test << "\n" << flush;
    cout << "packing (unsigned) int " << uint_test << "\n" << flush;
    cout << "packing (signed) long " << long_test << "\n" << flush;
    cout << "packing (unsigned) long " << ulong_test << "\n" << flush;
    cout << "packing (signed) short " << short_test << "\n" << flush;
    cout << "packing (unsigned) short " << ushort_test << "\n" << flush;
    cout << "packing float " << float_test << "\n" << flush;
    cout << "packing double " << double_test << "\n" << flush;
    cout << "packing boolean " << boolean_test << "\n" << flush;
    cout << "packing string " << string_test << "\n" << flush;

    ToSend.pack(char_test);
    char_test = ' ';
    ToSend.pack(int_test);
    int_test = 0;
    ToSend.pack(uint_test);
    uint_test = 0;
    ToSend.pack(long_test);
    long_test = 0;
    ToSend.pack(ulong_test);
    ulong_test = 0;
    ToSend.pack(short_test);
    short_test = 0;
    ToSend.pack(ushort_test);
    ushort_test = 0;
    ToSend.pack(float_test);
    float_test = 0;
    ToSend.pack(double_test);
    double_test = 0;
    ToSend.pack(boolean_test);
    boolean_test = FALSE;
    ToSend.packMappedString(string_test);
    string_test = 0;
    
    Use1->Test(ToSend, Received1);
    
    Received1.unpack(char_test);
    Received1.unpack(int_test);
    Received1.unpack(uint_test);
    Received1.unpack(long_test);
    Received1.unpack(ulong_test);
    Received1.unpack(short_test);
    Received1.unpack(ushort_test);
    Received1.unpack(float_test);
    Received1.unpack(double_test);
    Received1.unpack(boolean_test);
    Received1.unpack(string_test);

    cout << "\n";
    cout << "unpacked char " << char_test << "\n" << flush;
    cout << "unpacked (signed) int " << int_test << "\n" << flush;
    cout << "unpacked (unsigned) int " << uint_test << "\n" << flush;
    cout << "unpacked (signed) long " << long_test << "\n" << flush;
    cout << "unpacked (unsigned) long " << ulong_test << "\n" << flush;
    cout << "unpacked (signed) short " << short_test << "\n" << flush;
    cout << "unpacked (unsigned) short " << ushort_test << "\n" << flush;
    cout << "unpacked float " << float_test << "\n" << flush;
    cout << "unpacked double " << double_test << "\n" << flush;
    cout << "unpacked boolean " << boolean_test << "\n" << flush;
    cout << "unpacked string " << string_test << "\n" << flush;

    if (string_test)
    {
	::delete [] string_test;
	string_test = 0;
    }

    cout << "calling 2nd" << endl;
    
    Use2->Test(ToSend, Received2);

    cout << "and here with " << Received2 << endl;
    
    Received2.unpack(char_test);
    Received2.unpack(int_test);
    Received2.unpack(uint_test);
    Received2.unpack(long_test);
    Received2.unpack(ulong_test);
    Received2.unpack(short_test);
    Received2.unpack(ushort_test);
    Received2.unpack(float_test);
    Received2.unpack(double_test);
    Received2.unpack(boolean_test);
    Received2.unpack(string_test);
    
    cout << "\n";
    cout << "unpacked char " << char_test << "\n" << flush;
    if (char_test != 'a')
	cerr << "An error has occurred for char\n" << flush;

    cout << "unpacked (signed) int " << int_test << "\n" << flush;
    if (int_test != -1000)
	cerr << "An error has occurred for int\n" << flush;

    cout << "unpacked (unsigned) int " << uint_test << "\n" << flush;
    if (uint_test != 1000)
	cerr << "An error has occurred for unsigned int\n" << flush;

    cout << "unpacked (signed) long " << long_test << "\n" << flush;
    if (long_test != -123456789)
	cerr << "An error has occurred for long\n" << flush;

    cout << "unpacked (unsigned) long " << ulong_test << "\n" << flush;
    if (ulong_test != 123456789)
	cerr << "An error has occurred for unsigned long\n" << flush;

    cout << "unpacked (signed) short " << short_test << "\n" << flush;
    if (short_test != -1)
	cerr << "An error has occurred for short\n" << flush;

    cout << "unpacked (unsigned) short " << ushort_test << "\n" << flush;
    if (ushort_test != 1)
	cerr << "An error has occurred for unsigned short\n" << flush;

    cout << "unpacked float " << float_test << "\n" << flush;
    if (float_test != 1234.5)
	cerr << "An error has occurred for float\n" << flush;

    cout << "unpacked double " << double_test << "\n" << flush;
    if (double_test != 1.23456)
	cerr << "An error has occurred for double\n" << flush;

    cout << "unpacked boolean " << boolean_test << "\n" << flush;
    if (boolean_test != TRUE)
	cerr << "An error has occurred for boolean\n" << flush;

    cout << "unpacked string " << string_test << "\n" << flush;
    if (strcmp(string_test, "Testing") != 0)
	cerr << "An error has occurred for string\n" << flush;    

    if (string_test)
	::delete [] string_test;
    
    delete Use1;
    delete rpcC1;
    delete cRPC1;

    delete Use2;
    delete rpcC2;
    delete cRPC2;    

    return 0;
}

