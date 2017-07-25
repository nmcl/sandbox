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

#ifndef _UIDHASH_H_
#define _UIDHASH_H_

/*
 * $Id: UidHash.h,v 1.2 1991/08/15 19:53:37 nsmw Exp $
 *
 */

#include <Arjuna/Uid.h>
// #include <Arjuna/Buffer.h>

class UidHash
{
public:
    UidHash (unsigned long hSeed);
    ~UidHash ();

    unsigned long Hash(Uid name);

//  bool pack(Buffer& buff) const;
//  bool unpack(Buffer& buff);
private:
    unsigned long hashSeed;
};

#endif  _UIDHASH_H_
