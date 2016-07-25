/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LPObjStore.cc,v 
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef MEMORY_H_
#  include <System/memory.h>
#endif

#ifndef SYS_STAT_H_
#  include <System/sys/stat.h>
#endif

#ifndef SYS_RESOURCE_H_
#  include <System/sys/resource.h>
#endif

#ifndef SYS_UIO_H_
#  include <System/sys/uio.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef CTYPE_H_
#  include <System/ctype.h>
#endif

#ifndef DIRENT_H_
#  include <System/dirent.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef SYS_FILE_H_
#  include <System/sys/file.h>
#endif

#ifndef SYS_PARAM_H_
#  include <System/sys/param.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <Arjuna/ObjStore.h>
#endif

#ifndef LPOBJSTORE_H_
#  include <Arjuna/LPObjStore.h>
#endif

#ifndef FDCACHE_H_
#  include <Arjuna/FdCache.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

static const unsigned long MAGIC = 0x13579ace;

static const int MAXSTATESIZE = 16 * 1024 * 1024; /* 16MB */

Boolean LPObjStoreSetUp::setUp = FALSE;
StoreList* LPObjStoreSetUp::headOfList = 0;


LPObjStoreSetUp::LPObjStoreSetUp ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LPObjStoreSetUp::LPObjStoreSetUp ()" << endl;
#endif
    
    if (!setUp)
    {
	ObjectStore::addToList(LPObjStoreSetUp::create, LPObjStoreSetUp::destroy, "LocalPersistentObjectStore");
	setUp = TRUE;
    }
}

LPObjStoreSetUp::~LPObjStoreSetUp () {}

ObjectStore* LPObjStoreSetUp::create (const char* locationOfStore)
{
    StoreList* slptr = 0;
    Boolean found = FALSE, endOfList = FALSE;
    
    if (locationOfStore == 0)
	locationOfStore = OBJECTSTOREDIR_Q;
    
    slptr = headOfList;
    while ((slptr) && (!found) && (!endOfList))
    {
	if (::strcmp(slptr->instance->getStoreName(), locationOfStore) == 0)
	    found = TRUE;
	else
	{
	    if (slptr->next != 0)
		slptr = slptr->next;
	    else
		endOfList = TRUE;
	}
    }

    if (!found)
    {
	StoreList* toAdd = new StoreList;
	toAdd->instance = new LocalPersistentObjectStore(locationOfStore);

	if (headOfList == 0)
	    headOfList = toAdd;
	else
	    slptr->next = toAdd;

	slptr = toAdd;
    }

    slptr->useCount++;
    return slptr->instance;
}

void LPObjStoreSetUp::destroy (const char* locationOfStore)
{
    StoreList *slptr = 0, *slmarker = 0;
    Boolean found = FALSE;
    
    if (locationOfStore == 0)
	locationOfStore = OBJECTSTOREDIR_Q;

    if (headOfList == 0)
    {
	error_stream << WARNING << "Attempt to destroy object store instance without call on ObjectStore::create" << endl;
	return;
    }
    
    slptr = headOfList;
    while ((slptr) && (!found))
    {
	if (::strcmp(slptr->instance->getStoreName(), locationOfStore) == 0)
	    found = TRUE;
	else
	{
	    slmarker = slptr;
	    slptr = slptr->next;
	}
    }
    
    if (!found)
    {
	error_stream << WARNING << "Attempt to destroy non-existant object store instance" << endl;
	return;
    }
    
    slptr->useCount--;
    
    if (slptr->useCount == 0)
    {
	if (slmarker == 0)
	    headOfList = slptr->next;
	else
	    slmarker = slptr->next;
	
	delete slptr;
    }
}


/*
 * Public constructors and destructor
 */

LocalPersistentObjectStore::LocalPersistentObjectStore () 
                                                        : storeValid(TRUE),
							  storeName(OBJECTSTOREDIR_Q)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LocalPersistentObjectStore::LocalPersistentObjectStore()\n" << flush;
#endif

    setupStore();
}

LocalPersistentObjectStore::LocalPersistentObjectStore (const char* locationOfStore)
                                                                                   : storeValid(TRUE),
										     storeName(locationOfStore)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LocalPersistentObjectStore::LocalPersistentObjectStore (const char* locationOfStore)" << endl;
