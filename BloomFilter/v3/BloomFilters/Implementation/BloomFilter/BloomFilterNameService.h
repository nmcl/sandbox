/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BloomFilterNameService.h,v 1.1 1998/06/02 16:51:17 nmcl Exp $
 */

#ifndef BLOOMFILTERNAMESERVICE_H_
#define BLOOMFILTERNAMESERVICE_H_

#include <System/stdlib.h>
#include <System/sys/types.h>

#include <Common/Uid.h>
#include <Common/Boolean.h>

#include "BloomFilter.h"

class ostream;
class FilterContext;

class BloomFilterNameService
{
public:
    BloomFilterNameService (size_t size);
    ~BloomFilterNameService ();
    
    Boolean insert (BloomFilter* filter, const char* ior);

    Boolean present (const Uid& objId) const;
    
    char** locate (const Uid& objId, int& size) const;

private:
    FilterContext** _filters;
    size_t _size;
    size_t _index;
};

#endif
