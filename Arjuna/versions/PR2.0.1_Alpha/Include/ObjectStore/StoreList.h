/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * StoreList.h,v
 */

#ifndef STORELIST_H_
#define STORELIST_H_

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

// @NoRemote, @NoMarshall
class StoreList
{
public:
    StoreList ();
    ~StoreList ();
    
    unsigned long useCount;
    ObjectStore* instance;
    
    StoreList* next;
};

#endif