#endif

    setupStore();
}

LocalPersistentObjectStore::~LocalPersistentObjectStore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LocalPersistentObjectStore::~LocalPersistentObjectStore()\n" << flush;
#endif
}

/*
 * Public non-virtual functions
 */

/*
 * Given a type name return an objectstate that contains all of the
 * uids of objects of that type
 */

ObjectState *LocalPersistentObjectStore::allObjUids ( const TypeName tName )
{
    ObjectState *objstate = 0;
    DIR *dir = 0;
    char directory[MAXPATHLEN+1];
    char fname[MAXNAMLEN+1];

    ::strcpy(directory, storeName);
			       
    if (tName[0] != '/')
	::strcat(directory, "/");
    
    ::strcat(directory, tName);

    dir = ::opendir(directory);
    if (dir)
    {
	dirent *entry;

	objstate = new ObjectState();
	
	while (entry = ::readdir(dir))
	{
	    ::strcpy(fname,entry->d_name);

	    if (isdigit(fname[0]))
	    {
		Uid aUid(entry->d_name, TRUE);

		if (aUid != NIL_UID)
		    aUid.pack(*objstate);
	    }
	}

	/* terminate list */

	NIL_UID.pack(*objstate);
	::closedir(dir);
    }

    return objstate;
	
}

/*
 * The following operation commits a previous write_state operation which
 * was made with the SHADOW FileType argument. If the state was already
 * committed do nothing.
 */

Boolean LocalPersistentObjectStore::commit_state ( const Uid& objUid,
						   const TypeName tName )
{
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	debug_stream << "LocalPersistentObjectStore::commit_state(" << objUid << ", " << tName
	    << ")\n" << flush;
#endif
	
    Boolean commitOk = FALSE;

    /* Bail out if the object store is not set up */

    if (storeValid)
    {
#ifndef __linux__
	FdCache::cacheEntry *cachePtr = openAndLock(objUid, tName, F_WRLCK);
#else
	// nested types bug in g++ on linux!
	cacheEntry *cachePtr = openAndLock(objUid, tName, F_WRLCK);
#endif
	int fd = (cachePtr != 0 ? cachePtr->storeFd : -1);
	StateDictionary *sdPtr = (cachePtr != 0 ? cachePtr->cachedDictPtr : 0);

	if ((fd != -1) && (sdPtr != 0))
	{
	    if (sdPtr->shadowExists)
	    {
		unsigned long temp;
		int writeLen;
		iovec iov[2];

		writeLen = iov[0].iov_len = sizeof(struct StateDictionary);
		iov[0].iov_base = (caddr_t) sdPtr;

		/* swap over original/shadow entries */

		sdPtr->originalHidden = FALSE;
		temp = sdPtr->originalOffset;
		sdPtr->originalOffset = sdPtr->shadowOffset;
		sdPtr->shadowOffset = temp;
		temp = sdPtr->originalSize;
		sdPtr->originalSize = sdPtr->shadowSize;
		sdPtr->shadowSize = 0;
		sdPtr->shadowExists = FALSE;
		
		cachePtr->holdOpen = FALSE;
		
		if (sdPtr->originalSize <= cachePtr->maxCachedImageSize)
		{
		    writeLen += sdPtr->originalSize;
		    iov[1].iov_len = sdPtr->originalSize;
		    iov[1].iov_base = cachePtr->cachedBlockPtr;
		}
		else
		{
		    iov[1].iov_len = 0;
		    iov[1].iov_base = 0;
		}
	    
		/* rewrite header */
		
		::lseek(fd, 0, SEEK_SET);
		
		if (::writev(fd, iov, 2) != writeLen)
		{
		    error_stream << WARNING 
				 << "LocalPersistentObjectStore::commit_state()"
				 << " - writev failed for " << objUid << "\n";
		    closeAndUnlock(cachePtr);
		} else
#ifdef __linux__
		    if (::fsync(fd) == -1)
		    {
			error_stream << WARNING
				     << "LocalPersistentObjectStore::commit_state() "
				     << "fsync failed for " << objUid << "\n";
			closeAndUnlock(cachePtr);
			return FALSE;
		    }
		    else
#endif
			commitOk = closeAndUnlock(cachePtr);
	    }
	    else
		commitOk = closeAndUnlock(cachePtr);
	}
    }
	
    if (!commitOk)
    {
	/* If here there is an error */
	
	error_stream << WARNING 
	    << "LocalPersistentObjectStore::commit_state() "
		<< "failed for " << objUid << "\n";
    }

    return commitOk;
}

