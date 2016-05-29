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

#ifndef _UIDBLOOMFILTER_H_
#define _UIDBLOOMFILTER_H_

/*
 * $Id: UidBloomFilter.h,v 1.2 1991/08/15 19:53:36 nsmw Exp $
 *
 */

#include <Std/boolean.h>
#include "BloomFilter.h"
#include <Arjuna/Uid.h>
// #include <Arjuna/Buffer.h>

#include "UidHash.h"

class UidBloomFilter : public BloomFilter
{
public:
    UidBloomFilter (unsigned long numBits, unsigned long numOfHashes);
    ~UidBloomFilter ();

    void AddEntry(Uid entry);
    bool IsEntry(Uid entry);

//  bool pack(Buffer& buff) const;
//  bool unpack(Buffer& buff);
private:

    unsigned long numberOfHashes;
    UidHash**     hashes;
};

#endif  _UIDBLOOMFILTER_H_
