/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FdCache.h,v 1.9 1993/03/22 09:36:49 ngdp Exp $
 */

#ifndef FDCACHE_H_
#define FDCACHE_H_

/*
 *
 * Simple open file cache. Holds fds and header blocks for recently opened 
 * files in an attempt to speed up the Object Store which is bogged down by 
 * file opens
 *
 */

#ifndef SYS_TYPES_H_
#   include <System/sys/types.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

/* 
 * This structure sits in the 1st block of the file containing the states.
 */

struct StateDictionary
{
    unsigned long magic;
    off_t originalOffset;
    size_t originalSize;
    off_t shadowOffset;
    size_t shadowSize;
    Boolean originalHidden;
    Boolean shadowExists;
};

// @NoRemote, @NoMarshall
class FdCache
{
public:
    struct cacheEntry
    {
	cacheEntry (const Uid&, const int);
	~cacheEntry ();

	Uid cachedUid;			/* the uid in the store */
	int storeFd;		       	/* the fd of the underlying file */
	Boolean holdOpen;
	size_t defaultBlockSize;
	size_t maxCachedImageSize;
	StateDictionary *cachedDictPtr;
	char *cachedBlockPtr;		/* last written block */
	cacheEntry *next;
	cacheEntry *previous;
    };
    
    /* Constructors and destructor */
    
    FdCache ();
    ~FdCache ();

    cacheEntry *scanCache (const Uid&);
    cacheEntry *enterIntoCache(const Uid&, const int);
    Boolean compactCache ();
    void purgeFromCache(const Uid&);

private:
    int currCacheSize;			/* current number of live entries */
    int maxCacheSize;			/* max number of live entries allowed */
    cacheEntry *fileCacheHead;		/* the actual cache */
    cacheEntry *fileCacheTail;

};

/*
 * This typedef is for Cfront 2.1 based systems which cannot
 * cope with nested names properly
 */
 
typedef FdCache::cacheEntry FdCacheEntry;  

extern FdCache UnixFdCache;
#endif