/*
 * Private non-virtual functions
 */

void LocalPersistentObjectStore::setupStore ()
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "void LocalPersistentObjectStore::setupStore ()" << endl;
#endif

    /* The root of the objectstore must exist and be writable */

    if (::access(storeName, W_OK) != 0)
    {
	error_stream << FATAL << "Cannot access root of object store: "
	    << storeName << "\n";
	storeValid = FALSE;
	storeBlockSize = 0;
    } else
    {
	struct stat statarg;

	::stat(storeName, &statarg);
	storeBlockSize = statarg.st_blksize;
    }
}

/*
 * Unlock and close the file. Not that if the unlock fails we set
 * the return value to FALSE to indicate an error but rely on the
 * close to really do the unlock (which the manual says it will do).
 */

#ifndef __linux__
Boolean LocalPersistentObjectStore::closeAndUnlock ( FdCache::cacheEntry *ptr )
#else
// nested types bug in g++ on linux!
Boolean LocalPersistentObjectStore::closeAndUnlock ( cacheEntry *ptr )
#endif
{
    Boolean closedOk = TRUE;
    struct flock arg;

    arg.l_whence = SEEK_SET;
    arg.l_len = 0;
    arg.l_start = 0;
    arg.l_type = F_UNLCK;

    if (::fcntl(ptr->storeFd, F_SETLK, (int)&arg) == -1)
	closedOk = FALSE;

    if (!ptr->holdOpen)
	UnixFdCache.purgeFromCache(ptr->cachedUid);

    return closedOk;
}

/*
 * Attempt to build up the object store in the file system dynamically.
 * This creates directories as required as new types are added to the
 * store. Note that we made sure the root object store was created and
 * writable at construction time
 */

Boolean LocalPersistentObjectStore::createHierarchy ( const char *path,
						      int flags ) const
{
    int accMode = (flags & O_RDONLY ? X_OK : W_OK);
    Boolean createOk = TRUE;
    char *startPoint = 0;
    char *slashPoint = 0;
    char *tempPathName = new char[strlen(path) + 1];

    strcpy(tempPathName, path);

    /* Skip up path checking directories exist */
    
    while (createOk && (slashPoint = strrchr(tempPathName, '/')))
    {
	/* chop of the tail of the path */
	
	*slashPoint = 0;
	
	if (::access(tempPathName, accMode) != 0)
	{
	    /* access failed - if ENOENT maybe go up one level */

	    if (errno == ENOENT)
	    {
		/* Test if at root object store level */
	    
		if (strcmp(tempPathName, storeName) == 0)
		    break;
	    }
	    else
		createOk = FALSE;
	}
	else
	{
	    startPoint = slashPoint+1;
	    break;
	}
    }

    strcpy(tempPathName, path);

    if (startPoint != 0)
    {
	/* Create sub-directories as required */

	while (slashPoint = strchr(startPoint, '/'))
	{
	    *slashPoint = 0;
	    if ((::mkdir(tempPathName, 0777) != 0) &&
		(errno != EEXIST))
	    {
		createOk = FALSE;
		break;
	    }
	    chmod(tempPathName, 0777);
	    *slashPoint = '/';
	    startPoint = slashPoint + 1;
	} 
    }

#ifndef __GNUG__
    delete [] tempPathName;
#else
    delete tempPathName;
#endif

    return createOk;
}

/*
 * Function to return the file name for the state of the object
 * identified by the Uid and TypeName. If the FileType argument
 * is SHADOW then the Uid part of the name includes # characters.
 * The magic number 5 below is the number of extra characters needed
 * to make up the entire path.
 * Return char * is dynamically allocated and must be freed by caller
 */

