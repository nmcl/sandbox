/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BufferCheckDriver.cc,v 1.4 1995/08/09 11:15:02 nmcl Exp $
 */

#include <Config/Configure.h>

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef IOSTREAM_H_
# include <System/iostream.h>
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

#ifndef SIMPLERPCCLIENT_H_
#  include <RPC/SimpleRpc/SimpleRpcClient.h>
#endif


const long DO_WORK = 1;


int main(int argc, char** argv)
{
    Buffer ToSend, Received;
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
    char* hostname = 0;
    char* servicename = "BufferServer";
    SimpleRpcClient* service = (SimpleRpcClient*) 0;
    long opcode = DO_WORK, errcode = 0;

    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-host") == 0)
	    hostname = argv[i+1];
	if (::strcmp(argv[i], "-service") == 0)
	    servicename = argv[i+1];
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: BufferCheckDriver -host <host name> -service <service name>" << endl;
	    return 0;
	}
    }

    service = new SimpleRpcClient(servicename, hostname);

    if (service->initiate() != OPER_DONE)
    {
	cerr << "Constructor error\n" << flush;
	delete service;
	exit(0);
    }
    else
	cout << "Server constructed successfully." << endl;

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

    Buffer *SendBuff[2], *RecBuff[2];
    Buffer opBuff, errBuff;
    int numberReceived = -1;

    opBuff.pack(opcode);
    SendBuff[0] = &opBuff;
    SendBuff[1] = &ToSend;
    RecBuff[0] = &errBuff;
    RecBuff[1] = &Received;

    if (service->call(SendBuff, 2, RecBuff, numberReceived) == OPER_DONE)
    {
	errBuff.unpack(errcode);

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
	    delete string_test;
    }
    else
	cout << "Call error!" << endl;

    service->terminate();
    
    delete service;

    return 0;
}

