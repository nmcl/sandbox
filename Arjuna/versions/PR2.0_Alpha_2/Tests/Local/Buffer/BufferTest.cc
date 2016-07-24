/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BufferTest.cc,v 1.1 1993/11/03 15:00:20 nmcl Exp $
 */

#include <System/string.h>
#include <System/stdlib.h>
#include <System/iostream.h>

#include <Common/Buffer.h>
#include <Common/Boolean.h>

typedef char* string;

int main(int, char*[])
{
    Buffer Buff;
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
    string string_test = "Testing";

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

    if (! Buff.pack(char_test))
	cout << "Buff.pack(char): failed\n";
    char_test = ' ';
    if (! Buff.pack(int_test))
	cout << "Buff.pack(int): failed\n";
    int_test = 0;
    if (! Buff.pack(uint_test))
	cout << "Buff.pack(uint): failed\n";
    uint_test = 0;
    if (! Buff.pack(long_test))
	cout << "Buff.pack(long): failed\n";
    long_test = 0;
    if (! Buff.pack(ulong_test))
	cout << "Buff.pack(ulong): failed\n";
    ulong_test = 0;
    if (! Buff.pack(short_test))
	cout << "Buff.pack(short): failed\n";
    short_test = 0;
    if (! Buff.pack(ushort_test))
	cout << "Buff.pack(ushort): failed\n";
    ushort_test = 0;
    if (! Buff.pack(float_test))
      cout << "Buff.pack(float): failed\n";
    float_test = 0;
    if (! Buff.pack(double_test))
	cout << "Buff.pack(double): failed\n";
    double_test = 0;
    if (! Buff.pack(boolean_test))
	cout << "Buff.pack(boolean): failed\n";
    boolean_test = FALSE;
    if (! Buff.pack(string_test))
	cout << "Buff.pack(string): failed\n";
    string_test = 0;

    if (! Buff.unpack(char_test))
	cout << "Buff.unpack(char): failed\n";
    if (! Buff.unpack(int_test))
	cout << "Buff.unpack(int): failed\n";
    if (! Buff.unpack(uint_test))
	cout << "Buff.unpack(uint): failed\n";
    if (! Buff.unpack(long_test))
	cout << "Buff.unpack(long): failed\n";
    if (! Buff.unpack(ulong_test))
	cout << "Buff.unpack(ulong): failed\n";
    if (! Buff.unpack(short_test))
	cout << "Buff.unpack(short): failed\n";
    if (! Buff.unpack(ushort_test))
	cout << "Buff.unpack(ushort): failed\n";
    if (! Buff.unpack(float_test))
	cout << "Buff.unpack(float): failed\n";
    if (! Buff.unpack(double_test))
	cout << "Buff.unpack(double): failed\n";
    if (! Buff.unpack(boolean_test))
	cout << "Buff.unpack(boolean): failed\n";
    if (! Buff.unpack(string_test))
	cout << "Buff.unpack(string): failed\n";

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
    else
	delete [] string_test;
}
