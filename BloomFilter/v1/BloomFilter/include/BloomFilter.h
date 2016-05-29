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

#ifndef _BLOOMFILTER_H_
#define _BLOOMFILTER_H_

/*
 * $Id: BloomFilter.h,v 1.2 1991/08/15 19:53:32 nsmw Exp $
 *
 */

#include <Std/boolean.h>
// #include <Arjuna/Buffer.h>

class BloomFilter
{
public:
    BloomFilter (unsigned long numBits);
    ~BloomFilter ();

    unsigned long NumberOfBits();
    float ProportionSet();

//  bool pack(Buffer& buff) const;
//  bool unpack(Buffer& buff);
protected:

    void SetBit(unsigned long bitNum);
    bool TestBit(unsigned long bitNum);

private:
    unsigned long numberOfBits;
    unsigned long numberOfBitsSet;
    unsigned long numberOfBytes;
    char*         bloomFilterBits;
};

#endif  _BLOOMFILTER_H_
