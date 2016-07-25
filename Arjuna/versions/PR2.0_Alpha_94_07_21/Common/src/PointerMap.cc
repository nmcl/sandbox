/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PointerMap.cc,v 1.1 1994/03/21 10:10:14 ngdp Exp $
 */


#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef PONTERMAP_H_
#  include <Common/PointerMap.h>
#endif

static const char RCSid[] = "$Id: PointerMap.cc,v 1.1 1994/03/21 10:10:14 ngdp Exp $";

/* 
 * Public constructors and destructors
 */

/*
 * The following constructor is provided for creating an empty PointerMap.
 * The argument may be used to define the amount of memory allocated
 * each time extra memory is required - it defaults to the value
 * in PointerMap.h 
 */

PointerMap::PointerMap ( size_t initSize )
		       : mapEntries(0),
			 mapSize(0),
			 ptrMapping(0)
{
    if (sizeup(initSize))
    {
	mapEntries = 1;
	ptrMapping[0] = 0;		/* reserved for the null pointer */
    }
}
  
PointerMap::PointerMap ( const PointerMap& copyFrom )
		       : mapEntries(0),
			 mapSize(0),
			 ptrMapping(0)
{
    ptrMapping = new const void *[mapSize];
	
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
 */

Boolean PointerMap::addPtr ( const void *ptr, long& index )
{
    Boolean added = FALSE;

    if (index >= 0)
    {
	if (!mapPtr(ptr,index) && (index >= 0))
	{
	    added = TRUE;
	}
    }    
    else
	if (sizeup(-index))
	{
	    long i = -index;
	    
	    added = TRUE;
	    
	    if (i >= mapEntries)
	    {
		for (int j = mapEntries; j < i; j++)
		    ptrMapping[j] = 0;
		
		mapEntries = i+1;
	    }
	    ptrMapping[i] = ptr;	/* remember it */
	}
    
    return added;
}

/*
 * Clear the mapping table. If the argument is TRUE we zero the whole
 * thing, otherwise we only reset the number of entries to 0 and rely on
 * mapPtr to clear entries lazily as it needs them.
 */

void PointerMap::clearMap ( Boolean clearAll )
{
    if (clearAll)
    {
	for (int i = 0; i < mapSize; i++)
	    ptrMapping[i] = 0;
    }

    mapEntries = 0;
}

Boolean PointerMap::pack ( Buffer& packInto ) const
{
    Boolean res = FALSE;

    if (packInto.pack(mapEntries) && (mapEntries > 0))
    {
	res = packInto.pack((char *)*ptrMapping, (size_t)(mapEntries*sizeof(*ptrMapping)));
    }
    
    return res;
}

Boolean PointerMap::unpack ( Buffer& unpackFrom )
{
    Boolean res = (unpackFrom.unpack(mapEntries) &&  sizeup(mapEntries));
    size_t size =  mapEntries*sizeof(*ptrMapping);
    char *p = (char *)*ptrMapping;
    
    
    if (res && (mapEntries > 0))
	res = unpackFrom.unpack(p, size);
    
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
    
    ptrMapping = new const void *[copyFrom.mapSize];
	
    if (ptrMapping != 0)
    {
	for (int i = 0; i < copyFrom.mapEntries; i++)
	    ptrMapping[i] = copyFrom.ptrMapping[i];

    }

    return *this;
}

const void *PointerMap::operator[] ( long index ) const
{
    if (index < mapEntries)
	return ptrMapping[index];
    else
	return 0;
}

/*
 * returns index of slot containing pointer. Assigns if needs to
 */

Boolean PointerMap::mapPtr ( const void *p, long& index )
{
    index = 0;

    while (index < mapEntries)
    {
	if (ptrMapping[index] == p)
	    return TRUE;
	index++;
    }

    if ((index < mapSize) ||
	(sizeup((mapSize+1) * 2)))
    {
	ptrMapping[index] = p;
	mapEntries++;
    }
    else
	index = -1;
	 
    return FALSE;
}

/*
 * If map table is full allocate a bigger one, and 
 * copy old entries
 */
 
Boolean PointerMap::sizeup ( long newSize )
{
    Boolean ok = TRUE;
    
    if (newSize > mapSize)
    {
	const void **temp = new const void *[newSize];

	if (temp != 0)
	{
	    for (int i = 0; i < mapEntries; i++)
		temp[i] = ptrMapping[i];
	
	    delete [] ptrMapping;
	    ptrMapping = temp;
	    mapSize = newSize;
	} else
	    ok = FALSE;
    }

    return ok;
}
