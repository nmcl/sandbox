/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BloomFilter.h,v 1.3 1998/07/23 10:47:54 nmcl Exp $
 */

#ifndef BLOOMFILTER_H_
#define BLOOMFILTER_H_

#include <stdlib.h>
#include <sys/types.h>

#include <Common/Uid.h>
#include <Common/Boolean.h>

class ostream;

class BloomFilter
{
public:
    BloomFilter (long vectorSize, int numberOfHash);  // size in bits
    virtual ~BloomFilter ();
    
    void insert (const Uid& name);
    Boolean present (const Uid& name) const;

    ostream& print (ostream& strm) const;

protected:
    virtual Boolean testBit (unsigned long key) const;
    virtual void setBit (unsigned long key);
    
    char* _bitVector;
    long _vectorSize;
    long _vectorSizeInBytes;
    int _hashNumbers;
    
private:
    unsigned long hashKey (const Uid& u) const;
};

extern ostream& operator<< (ostream& strm, const BloomFilter& b);

#endif
