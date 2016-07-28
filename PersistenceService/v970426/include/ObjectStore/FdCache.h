/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FdCache.h,v 1.7 1994/11/29 13:43:41 ngdp Exp $
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
#   include <os/sys/types.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

struct StateDictionary;

#ifdef GCC_NESTED_TYPE_BUG

struct cacheEntry
{
  cacheEntry (const Uid&, int);
  ~cacheEntry ();

  Uid cachedUid;			/* the uid in the store */
  int storeFd;				/* the fd of the underlying file */
  Boolean holdOpen;
  size_t defaultBlockSize;
  size_t maxCachedImageSize;
  StateDictionary *cachedDictPtr;
  char *cachedBlockPtr;			/* last read/written block */
  cacheEntry *next;
  cacheEntry *previous;
};

#endif

// @NoRemote, @NoMarshall
class FdCache
{
public:

#ifndef GCC_NESTED_TYPE_BUG__linux__
    struct cacheEntry
    {
	cacheEntry (const Uid&, int);
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
#endif
    
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

#ifndef GCC_NESTED_TYPE_BUG
typedef FdCache::cacheEntry FdCacheEntry;  
#endif

extern FdCache UnixFdCache;
#endif
