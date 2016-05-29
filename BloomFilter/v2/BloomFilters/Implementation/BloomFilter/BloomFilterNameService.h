/*
 * Copyright (C) 1998,
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: BloomFilterNameService.h,v 1.3 1998/07/23 10:47:56 nmcl Exp $
 */

#ifndef BLOOMFILTERNAMESERVICE_H_
#define BLOOMFILTERNAMESERVICE_H_

#include <stdlib.h>
#include <sys/types.h>

#include <Common/Uid.h>
#include <Common/Boolean.h>

class ostream;
class FilterContext;

class BloomFilterNameService
{
public:
    BloomFilterNameService (long nodes, long vectorSize, int numberOfHash);
    virtual ~BloomFilterNameService ();

    void reset ();
    
    Boolean addDomain (const char* ior, int& domainNumber);
    Boolean removeDomain (int domainNumber);
    
    Boolean insert (const Uid& objId, int domainNumber);
    Boolean present (const Uid& objId) const;

    /*
     * Returns an array (size specified) of possible hits. It
     * is the callers responsibility to delete the array.
     */
    
    char** locate (const Uid& objId, int& size) const;

    ostream& print (ostream& strm) const;
    
private:
    FilterContext** _filters;
    long _size;
    long _vectorSize;
    int _numberOfHash;
};

extern ostream& operator<< (ostream& strm, const BloomFilterNameService& b);

#endif
