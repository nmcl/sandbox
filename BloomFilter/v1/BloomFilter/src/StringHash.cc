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
 * $Id: StringHash.cc,v 1.1 1991/08/15 19:55:45 nsmw Exp $
 *
 */

#include "StringHash.h"

StringHash::StringHash (unsigned long hSeed)
{
    hashSeed = hSeed;
}

StringHash::~StringHash ()
{
}

unsigned long StringHash::Hash(char* entry)
{
    register char*         cp = entry;
    register unsigned long temp;
    register unsigned long hashVal = hashSeed;

    while (*cp != '\0')
    {
        hashVal = (hashVal << 4) + (*cp);
        if (temp == (hashVal & 0xf0000000))
        {
            hashVal = hashVal ^ (temp >> 24);
            hashVal = hashVal ^ temp;
        }
        cp++;
    }
    return hashVal;
}
