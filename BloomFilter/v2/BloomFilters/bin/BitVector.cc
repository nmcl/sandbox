/*
 * Copyright (C) 1997, 1998,
 *
 * Department of Computing Science,
 * University of Newcastle upon Tyne,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BitVector.cc,v 1.4 1998/07/23 10:47:58 nmcl Exp $
 */

#include <math.h>
#include <iostream.h>
#include <string.h>
#include <values.h>
#include <stdlib.h>
#include <strstream.h>
#include <sys/time.h>

#include <ClassLib/Random.h>
#include <Stat/Variance.h>

#define TRUE 1
#define FALSE 0

enum HashType { PJW, RANDOM1, RANDOM2 };

const int numberSize = sizeof(double)*8;  // in bits

Variance v;
UniformStream* dataStream = 0;
char* bitVector = 0;
int numbers = 0;
int iterations = 50000;
int bitsToSet = 5;
unsigned long numberBitsSet = 0;
unsigned long vectorSize = 288540;    // bits
unsigned long vectorByte = 36068;     // bytes
unsigned long falseHits = 0;

static char bitMasks[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };
static const int mFactor = 4;

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
    unsigned long oneCount = 0;
    
    for (int i = 0; i < vectorByte; i++)
    {
	for (int j = 0; j < 8; j++)
	{
	    if (bitVector[i] & bitMasks[j])
	    {
#ifdef GRAPHIC
		cout << "1";
#endif		
		oneCount++;
	    }
	    else
	    {
#ifdef GRAPHIC		
		cout << "0";
#endif		
	    }
	}
    }

    cout << "\nbits set: " << numberBitsSet << endl;
    cout << "zeroCount: " << vectorSize-oneCount << endl;
    cout << "oneCount: " << oneCount << endl;
    cout << "false hits: " << falseHits << endl;
}

Boolean testBit (unsigned long key)
{
    unsigned long bitToSet = key;
    unsigned long overflow = 0;
    
    if (bitToSet > vectorSize)
    {
	overflow = bitToSet/vectorSize;
	bitToSet = bitToSet%vectorSize;
    }

    unsigned long byteNumber = bitToSet/8;
    
    if ((byteNumber != 0) && (byteNumber%8 == 0))
	byteNumber--;

    /*
     * Odd or even?
     */

    if ((byteNumber != 0) && (bitToSet/2 != 0))    
    	byteNumber = vectorByte - byteNumber;

    char byteMask = bitMasks[(bitToSet+overflow)%8];

    if (bitVector[byteNumber] & byteMask)
	return TRUE;
    else
	return FALSE;
}

void setBit (unsigned long key)
{
    numberBitsSet++;

    /*
     * Algorithm: simply mod the key by the
     * size of the vector and set that bit.
     *
     * If the number is odd, then work out way
     * down the vector from right to left, otherwise
     * left to right.
     */

    unsigned long bitToSet = key;
    unsigned long overflow = 0;
    
    if (bitToSet > vectorSize)
    {
	overflow = bitToSet/vectorSize;
	bitToSet = bitToSet%vectorSize;
    }

    unsigned long byteNumber = bitToSet/8;
    
    if ((byteNumber != 0) && (byteNumber%8 == 0))
	byteNumber--;

    /*
     * Odd or even?
     */

    if ((byteNumber != 0) && (bitToSet/2 != 0))
    	byteNumber = vectorByte - byteNumber;

    char byteMask = bitMasks[(bitToSet+overflow)%8];

    bitVector[byteNumber] |= byteMask;
}

void random1 ()
{
    int i;
    
    for (i = 0; i < iterations; i++)
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
	
	unsigned long loRange = 0;
	unsigned long hiRange = (((key == 0) || (key > MAXLONG/mFactor)) ? MAXLONG : key*mFactor);
	UniformStream stream(loRange, hiRange);
	
	for (int j = 0; j < bitsToSet; j++)
	{
	    unsigned long value = stream();
	    
	    setBit(value);
	}
    }

    for (i = 0; i < iterations; i++)
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

	unsigned long loRange = 0;
	unsigned long hiRange = (((key == 0) || (key > MAXLONG/mFactor)) ? MAXLONG : key*mFactor);
	UniformStream stream(loRange, hiRange);
	unsigned long hits = 0;
	
	for (int j = 0; j < bitsToSet; j++)
	{
	    unsigned long value = stream();

	    if (testBit(value))
		hits++;
	}

	if (hits == bitsToSet)
	    falseHits++;

	::delete [] buffer;
    }    
}

