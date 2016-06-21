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
 * $Id: StringBloomFilter.cc,v 1.1 1991/08/15 19:55:42 nsmw Exp $
 *
 */

#define RAND(x) ((x * 1103515245) + 12345)

#include "StringBloomFilter.h"

StringBloomFilter::StringBloomFilter (unsigned long numBits, unsigned long numOfHashes) : BloomFilter (numBits)
{
    hashes          = 0;

    numberOfHashes = numOfHashes;

    hashes         = new StringHash*[numberOfHashes];
    for (int index = 0; index < numberOfHashes; index++)
	hashes[index] = new StringHash(RAND(index));    
}

StringBloomFilter::~StringBloomFilter ()
{
    if (hashes != 0)
    {
	for (int index = 0; index < numberOfHashes; index++)
	    if (hashes[index] != 0)
		delete hashes[index];
	delete hashes;
    }
}

void StringBloomFilter::AddEntry(char* name)
{
    for (int index = 0; index < numberOfHashes; index++)
	SetBit(hashes[index]->Hash(name) % NumberOfBits());
}

bool StringBloomFilter::IsEntry(char* name)
{
    bool res = true;

    for (int index = 0; (index < numberOfHashes) && res; index++)
	res = TestBit(hashes[index]->Hash(name) % NumberOfBits());

    return res;
}
