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
 * $Id: UidHash.cc,v 1.1 1991/08/15 19:55:48 nsmw Exp $
 *
 */

// #include <Arjuna/Buffer.h>

#include "UidHash.h"

UidHash::UidHash (unsigned long hSeed)
{
    hashSeed = hSeed;
}

UidHash::~UidHash ()
{
}

unsigned long UidHash::Hash(Uid entry)
{
    return hashSeed ^ entry.hash(0xffffffff);
}
