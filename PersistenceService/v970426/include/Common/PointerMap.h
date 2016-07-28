/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PointerMap.h,v 1.4 1994/12/09 11:38:19 ngdp Exp $
 */

#ifndef POINTERMAP_H_
#define POINTERMAP_H_ 

#ifndef STDDEF_H_
#  include <os/stddef.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

class Buffer;

static const int MAP_TBL_SIZE = 16;

struct MapEntry
{
    Uint32 signature;
    const void *ptr;
};

class PointerMap
{
public:
    /* Constructors & destructor */
    
    PointerMap (size_t initSize = MAP_TBL_SIZE);
    PointerMap (const PointerMap&);
    ~PointerMap ();

    /* non-virtual member functions and operators */
    
    Boolean addPtr (const void *ptr, Int32& ptrIndex, Boolean = FALSE,
		    Uint32 = 0);    
    void clearMap (Boolean = FALSE);
    
    Boolean pack (Buffer&) const;
    Boolean unpack(Buffer&);

    PointerMap& operator= (const PointerMap&);
    const void *operator[] (Int32 index) const;
    
private:
    /* private member functions */

    Boolean mapPtr (const void *ptr, Int32& index, Boolean, Uint32);
    Boolean sizeup (Int32 newSize);
    
    Int32 mapEntries;
    Int32 mapSize;
    MapEntry *ptrMapping;
};

#endif