char* LocalPersistentObjectStore::genPathName ( const Uid& objUid, 
					        const TypeName tName ) const
{
    char uidBuff[MAXPATHLEN+1];
    ostrstream os(uidBuff, MAXPATHLEN+1, ios::out);
    char *fname;
    register char *cPtr;
    register int slen;

    /* generate string version of Uid - BEWARE need trailing null!!*/

    os << objUid << ends;

    fname = new char[strlen(storeName) + strlen(tName) +
		     strlen(uidBuff) + 5];

    cPtr = fname;

    slen = strlen(storeName);
    memcpy(cPtr, storeName, slen);
    cPtr += slen;

    if (tName[0] != '/')
	*cPtr++ = '/';
    slen = strlen(tName);
    memcpy(cPtr, tName, slen);
    cPtr += slen;

    if (*(cPtr-1) != '/')
	*cPtr++ = '/';

    slen = strlen(uidBuff);
    memcpy(cPtr, uidBuff, slen);
    cPtr += slen;
    *cPtr = '\0';

    return fname;
}

#ifndef __linux__
FdCache::cacheEntry *
#else
// nested types bug in g++ on linux
cacheEntry *
#endif
LocalPersistentObjectStore::openAndLock ( const Uid& objUid,
					  const TypeName tName,
					  const int lMode )
{
#ifndef __linux__
    FdCache::cacheEntry *cachePtr = UnixFdCache.scanCache(objUid);
#else
    cacheEntry *cachePtr = UnixFdCache.scanCache(objUid);
#endif

    int fd = (cachePtr == 0 ? -1 : cachePtr->storeFd);
    Boolean openOk = FALSE;
    
    if (fd == -1)
    {
	/* File not currently open make it so if possible */
	
	int flags = (lMode == F_RDLCK ? O_RDWR : O_RDWR | O_CREAT | O_SYNC);
	char *fname = genPathName(objUid, tName);
	Boolean retryOpen;
	
	fd = ::open(fname, flags, 0666);
	retryOpen = (fd == -1 ? TRUE : FALSE);

	while (retryOpen)
	{
	    /* primary open failed - determine reason and maybe retry*/

	    if ((flags & O_CREAT) && (errno == ENOENT))
	    {
		/* File did not exist */

		retryOpen = createHierarchy(fname, flags);
	    }
	    else
		if (errno == EMFILE)
		{
		    /* Too many open files */

		    retryOpen = UnixFdCache.compactCache();
		}
		else
		    retryOpen = FALSE;
	    
	    if (retryOpen)
		retryOpen = ((fd = ::open(fname, flags, 0666)) == -1);
	    
	}
	
	if (fd != -1)
	    ::fchmod(fd, 0666);

#ifndef __GNUG__
	delete [] fname;
#else
	delete fname;
#endif	
    }
    
    if (fd != -1)
    {
	/* File now open - enter in cache if possible */
	
	cachePtr = UnixFdCache.enterIntoCache(objUid, fd);
	
	if ((cachePtr != 0) && (cachePtr->cachedDictPtr != 0))
	{
	    /* Now have cache entry - lock file and update cache fields */
	    
	    struct flock arg;
	    
	    arg.l_whence = SEEK_SET;
	    arg.l_start = 0;
	    arg.l_len = 0;
	    arg.l_type = lMode;
	    
	    if (::fcntl(fd, F_SETLKW, (int)&arg) != -1)
	    {
		struct stat statarg;
		
		if (::fstat(fd, &statarg) == 0)
		{
		    if (cachePtr->cachedBlockPtr == 0)
			cachePtr->cachedBlockPtr = new char[statarg.st_blksize];

		    if ( (cachePtr->cachedBlockPtr != 0) &&
			 (cachePtr->defaultBlockSize == 0))
		    {
			cachePtr->defaultBlockSize = (unsigned int) statarg.st_blksize;
			cachePtr->maxCachedImageSize = (unsigned int) (statarg.st_blksize - sizeof(struct StateDictionary));
		    }
		    
		    ::lseek(fd, 0, SEEK_SET);
		    
		    if (lMode & F_WRLCK)
		    {
			/* open for writing - pre-read file header or initialise */
			
			if (statarg.st_size == 0)
			{	
			    cachePtr->cachedDictPtr->magic = MAGIC;
			    cachePtr->cachedDictPtr->shadowSize = 0;
			    cachePtr->cachedDictPtr->originalSize = 0;
			    cachePtr->cachedDictPtr->shadowOffset = storeBlockSize + MAXSTATESIZE;
			    cachePtr->cachedDictPtr->originalOffset = storeBlockSize;
			    cachePtr->cachedDictPtr->originalHidden = FALSE;
			    cachePtr->cachedDictPtr->shadowExists = FALSE;
			    openOk = TRUE;
			}
			else
			    if (::read(fd, (char *)cachePtr->cachedDictPtr, sizeof(struct StateDictionary)) == sizeof(struct StateDictionary))
			    {
				if (cachePtr->cachedDictPtr->magic == MAGIC)
				    openOk = TRUE;
			    }
		    }
		    else
			openOk = TRUE;
		}
	    }
	}
    }
    
    if (!openOk)
    {
	/* An error occurred - cleanup */
	
	if (cachePtr != 0)
	{
	    cachePtr->holdOpen = FALSE;
	    closeAndUnlock(cachePtr);
	    cachePtr = 0;
	}
	else
	    if (fd != -1)
		::close(fd);
    }
    
    return cachePtr;
}


