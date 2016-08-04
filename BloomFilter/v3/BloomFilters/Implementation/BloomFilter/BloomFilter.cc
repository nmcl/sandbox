/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BloomFilter.cc,v 1.1 1998/06/02 16:51:15 nmcl Exp $
 */

#include <System/iostream.h>
#include <System/string.h>
#include <System/math.h>

#include <ClassLib/Random.h>

#ifndef BLOOMFILTER_H_
#  include "BloomFilter.h"
#endif

static char bitMasks[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

BloomFilter::BloomFilter (size_t size, unsigned short numberOfHash)
			 : _bitVector(::new char[ceil(size/8)]),
			   _vectorSize(size),
			   _vectorSizeInBytes(ceil(size/8)),
			   _hashNumbers(numberOfHash)
{
    ::memset(_bitVector, '\0', _vectorSizeInBytes);
}

BloomFilter::~BloomFilter ()
{
    ::delete [] _bitVector;
}
    
void BloomFilter::insert (const Uid& name)
{
    unsigned long key = hashKey(name);
    UniformStream stream(0, key*2, 0, key);

    for (int j = 0; j < _hashNumbers; j++)
    {
	setBit(key);
	
	key = stream();
    }
}

Boolean BloomFilter::present (const Uid& name) const
{
    unsigned long key = hashKey(name);
    UniformStream stream(0, key*2, 0, key);

    for (int j = 0; j < _hashNumbers; j++)
    {
	if (!testBit(key))
	    return FALSE;

	key = stream();
    }

    return TRUE;
}

Boolean BloomFilter::testBit (unsigned long key) const
{
    unsigned long bitToSet = key;
    unsigned long overflow = 0;
    
    if (bitToSet > _vectorSize)
    {
	overflow = bitToSet/_vectorSize;
	bitToSet = bitToSet%_vectorSize;
    }

    unsigned long byteNumber = bitToSet/8;

    if ((byteNumber != 0) && (byteNumber%8 == 0))
	byteNumber--;
    
    /*
     * Odd or even?
     */

    if ((byteNumber != 0) && (bitToSet/2 != 0))
    	byteNumber = _vectorSizeInBytes - byteNumber;

    char byteMask = bitMasks[(bitToSet+overflow)%8];

    if (_bitVector[byteNumber] & byteMask)
	return TRUE;
    else
	return FALSE;
}

void BloomFilter::setBit (unsigned long key)
{
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
    
    if (bitToSet > _vectorSize)
    {
	overflow = bitToSet/_vectorSize;
	bitToSet = bitToSet%_vectorSize;
    }

    unsigned long byteNumber = bitToSet/8;

    if ((byteNumber != 0) && (byteNumber%8 == 0))
	byteNumber--;

    /*
     * Odd or even?
     */

    if ((byteNumber != 0) && (bitToSet/2 != 0))
    	byteNumber = _vectorSizeInBytes - byteNumber;

    char byteMask = bitMasks[(bitToSet+overflow)%8];

    _bitVector[byteNumber] |= byteMask;
}

ostream& BloomFilter::print (ostream& strm) const
{
    unsigned long zeroCount = 0;
    unsigned long oneCount = 0;
    
    for (int i = 0; i < _vectorSizeInBytes; i++)
    {
	for (int j = 0; j < 8; j++)
	{
	    if (_bitVector[i] & bitMasks[j])
	    {
#ifdef GRAPHIC		
		strm << "1";
#endif		
		oneCount++;
	    }
	    else
	    {
#ifdef GRAPHIC		
		strm << "0";
#endif		
		zeroCount++;
	    }
	}
    }

    strm << "zeroCount: " << zeroCount << endl;
    strm << "oneCount: " << oneCount << endl;

    return strm;
}

unsigned long BloomFilter::hashKey (const Uid& u) const
{
    char* buffer = u.stringForm();
    unsigned long g, h = 0;
    const char *p = (const char *)buffer;
    size_t len = ::strlen(buffer);

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

    ::delete [] buffer;
    
    return h;
}

ostream& operator<< (ostream& strm, const BloomFilter& b)
{
    return b.print(strm);
}
