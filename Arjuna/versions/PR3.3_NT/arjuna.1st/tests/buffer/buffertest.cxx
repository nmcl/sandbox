/*
 * Copyright (C) 1993
 * 
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BufferTest.cc,v 1.8 1994/11/17 15:51:21 ngdp Exp $
 */

#include <System/string.h>
#include <System/stdlib.h>
#include <System/iostream.h>
#include <System/memory.h>
#include <Common/Boolean.h>
#include <Common/Buffer.h>

typedef char* string;

int main (int, char*[])
{
    size_t i = 0;
    Buffer Buff;
    Buffer Buff2;
    
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
    void *block_test = new char[72];
    char *block_master = new char[72];
    char *temp = (char *)block_test;
    
    size_t block_size = 72;
    
    for (i = 0; i < block_size; i++)
    {
		block_master[i] = 'a';
    }
    ::memcpy(block_test, block_master, block_size);
    
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
    cout << "packing string(a second time) " << string_test << "\n" << flush;
    cout << "packing block of memory ";
    for (i = 0; i < block_size; i++)
	cout << block_master[i];
    cout << "\n" << flush;
    
    cout << "packing block of memory(a second time) \n" << flush;

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
    float_test = 0.0;
    if (! Buff.pack(double_test))
	cout << "Buff.pack(double): failed\n";
    double_test = 0;
    if (! Buff.pack(boolean_test))
	cout << "Buff.pack(boolean): failed\n";
    boolean_test = FALSE;
    if (! Buff.packMappedString(string_test))
	cout << "Buff.pack(string): failed\n";
    if (! Buff.packMappedString(string_test))
	cout << "Buff.pack(string alias): failed\n";
    if (! Buff.packMappedBlock(block_test,(size_t)block_size))
	cout << "Buff.pack(block test): failed\n";
    if (! Buff.packMappedBlock(block_test,(size_t)block_size))
	cout << "Buff.pack(block testalias ): failed\n";

    cout << "pack buffer 1 into buffer 2" << endl;
    
    Buff.packInto(Buff2);

    string_test = 0;
    delete block_test;
    block_test = 0;

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
    {
	string s2 = 0;
	
	if (! Buff.unpack(s2))
	    cout << "Buff.unpack(string alias): failed\n";
	
	if (s2 != string_test)
	    cout << "Buffer string pointer aliasing failed\n";
	else
	    cout << "Buffer string pointer aliasing ok\n";
	
	delete [] string_test;
    }

    if (! Buff.unpack(block_test,block_size))
	cout << "Buff.unpack(block test): failed\n";

    cout << "unpacked block ";
    temp = (char *)block_test;
    
    for (i = 0; i < block_size; i++)
	cout << temp[i];
    cout << "\n" << flush;
    
    if (memcmp(block_test, block_master, block_size) != 0)
	cerr << "An error has occurred for block unpacking\n" << flush;
    else
    {
	void * b2 = 0;
	size_t size = block_size;
	
	if (! Buff.unpack(b2, size))
	    cout << "Buff.unpack(block alias): failed\n";
	
	if ((b2 != block_test) || (size != block_size))
	    cout << "Buffer memory pointer aliasing failed\n";
	else
	    cout << "Buffer memory pointer aliasing ok\n";
	
	delete [] block_test;
	block_test = 0;
    }

    cout << "restoring original buffer" << endl;
    
    Buff.unpackFrom(Buff2);
    
    string_test = 0;
    if (block_test)
	delete [] block_test;
    block_test = 0;

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
    {
	string s2 = 0;
	
	if (! Buff.unpack(s2))
	    cout << "Buff.unpack(string alias): failed\n";
	
	if (s2 != string_test)
	    cout << "Buffer pointer aliasing failed\n";
	else
	    cout << "Buffer pointer aliasing ok\n";
	
	delete [] string_test;
    }

    if (! Buff.unpack(block_test,block_size))
	cout << "Buff.unpack(block test): failed\n";

    cout << "unpacked block ";
    temp = (char *)block_test;
    
    for (i = 0; i < block_size; i++)
	cout << temp[i];
    cout << "\n" << flush;
    
    if (memcmp(block_test, block_master, block_size) != 0)
	cerr << "An error has occurred for block unpacking\n" << flush;
    else
    {
	void * b2 = 0;
	size_t size = block_size;
	
	if (! Buff.unpack(b2, size))
	    cout << "Buff.unpack(block alias): failed\n";
	
	if ((b2 != block_test) || (size != block_size))
	    cout << "Buffer pointer aliasing failed\n";
	else
	    cout << "Buffer pointer aliasing ok\n";
	
	delete [] block_test;
    }
    delete block_master;
    return 0;
}