/*
 * Remove the ObjectState/file from the object store.
 */

Boolean LocalPersistentObjectStore::remove_state ( const Uid& storeUid,
						   const TypeName tName )
{
    return remove_state(storeUid, tName, ORIGINAL);
}

Boolean LocalPersistentObjectStore::remove_state ( const Uid& objUid, 
						   const TypeName tName, 
						   FileType ft)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "LocalPersistentObjectStore::remove_state(" << objUid << ", " << tName << ", "
                 << (ft == ORIGINAL ? "ORIGINAL" : "SHADOW") << ")\n" << flush;
#endif
    
    Boolean removeOk = FALSE;

    if (storeValid)
    {
#ifndef __linux__
	FdCache::cacheEntry *cachePtr = openAndLock(objUid, tName, F_WRLCK);
#else
	// nested types bug in g++ on linux
	cacheEntry *cachePtr = openAndLock(objUid, tName, F_WRLCK);
#endif
	int storeFd = (cachePtr == 0 ? -1 : cachePtr->storeFd);
	
	if (storeFd != -1)
	{
	    switch (ft)
	    {
	    case ORIGINAL:
		cachePtr->cachedDictPtr->originalSize = 0;
		break;
	    case SHADOW:
		cachePtr->cachedDictPtr->shadowSize = 0;
		break;
	    case HIDDEN:
		error_stream << WARNING
			     << "Attempt to remove hidden state for object "
			     << objUid << "\n" << flush;
		break;
	    }

	    if ((cachePtr->cachedDictPtr->originalSize == 0) && (cachePtr->cachedDictPtr->shadowSize == 0))
	    {
		char *fname = genPathName(objUid, tName);
		
		::unlink(fname);
		cachePtr->holdOpen = FALSE;
		removeOk = closeAndUnlock(cachePtr);

#ifndef __GNUG__
		delete [] fname;
#else
		delete fname;
#endif		
	    }
	    else
	    {
		/* rewrite header */
		::lseek(storeFd, 0, SEEK_SET);
		if ((::write(storeFd, (const char *)cachePtr->cachedDictPtr, sizeof(struct StateDictionary))) == sizeof(struct StateDictionary))
		    removeOk = closeAndUnlock(cachePtr);
#ifdef __linux__
		  else
		    if (::fsync(storeFd) == -1)
		    {
		      error_stream << WARNING << "fsync failed for storeFd" << endl;
		      return FALSE;
		    }
#endif
	    }
	}
    }

    return removeOk;
}

/*
 * read a committed instance of ObjectState out of the object store. The instance
 * is identified by the unique id and type
 */

ObjectState* LocalPersistentObjectStore::read_state ( const Uid& storeUid,
						      const TypeName tName)
{
    return read_state(storeUid, tName, ORIGINAL);
}

ObjectState* LocalPersistentObjectStore::read_state ( const Uid& objUid, 
						      const TypeName tName,
						      FileType ft)
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "LocalPersistentObjectStore::read_state(" << objUid << ", "
	         << tName << ", "
                 << (ft == ORIGINAL ? "ORIGINAL" : "SHADOW") << ")\n" << flush;
