/*
 * Copyright 1990, 1991,
 * Computing Laboratory, University of Newcastle upon Tyne, UK.
 *
 * Permission to use, copy, modify and distribute this program for
 * evaluation, teaching and/or research purposes only and without fee is
 * hereby granted, providing that this copyright and permission notice
 * appear on all copies and supporting documentation, and that similar
 * conditions are imposed on any individual or organization to whom the
 * program is distributed. The University of Newcastle upon Tyne makes no
 * representation about the suitability of this software for any purpose.
 * It is provided "as is" without express or implied warranty.
 */

/*
 * $Id: BloomFilter.cc,v 1.1 1991/08/15 19:55:40 nsmw Exp $
 *
 */

static char bitMasks[] = { 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80 };

#include <Arjuna/commondefs.h>
#include "BloomFilter.h"

BloomFilter::BloomFilter (unsigned long numBits)
{
    bloomFilterBits = NILPTR(char);
    numberOfBits    = numBits;
    numberOfBytes   = (numberOfBits + 7) / 8;
    numberOfBitsSet = 0;

    bloomFilterBits = new char[numberOfBytes];

    for (int index1 = 0; index1 < numberOfBytes; index1++)
	bloomFilterBits[index1] = 0;
}

BloomFilter::~BloomFilter ()
{
    if (bloomFilterBits != NILPTR(char))
	delete bloomFilterBits;
}

unsigned long BloomFilter::NumberOfBits()
{
    return numberOfBits;
}

float BloomFilter::ProportionSet()
{
    return (float) numberOfBitsSet / numberOfBits;
}

void BloomFilter::SetBit(unsigned long bitNum)
{
    char          bitMask    = bitMasks[bitNum & 0x07];
    unsigned long byteNumber = (bitNum >> 0x03);

    if ((bloomFilterBits[byteNumber] & bitMask) == 0)
    {
	numberOfBitsSet++;
	bloomFilterBits[byteNumber] |= bitMask;
    }
}

bool BloomFilter::TestBit(unsigned long bitNum)
{
    char          bitMask    = bitMasks[bitNum & 0x07];
    unsigned long byteNumber = (bitNum >> 0x03);

    return bool (! ((bloomFilterBits[byteNumber] & bitMask) == 0));
}
