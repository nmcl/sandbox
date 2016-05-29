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
 * $Id: UidBloomFilter.cc,v 1.1 1991/08/15 19:55:47 nsmw Exp $
 *
 */

#define RAND(x) ((x * 1103515245) + 12345)

#include <Arjuna/commondefs.h>
#include "UidBloomFilter.h"

UidBloomFilter::UidBloomFilter (unsigned long numBits, unsigned long numOfHashes) : BloomFilter (numBits)
{
    hashes          = NILPTR(UidHash*);

    numberOfHashes = numOfHashes;

    hashes         = new UidHash*[numberOfHashes];
    for (int index = 0; index < numberOfHashes; index++)
	hashes[index] = new UidHash(RAND(index));    
}

UidBloomFilter::~UidBloomFilter ()
{
    if (hashes != NILPTR(UidHash*))
    {
	for (int index = 0; index < numberOfHashes; index++)
	    if (hashes[index] != NILPTR(UidHash))
		delete hashes[index];
	delete hashes;
    }
}

void UidBloomFilter::AddEntry(Uid entry)
{
    for (int index = 0; index < numberOfHashes; index++)
	SetBit(hashes[index]->Hash(entry) % NumberOfBits());
}

bool UidBloomFilter::IsEntry(Uid entry)
{
    bool res = TRUE;

    for (int index = 0; (index < numberOfHashes) && res; index++)
	res = TestBit(hashes[index]->Hash(entry) % NumberOfBits());

    return res;
}