#endif

    ObjectState* newImage = 0;
    
    if (storeValid)
    {
#ifndef __linux__
	FdCache::cacheEntry *cachePtr = openAndLock(objUid, tName, F_RDLCK);
#else
	// nested types bug in g++ on linux
	cacheEntry *cachePtr = openAndLock(objUid, tName, F_RDLCK);
#endif
	int fd = (cachePtr == 0 ? -1 : cachePtr->storeFd);
	
	if (fd != -1)
	{
	    off_t imageSize = 0;
	    off_t seekOffset = 0;
	    iovec iov[2];
		     
	    iov[0].iov_len = sizeof(struct StateDictionary);
	    iov[0].iov_base = (caddr_t)cachePtr->cachedDictPtr;
	    iov[1].iov_len = cachePtr->defaultBlockSize;
	    iov[1].iov_base = new char[cachePtr->defaultBlockSize];

	    if (::readv(fd, iov, 2) >= sizeof(struct StateDictionary))
	    {
		switch (ft)
		{
		case ORIGINAL:
		    if (cachePtr->cachedDictPtr->originalHidden)
		    {
			error_stream << WARNING 
				     << "Attempted read on hidden state for "
				     << objUid << "\n" << flush;
#ifndef __GNUG__
			delete [] iov[1].iov_base;
#else
			delete iov[1].iov_base;
#endif
			closeAndUnlock(cachePtr);

			return 0;
		    }
		case HIDDEN:		/* beware of fall through */
		    imageSize = cachePtr->cachedDictPtr->originalSize;
		    seekOffset = cachePtr->cachedDictPtr->originalOffset;
		    break;
		case SHADOW:
		    imageSize = cachePtr->cachedDictPtr->shadowSize;
		    seekOffset = cachePtr->cachedDictPtr->shadowOffset;
		    break;
		}
		
		if (imageSize > 0)
		{
		    if ((ft == ORIGINAL) &&
			(imageSize) <= (cachePtr->maxCachedImageSize))
		    {
			/* Already got via readv on header! */
			
			newImage = new ObjectState(objUid, tName,
						   (int)imageSize,
						   (char*) iov[1].iov_base);
		    }
		    else
		    {
			::lseek(fd, seekOffset, SEEK_SET);
#ifndef __GNUG__
			delete [] iov[1].iov_base;
#else
			delete iov[1].iov_base;
#endif
			
#ifdef DEBUG
			debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
			debug_stream << "\treading " << objUid
			    << "(" << imageSize << " bytes)\n" << flush;
#endif

			/* create new objectstate to read the image into */
			
			newImage = new ObjectState(objUid, tName, (int)imageSize,
						   new char[(int)imageSize]);
			
			/* now try to read the actual image out of the store */

			if (newImage)
			{
			    if ((newImage->buffer() == 0) ||
				(::read(fd, newImage->buffer(), (int)imageSize) != imageSize))
			    {
				error_stream << WARNING
				    <<  "LocalPersistentObjectStore::read_state() failed\n";
				delete newImage;
				newImage = 0;
			    }
			}
		    }
		}
		else
#ifndef __GNUG__
		    delete [] iov[1].iov_base;
#else
		    delete iov[1].iov_base;
#endif
	    }
	
	    if (!closeAndUnlock(cachePtr))
	    {
		error_stream << WARNING << "Unlock or close of " << objUid
		    << "failed " << flush;
	    }
	}
    }

    return newImage;
}


/*
 * write_state saves the ObjectState in a file named by the type and Uid
 * of the ObjectState. If the second argument is SHADOW, then the file name
 * is different so that a subsequent commit_state invocation will rename
 * the file. If writting a shadow the original file is hidden.
 */

Boolean LocalPersistentObjectStore::write_state ( const Uid& storeUid,
						 const TypeName tName,
						 const ObjectState &state )
{
    return write_state(storeUid, tName, state, SHADOW);
}

Boolean LocalPersistentObjectStore::write_state ( const Uid& objUid, 
						  const TypeName tName,
						  const ObjectState& state, 
						  FileType ft )
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "LocalPersistentObjectStore::write_state(" << objUid << ", "
	         << tName << ", "<< (void *)&state << ", "
                 << (ft == ORIGINAL ? "ORIGINAL" : "SHADOW") << ")\n" << flush;
