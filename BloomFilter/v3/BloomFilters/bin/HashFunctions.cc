/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HashFunctions.cc,v 1.2 1998/05/28 14:50:28 nmcl Exp $
 */

#include <math.h>
#include <iostream.h>
#include <string.h>
#include <stdlib.h>
#include <strstream.h>

#include <Stat/PHistogram.h>
#include <ClassLib/Random.h>

#define TRUE 1
#define FALSE 0

enum HashType { KNUTH, RANDOM1, RANDOM2 };

const int numberSize = sizeof(long)*8;  // in bits

UniformStream* dataStream = 0;
PrecisionHistogram* histogram = 0;
int numbers = 0;
int iterations = 10000;

char* generateID ()
{
    size_t bufferSize = numbers*numberSize;
    char* buffer = ::new char[bufferSize];
    
    ::memset(buffer, bufferSize, '\0');

    strstream tos(buffer, bufferSize, ios::out);
	
    for (int j = 0; j < numbers; j++)
    {
	long value = (long) (*dataStream)();

	tos << value;
    }

    return buffer;
}

void print ()
{
    for (int i = 0; i < (int) histogram->numberOfBuckets(); i++)
    {
	double name = 0.0;

	if (histogram->bucketName(i, name))
	{
	    double size = 0.0;

	    histogram->sizeByName(name, size);
	    
	    cout << name << " " << size << endl;
	}
    }
}

void random1 ()
{
    for (int i = 0; i < iterations; i++)
    {
	char* buffer = generateID();

	unsigned long g, h = 0;
	const char *p = (const char *)buffer;
	size_t len = numbers*numberSize;

	if (buffer)
	{
	    while (len != 0)
	    {
		g = *(unsigned long*) p;
		
		h = h ^ g;
		
		p += sizeof(unsigned long);
		len -= sizeof(unsigned long);
	    }
	}

	UniformStream stream(0, h);

	histogram->setValue(stream());

	::delete [] buffer;
    }
}

void random2 ()
{
    for (int i = 0; i < iterations; i++)
    {
	char* buffer = generateID();

	unsigned long g, h = 0;
	const char *p = (const char *)buffer;
	size_t len = numbers*numberSize;

	if (buffer)
	{
	    while (len != 0)
	    {
		g = *(unsigned long*) p;
		
		h = h ^ g;
		
		p += sizeof(unsigned long);
		len -= sizeof(unsigned long);
	    }
	}

	long seed = (long) h;

	if (seed%2 == 0)  // make sure seed is odd
	    seed++;

	UniformStream stream(0, h, 0, seed, 1878892440);

	histogram->setValue(stream());

	::delete [] buffer;	
    }
}

void knuth ()
{
    for (int i = 0; i < iterations; i++)
    {
	char* buffer = generateID();

	unsigned long g, h = 0;
	const char *p = (const char *)buffer;
	size_t len = numbers*numberSize;

	if (buffer)
	{
	    while (len-- > 0)
	    {
		h = (h << 4) + (*p);
		if ((g = h & 0xf0000000))
		{
		    h = h ^ (g >> 24);
		    h = h ^ g;
		}
		p++;
	    }
	}
	
	histogram->setValue(h);

	::delete [] buffer;
    }
}


int main (int argc, char** argv)
{
    int entrySize = 32;     // bits
    HashType type = KNUTH;
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-bits") == 0)
	    entrySize = ::atoi(argv[i+1]);
	if (::strcmp(argv[i], "-knuth") == 0)
	    type = KNUTH;
	if (::strcmp(argv[i], "-random1") == 0)
	    type = RANDOM1;
	if (::strcmp(argv[i], "-random2") == 0)
	    type = RANDOM2;
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " [-bits <size>] [-random1] [-random2] [-knuth] [-help]" << endl;
	    return 0;
	}
    }

    /*
     * Given the number of bits in the entry, work out
     * the number of random numbers to take.
     */

    numbers = (int) ceil((double) entrySize/(double) numberSize);
    histogram = new PrecisionHistogram();
    dataStream = new UniformStream(0, 1000000L);

    switch (type)
    {
    case KNUTH:
	knuth();
	break;
    case RANDOM1:
	random1();
	break;
    case RANDOM2:
	random2();
	break;
    }

    print();

    delete histogram;

    return 0;
}
