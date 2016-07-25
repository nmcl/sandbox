/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FdCache.cc,v 1.12 1993/07/26 10:11:17 nmcl Exp $
 */

/*
 *
 * Simple file descriptor cache. Holds fds for recently opened files in an
 * attempt to speed up the Object Store which is bogged down by file opens
 * Entries go in here at file CLOSE. This ensures that if the cache needs
 * emptying it can legitamately close the corresponding fd.
 *
 */ 

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef SYS_RESOURCE_H
#  include <System/sys/resource.h>
#endif

#ifdef _AIX
#  ifndef LIMITS_H_
#    include <System/limits.h>
#  endif
#endif

#ifndef FDCACHE_H_
#  include <Arjuna/FdCache.h>
#endif

/* The static instance of the cache for Unix process fds */

FdCache UnixFdCache;

FdCache::FdCache ()
                 :currCacheSize(0),
		  fileCacheTail(0),
		  fileCacheHead(0)
{
#if defined(_AIX) || defined(__linux__)
    maxCacheSize = OPEN_MAX / 2;
#else
    rlimit arg;

    ::getrlimit(RLIMIT_NOFILE, &arg);
    maxCacheSize = arg.rlim_cur / 2;
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

#ifndef __linux__
FdCache::cacheEntry *FdCache::scanCache ( const Uid& objUid )
#else
// bug in g++ on linux - it cannot understand nested types!
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

#ifndef __linux__
FdCache::cacheEntry *FdCache::enterIntoCache ( const Uid& objUid, const int fd )
#else
// bug in g++ on linux - it cannot understand nested types!
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


#ifdef __linux__
// nested types bug in g++ on linux
cacheEntry::
#else 
FdCacheEntry::
#endif 
cacheEntry ( const Uid& objUid, const int fd )
                         : cachedUid(objUid),
			   storeFd(fd),
			   holdOpen(TRUE),
			   defaultBlockSize(0),
			   maxCachedImageSize(0),
			   cachedDictPtr(new StateDictionary),
			   cachedBlockPtr(0),
			   next(0),
			   previous(0)
{
}

#ifndef __linux__
#ifdef __GNUG__
FdCache::cacheEntry::~cacheEntry (void)
#else
FdCacheEntry::~cacheEntry ()
#endif
#else
// nexted types bug in g++ on linux
cacheEntry::~cacheEntry ()
#endif
{
    if (storeFd != -1)
	::close(storeFd);
    
    if (cachedDictPtr != 0)
	delete cachedDictPtr;

    if (cachedBlockPtr != 0)
#ifndef __GNUG__
	delete [] cachedBlockPtr;
#else
        // bug in g++
        delete cachedBlockPtr;
#endif
}
    
