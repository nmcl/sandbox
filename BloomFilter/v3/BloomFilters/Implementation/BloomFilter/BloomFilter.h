/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BloomFilter.h,v 1.1 1998/06/02 16:51:16 nmcl Exp $
 */

#ifndef BLOOMFILTER_H_
#define BLOOMFILTER_H_

#include <System/stdlib.h>
#include <System/sys/types.h>

#include <Common/Uid.h>
#include <Common/Boolean.h>

class ostream;

class BloomFilter
{
public:
    BloomFilter (size_t size, unsigned short numberOfHash);  // size in bits
    virtual ~BloomFilter ();
    
    void insert (const Uid& name);
    Boolean present (const Uid& name) const;

    ostream& print (ostream& strm) const;

protected:
    virtual Boolean testBit (unsigned long key) const;
    virtual void setBit (unsigned long key);
    
    char* _bitVector;
    size_t _vectorSize;
    size_t _vectorSizeInBytes;
    unsigned short _hashNumbers;
    
private:
    unsigned long hashKey (const Uid& u) const;
};

extern ostream& operator<< (ostream& strm, const BloomFilter& b);

#endif
