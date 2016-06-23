/*
 * Copyright (C) 1997,
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: DummyBufferDriver.cc,v 1.2 1997/09/26 09:08:51 nmcl Exp $
 */

#include <Config/Configure.h>

#include <System/unistd.h>

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

#include "DummyBuffer_stub.h"

const char* defaultServiceName = "DummyBuffer";

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
    ClientRpc* cRPC = (ClientRpc*) 0;
    ClientRpcManager* rpcC = (ClientRpcManager*) 0;
    char* hostname = (char*) 0;
    char* servicename = (char*) 0;
    Uid u("80f09608:7214:2d6a1bae:2");

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-host") == 0)
	    hostname = argv[i+1];
	if (::strcmp(argv[i], "-service") == 0)
	    servicename = argv[i+1];
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " -host <host name> [-service <service name>] [-help]" << endl;
	    return 0;
	}
    }

    if (hostname == (char*) 0)
    {
	cout << "Error. Hostname not given." << endl;
	return -1;
    }

    if (servicename == (char*) 0)
	servicename = (char*) defaultServiceName;
    
    cRPC = new ClientRpc(servicename);
    (cRPC->control()).setHost(hostname);
    (cRPC->control()).setProtocol(STREAM);
    (cRPC->control()).setServerID(u);      // shared server id.
    rpcC = new ClientRpcManager(cRPC);
    
    DummyBuffer* Use = new DummyBuffer(res, rpcC);
    
    if (res != 0)
    {
	cerr << "Constructor error\n" << flush;
	delete Use;
	exit(0);
    }

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
    
    Use->Test(ToSend, Received1);

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

    cout << "calling base class Test" << endl;
    
    Use->BufferCheck::Test(ToSend, Received2);
    
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
    
    delete Use;
    delete rpcC;
    delete cRPC;

    return 0;
}

