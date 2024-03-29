/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FdCache.cc,v 1.14 1995/08/24 10:34:37 ngdp Exp $
 */

/*
 *
 * Simple file descriptor cache. Holds fds for recently opened files in an
 * attempt to speed up the Object Store which is bogged down by file opens
 *
 */ 

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef SYS_RESOURCE_H_
#  include <System/sys/resource.h>
#endif

#ifdef _AIX
#  ifndef LIMITS_H_
#    include <System/limits.h>
#  endif
#endif

#ifndef FDCACHE_H_
#  include <ObjectStore/FdCache.h>
#endif

#ifndef STATEDICT_H_
#  include <ObjectStore/StateDict.h>
#endif

/* The static instance of the cache for Unix process fds */

FdCache UnixFdCache;

FdCache::FdCache ()
                 : currCacheSize(0),
		   maxCacheSize(0),
		   fileCacheHead(0),
		   fileCacheTail(0)
{
#if defined(_AIX) || defined(__linux__)
    maxCacheSize = OPEN_MAX / 2;
#else
#ifdef HAVE_SYSCONF
    maxCacheSize = sysconf(_SC_OPEN_MAX);
#else    
    rlimit arg;
    ::getrlimit(RLIMIT_NOFILE, &arg);
    maxCacheSize = arg.rlim_cur / 2;
#endif
#endif
}

FdCache::~FdCache ()
{
    cacheEntry *temp = 0;
    
    while (fileCacheHead)
    {
	temp = fileCacheHead;
	fileCacheHead = fileCacheHead->next;
	delete temp;
    }
}

/* 
 * Scan cache looking for Uid. If found move entry to head of list and
 * return a pointer to the cache entry.
 */

#ifndef GCC_NESTED_TYPE_BUG
FdCache::cacheEntry *FdCache::scanCache ( const Uid& objUid )
#else
cacheEntry *FdCache::scanCache ( const Uid& objUid )
#endif
{
    cacheEntry *last = 0;
    cacheEntry *current = fileCacheHead;

    while (current)
    {
	if (current->cachedUid == objUid)
	{
	    /* Found - update and move */

	    if (current != fileCacheHead)
	    {
		if (current != fileCacheTail)
		{
		    /* moving entry in middle of list */
		    current->next->previous = last;
		}
		else
		{
		    /* moving last element */
		    fileCacheTail = last;
		}

		last->next = current->next;		
		current->next = fileCacheHead;
		current->previous = 0;
		fileCacheHead->previous = current;
		fileCacheHead = current;
	    }
	    
	    return current;
	}

	last = current;
	current = current->next;
    }

    return 0;
}

/*
 * Insert a cache entry. Add to front of list.
 * If cache is now too big (too many open files) try to clean up
 * by closing files (and setting fd to -1) but keep rest of entry for reuse
 * later
 */

#ifndef GCC_NESTED_TYPE_BUG
FdCache::cacheEntry *FdCache::enterIntoCache ( const Uid& objUid, const int fd )
#else
cacheEntry *FdCache::enterIntoCache ( const Uid& objUid, const int fd )
#endif
{
    cacheEntry *current = scanCache(objUid);

    if (current == 0)
    {
	/* Allocate new entry */
	current = new cacheEntry(objUid, fd);

	if (current == 0)
	    return 0;

	current->next = fileCacheHead;

	if (fileCacheHead)
	    fileCacheHead->previous = current;

	fileCacheHead = current;

	if (fileCacheTail == 0)
	    fileCacheTail = current;

	currCacheSize++;
    }
    else
    {
	if (current->storeFd == -1)
	    currCacheSize++;
	current->storeFd = fd;
    }
    
    
    if (currCacheSize > maxCacheSize)
    {
	compactCache();
    }

    return current;
}

Boolean FdCache::compactCache ()
{
    Boolean cacheCompacted = FALSE;
    cacheEntry *rear = fileCacheTail;
    cacheEntry *temp = 0;
    
    while ((rear) && 
	   (rear != fileCacheHead) &&
	   (currCacheSize > (maxCacheSize/2)))
    {
	temp = rear;
	rear = rear->previous;

	if (temp->storeFd != -1)
	{
	    ::close(temp->storeFd);
	    temp->storeFd = -1;
	    currCacheSize--;
	    cacheCompacted = TRUE;

	    if ((temp->cachedBlockPtr == 0) &&
		(temp->cachedDictPtr != 0))
	    {
		/* File has not been written to so drop the dictionary block also */
		
#ifdef GCC_DELETE_BUG
		delete temp->cachedDictptr;
#else
		delete [] temp->cachedDictPtr;
#endif
		temp->cachedDictPtr = 0;
	    }
	}
    }

    return cacheCompacted;
}

/*
 * Purge cache entry. Close associated file and release memory
 */

void FdCache::purgeFromCache ( const Uid& objUid )
{
    cacheEntry *last = 0;
    cacheEntry *current = fileCacheHead;
    
    while (current)
    {
	if (current->cachedUid == objUid)
	{
	    if (fileCacheTail == fileCacheHead)
	    {
		fileCacheTail = fileCacheHead = 0;
	    }
	    else
	    {
		if (current == fileCacheTail)
		{
		    fileCacheTail = last;
		    last->next = 0;
		}
		else
		{
		    current->next->previous = last;
		    if (current == fileCacheHead)
		    {
			fileCacheHead = current->next;
		    }
		    else
		    {
			last->next = current->next;
		    }
		}
	    }

	    if (current->storeFd != -1)
		currCacheSize--;	
	 
	    delete current;

	    break;
	}
	last = current;
	current = current->next;
    }
}
  
/*
 * FdCacheEntry is a typedef name of FdCache::cacheEntry
 * Used here because Cfront 2.1 based systems cannot cope
 * with names like FdCache::cacheEntry::cacheEntry
 */


#ifdef GCC_NESTED_TYPE_BUG
cacheEntry::
#else 
FdCacheEntry::
#endif 
cacheEntry ( const Uid& objUid, int fd )
	   : cachedUid(objUid),
	     storeFd(fd),
	     holdOpen(TRUE),
	     defaultBlockSize(0),
	     maxCachedImageSize(0),
	     cachedDictPtr(0),
	     cachedBlockPtr(0),
	     next(0),
	     previous(0)
{
}

#ifndef GCC_NESTED_TYPE_BUG
FdCacheEntry::~cacheEntry ()
#else
cacheEntry::~cacheEntry ()
#endif
{
    if (storeFd != -1)
	::close(storeFd);
    
    if (cachedDictPtr != 0)
	delete cachedDictPtr;

    if (cachedBlockPtr != 0)
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] cachedBlockPtr;
#else
        delete cachedBlockPtr;
#endif
}
    