#endif

    Boolean wroteOk = FALSE;
    
    if ((storeValid) && state.notempty())
    {
#ifndef __linux__
	FdCache::cacheEntry *cachePtr = openAndLock(objUid, tName, F_WRLCK);
#else
	// nested types bug in g++ on linux
	cacheEntry *cachePtr = openAndLock(objUid, tName, F_WRLCK);
#endif
	int fd = (cachePtr == 0 ? -1 : cachePtr->storeFd);
	StateDictionary *sdPtr = (cachePtr == 0 ? 0 : cachePtr->cachedDictPtr);
	
	if ((fd != -1) && (sdPtr != 0))
	{
	    iovec iov[2];
	    int writeLen = 0;
	    size_t imageSize = state.size();
	    off_t seekOffset = 0;
	    char *imageBuffer = state.buffer();
	    Boolean doWrite = TRUE;

	    iov[1].iov_len = 0;
	    iov[1].iov_base = 0;

	    switch (ft)
	    {
	    case ORIGINAL:
		sdPtr->originalHidden = FALSE;
		sdPtr->shadowExists = FALSE;
		sdPtr->originalSize = imageSize;
		seekOffset = sdPtr->originalOffset;
		break;
	    case SHADOW:
		sdPtr->originalHidden = TRUE;
		sdPtr->shadowExists = TRUE;
		sdPtr->shadowSize = imageSize;
		seekOffset = sdPtr->shadowOffset;
		break;
	    case HIDDEN:
		error_stream << WARNING
			     << "Attempt to write hidden state for object "
			     << objUid << "\n" << flush;
		break;
	    }

	    ::lseek(fd, seekOffset, SEEK_SET);

#ifdef DEBUG   
	    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	    debug_stream << "\twriting " << objUid
		         << "(" << state.size() << " bytes)\n" << flush;
#endif

	    if (imageSize <= (cachePtr->maxCachedImageSize))
	    {
		if (ft == ORIGINAL)
		{
		    writeLen = imageSize;
		    iov[1].iov_len = imageSize;
		    iov[1].iov_base = imageBuffer;
		    doWrite = FALSE;
		}
		else
		{
		    ::memcpy(cachePtr->cachedBlockPtr, imageBuffer, imageSize);
		}
	    }

	    if (doWrite && (::write(fd, imageBuffer, imageSize) != imageSize))
	    {
		error_stream << WARNING 
		    << "LocalPersistentObjectStore::write_state() - write failed "
			<< "for " << objUid << "\n";
		closeAndUnlock(cachePtr);
	    }
	    else
	    {
#ifdef __linux__
	      if (::fsync(fd) == -1)
	      {
		error_stream << WARNING << "fsync failed for " << objUid << endl;
		closeAndUnlock(cachePtr);
		return FALSE;
	      }
#endif

		/* rewrite header */
		
		writeLen += sizeof(struct StateDictionary);
		iov[0].iov_len = sizeof(struct StateDictionary);
		iov[0].iov_base = (caddr_t)sdPtr;
		
		::lseek(fd, 0, SEEK_SET);

		if (::writev(fd, iov, 2) != writeLen)
		{
		    error_stream << WARNING 
				 << "LocalPersistentObjectStore::write_state()"
				 << " - write failed for " << objUid << "\n";
		    closeAndUnlock(cachePtr);  
		}
		else
#ifdef __linux__
		  if (::fsync(fd) == -1)
		  {
		    error_stream << WARNING << "fsync failed for " << objUid << endl;
		    closeAndUnlock(cachePtr);
		    return FALSE;
		  }
	          else
#endif
		    wroteOk = closeAndUnlock(cachePtr);
	    }
	}
    }

    if (!wroteOk)
    {
	/* If here there is an error */
	
	error_stream << WARNING 
	    << "LocalPersistentObjectStore::write_state() "
		<< "failed for " << objUid << "\n";
    }

    return wroteOk;
}

const TypeName LocalPersistentObjectStore::type () const
{
    return "LocalPersistentObjectStore";
}



#ifdef NO_INLINES
#  define LPOBJSTORE_CC_
#  include "Arjuna/LPObjStore.n"
#  undef LPOBJSTORE_CC_
#endif

