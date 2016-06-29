/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PointerMap.cc,v 1.1 1997/06/09 19:51:47 nmcl Exp $
 */


#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef PONTERMAP_H_
#  include <Common/PointerMap.h>
#endif

static const char RCSid[] = "$Id: PointerMap.cc,v 1.1 1997/06/09 19:51:47 nmcl Exp $";

/* 
 * Public constructors and destructors
 */

/*
 * The following constructor is provided for creating an empty PointerMap.
 * The argument may be used to define the amount of memory allocated
 * each time extra memory is required - it defaults to the value
 * in PointerMap.h
 * Since sizeup ensures that ptrMapping[size] is valid we reduce
 * the initial value by one for the 0 start of C++ arrays
 */

PointerMap::PointerMap ( size_t initSize )
		       : mapEntries(0),
			 mapSize(0),
			 ptrMapping(0)
{
    if (initSize <= 0)
        initSize = MAP_TBL_SIZE;

    if (sizeup(initSize-1))
    {
	mapEntries = 1;
	ptrMapping[0].ptr = 0;	/* reserved for the null pointer */
	ptrMapping[0].signature = 0;
    }
}
  
PointerMap::PointerMap ( const PointerMap& copyFrom )
		       : mapEntries(0),
			 mapSize(0),
			 ptrMapping(0)
{
    ptrMapping = new MapEntry [copyFrom.mapSize];
	
    if (ptrMapping != 0)
    {
	mapEntries = copyFrom.mapEntries;
	mapSize = copyFrom.mapSize;
	
	for (int i = 0; i < mapEntries; i++)
	    ptrMapping[i] = copyFrom.ptrMapping[i];

    }
}
     
PointerMap::~PointerMap ()
{
    if (ptrMapping != 0)
    {
	delete [] ptrMapping;
	mapEntries = 0;
	mapSize = 0;
    }
}

/* 
 * Public non-virtual member functions and operators
 */

/*
 * These routines are used by Buffer to handle pointer aliasing. Each pointer
 * map maintains a list of pointers seen to date.
 * If the index is initially +ve a search of the existing map is made.
 * If this returns TRUE the pointer already exists and index has been set
 * If it returns FALSE a +ve index implies a new entry a -ve index is an error
 * If index is inititally -ve then
 * the pointer is assigned to the complementary slot in the table. This is
 * used during unpacking to ensure that unpacked pointers are placed in the
 * correct table slot.
 * If replace is TRUE we override any old entry (replacing it by 0) and add
 * a new one.
 */

Boolean PointerMap::addPtr ( const void *ptr, Int32& index,
			     Boolean replace, Uint32 signature )
{
    Boolean added = FALSE;

    if (index >= 0)
    {
	if (!mapPtr(ptr,index, replace,signature) && (index >= 0))
	{
	    added = TRUE;
	}
    }    
    else
	if (sizeup(-index))
	{
	    Int32 i = -index;
	    
	    added = TRUE;
	    
	    if (i >= mapEntries)
	    {
		for (Int32 j = mapEntries; j < i; j++)
		{
		    ptrMapping[j].ptr = 0;
		    ptrMapping[j].signature = 0;
		}
		
		mapEntries = i+1;
	    }
	    ptrMapping[i].ptr = ptr;	/* remember it */
	    ptrMapping[i].signature = signature;
	}
    
    return added;
}

/*
 * Clear the mapping table. If the argument is TRUE we zero the whole
 * thing, otherwise we only reset the number of entries to 1 and rely on
 * mapPtr to clear entries lazily as it needs them.
 * Remember that entry 0 is reserved for the null pointer - hence the reset to 1
 */

void PointerMap::clearMap ( Boolean clearAll )
{
    if (clearAll)
    {
	for (int i = 1; i < mapSize; i++)
	{
	    ptrMapping[i].ptr = 0;
	    ptrMapping[i].signature = 0;
	}
	
    }

    mapEntries = 1;
}

Boolean PointerMap::pack ( Buffer& packInto ) const
{
    Boolean res = FALSE;

    if (packInto.pack(mapEntries) && (mapEntries > 0))
    {
	res = packInto.packMappedBlock((char *)ptrMapping, (size_t)(mapEntries*sizeof(MapEntry)));
    }
    
    return res;
}

Boolean PointerMap::unpack ( Buffer& unpackFrom )
{
    Int32 newEntries = 0;
    
    Boolean res = unpackFrom.unpack(newEntries);

    if (res)
    {
	Int32 oldEntries = mapEntries;
    
	/* Prevent sizeup doing any copying of old entries */
	
	mapEntries = 0;
	
	if(sizeup(newEntries))
	{
	    mapEntries = newEntries;
	    if (newEntries > 0)
	    {
		size_t size =  newEntries*sizeof(MapEntry);
		void *p = ptrMapping;
		res = unpackFrom.unpack(p, size);
	    }
	}
	else
	    mapEntries = oldEntries;
    }
	
    return res;
}

PointerMap& PointerMap::operator= ( const PointerMap& copyFrom )
{
    /* catch s = s */
    
    if (this == &copyFrom)
	return *this;

    /* free existing map if it exists */
    
    if (ptrMapping != 0)
	delete [] ptrMapping;

    mapSize = copyFrom.mapSize;
    mapEntries = copyFrom.mapEntries;
    
    ptrMapping = new MapEntry[copyFrom.mapSize];
	
    if (ptrMapping != 0)
    {
	for (int i = 0; i < copyFrom.mapEntries; i++)
	    ptrMapping[i] = copyFrom.ptrMapping[i];

    }

    return *this;
}

const void *PointerMap::operator[] ( Int32 index ) const
{
    if (index < mapEntries)
	return ptrMapping[index].ptr;
    else
	return 0;
}

/*
 * returns index of slot containing pointer. Assigns if needed
 * If the replace flag is TRUE we add regardless and overwrite the old
 * index entry with 0. This is not a problem since slot 0 is always
 * reserved for the null pointer
 */

Boolean PointerMap::mapPtr ( const void *p, Int32& index, Boolean replace,
			     Uint32 check )
{
    index = 0;

    while (index < mapEntries)
    {
	if ((ptrMapping[index].ptr == p) &&
	    ((ptrMapping[index].signature == check)))
	{
	    /* never replace the null pointer */
	    
	    if (replace && p != 0)
	    {
		/* clear out old values */
		
		ptrMapping[index].ptr = 0;
		ptrMapping[index].signature = 0;
	    }
	    
	    else
		return TRUE;
	}
	
	index++;
    }

    if ((index < mapSize) ||
	(sizeup(mapSize)))
    {
	ptrMapping[index].ptr = p;
	ptrMapping[index].signature = check;
	mapEntries++;
    }
    else
	index = -1;
	 
    return FALSE;
}

/*
 * If map table is full allocate a new one and copy entries.
 * This will be at least as big as the requested size and probably bigger
 * We ensure here that ptrMapping[newSize] exists
 */
 
Boolean PointerMap::sizeup ( Int32 newSize )
{
    Boolean ok = TRUE;

    if (newSize < 0)
	return FALSE;
    
    if (newSize >= mapSize)
    {
	Int32 newMapSize = (mapSize <= 0 ? 1 : mapSize);
	
	while (newMapSize <= newSize)
	    newMapSize *=2;
	
	MapEntry *temp = new MapEntry[newMapSize];

	if (temp != 0)
	{
	    for (int i = 0; i < mapEntries; i++)
		temp[i] = ptrMapping[i];
	
	    delete [] ptrMapping;
	    ptrMapping = temp;
	    mapSize = newMapSize;
	} else
	    ok = FALSE;
    }

    return ok;
}