void random2 ()
{
    int i;
    
    for (i = 0; i < iterations; i++)
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

	unsigned long loRange = 0;
	unsigned long hiRange = (((key == 0) || (key > MAXLONG/mFactor)) ? MAXLONG : key*mFactor);
	UniformStream stream(loRange, hiRange, 0, seed, 1878892440);
	
	for (int j = 0; j < bitsToSet; j++)
	{
	    unsigned long value = stream();

	    setBit(value);
	}
    }

    for (i = 0; i < iterations; i++)
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

	unsigned long loRange = 0;
	unsigned long hiRange = (((key == 0) || (key > MAXLONG/mFactor)) ? MAXLONG : key*mFactor);
	UniformStream stream(loRange, hiRange, 0, seed, 1878892440);
	unsigned long hits = 0;
	
	for (int j = 0; j < bitsToSet; j++)
	{
	    unsigned long value = stream();

	    if (testBit(value))
		hits++;
	}

	if (hits == bitsToSet)
	    falseHits++;

	::delete [] buffer;	
    }    
}

void pjw ()
{
    int i;
    
    for (i = 0; i < iterations; i++)
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
    
	/*
	 * Set the first bit using h and then
	 * if we need more use a random number
	 * generator with h as the seed.
	 */

	unsigned long loRange = 0;
	unsigned long hiRange = (((key == 0) || (key > MAXLONG/mFactor)) ? MAXLONG : key*mFactor);
	UniformStream stream(loRange, hiRange);
	unsigned long value = h;
	
	for (int j = 0; j < bitsToSet; j++)
	{
	    if (j > 0)
		value = stream();

	    setBit(value);
	}
    }

    for (i = 0; i < iterations; i++)
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
    
	/*
	 * Set the first bit using h and then
	 * if we need more use a random number
	 * generator with h as the seed.
	 */

	unsigned long loRange = 0;
	unsigned long hiRange = (((key == 0) || (key > MAXLONG/mFactor)) ? MAXLONG : key*mFactor);
	UniformStream stream(loRange, hiRange);	
	unsigned long hits = 0;
	unsigned long value = h;
	
	for (int j = 0; j < bitsToSet; j++)
	{
	    if (j > 0)
		value = stream();

	    if (testBit(value))
		hits++;
	}

	if (hits == bitsToSet)
	    falseHits++;
	
	::delete [] buffer;
    }    
}

/*
 * This program creates a bit vector of the specified size and then
 * uses one of the hash functions to populate it with the specified
 * number of hash values.
 *
 * It does this 1000 times and computes statistics about the number
 * of false hits obtained.
 */

int main (int argc, char** argv)
{
    int entrySize = 19;     // bits
    HashType type = PJW;
    
    for (int i = 0; i < argc; i++)
    {
	if (::strcmp(argv[i], "-bitsize") == 0)
	    entrySize = ::atoi(argv[i+1]);
	if (::strcmp(argv[i], "-set") == 0)
	    bitsToSet = ::atoi(argv[i+1]);
	if (::strcmp(argv[i], "-entries") == 0)
	    iterations = ::atol(argv[i+1]);
	if (::strcmp(argv[i], "-table") == 0)
	    vectorSize = ::atoi(argv[i+1]);
	if (::strcmp(argv[i], "-pjw") == 0)
	    type = PJW;
	if (::strcmp(argv[i], "-random1") == 0)
	    type = RANDOM1;
	if (::strcmp(argv[i], "-random2") == 0)
	    type = RANDOM2;
	if (::strcmp(argv[i], "-help") == 0)
	{
	    cout << "Usage: " << argv[0] << " [-bitsize <size>] [-set <number>] [-entries <number>] [-table <size>] [-random1] [-random2] [-pjw] [-help]" << endl;
	    return 0;
	}
    }

    vectorByte = ceil(vectorSize/8);

    struct timeval stime;

    ::gettimeofday(&stime, NULL);
    
    dataStream = new UniformStream(0, stime.tv_usec);
    
    for (int k = 0; k < 1000; k++)
    {
	cout << k << endl;
	
	bitVector = ::new char[vectorByte];

	::memset(bitVector, '\0', vectorByte);
    
	/*
	 * Given the number of bits in the entry, work out
	 * the number of random numbers to take.
	 */

	numbers = (int) ceil((double) entrySize/(double) numberSize);
	numberBitsSet = 0;
    
	switch (type)
	{
	case PJW:
	    pjw();
	    break;
	case RANDOM1:
	    random1();
	    break;
	case RANDOM2:
	    random2();
	    break;
	}

	v += falseHits;

	print();
    
	falseHits = 0;
	::delete [] bitVector;
    
	cout << "Mean: " << v.mean() << endl;
	cout << "Max: " << v.max() << endl;
	cout << "Min: " << v.min() << endl;
	cout << "Variance: " << v.variance() << endl;
	cout << "StdDev: " << v.stdDev() << "\n" << endl;
    }
    
    return 0;
}
