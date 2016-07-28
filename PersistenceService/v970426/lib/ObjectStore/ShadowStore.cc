/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ShadowStore.cc,v 1.19 1995/06/26 10:30:02 ngdp Exp $
 */

#ifndef STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <os/unistd.h>
#endif

#ifndef STRSTREAM_H_
#  include <os/strstream.h>
#endif

#ifndef PERSISTENCE_SERVICE

#ifndef PROTOS_H_
#  include <os/protos.h>
#endif

#ifndef MEMORY_H_
#  include <os/memory.h>
#endif

#ifndef SYS_FILE_H_
#  include <os/sys/file.h>
#endif

#endif

#ifndef SYS_STAT_H_
#  include <os/sys/stat.h>
#endif

#ifndef ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef STRING_H_
#  include <os/string.h>
#endif

#ifndef CTYPE_H_
#  include <os/ctype.h>
#endif

#ifndef DIRENT_H_
#  include <os/dirent.h>
#endif

#ifndef FCNTL_H_
#  include <os/fcntl.h>
#endif

#ifndef OBJSTATE_H_
#  include <Common/ObjState.h>
#endif

#ifndef FSSTORE_H_
#  include <ObjectStore/FSStore.h>
#endif

#ifndef SHADOWSTORE_H_
#  include <ObjectStore/ShadowStore.h>
#endif

#ifndef STORELIST_H_
#  include <ObjectStore/StoreList.h>
#endif

#ifdef NO_SYNC
#  undef DO_FSYNC
#  define SYNC 0
#else
#  ifdef __linux
#    define DO_FSYNC
#    define SYNC 0
#  endif
#endif

#if !defined(NO_SYNC) && !defined(DO_FSYNC)
#  ifdef O_DSYNC
#    define SYNC O_DSYNC
#  else
#    ifdef O_FSYNC
#      define SYNC O_FSYNC
#    else
#      ifdef O_SYNC
#        define SYNC O_SYNC
#      endif
#    endif
#  endif
#endif

/*
 * These values should be determined via something like pathconf
 */

static osCreatorFunc createSSInstance;

static const char HIDDINGCHAR = '#';
static const char SHADOWCHAR = '!';

#ifdef STRUCT_INIT_BUG
static SetUpInfo ShadowingSetup = { 0, 0, 0, &createSSInstance, ShadowStoreType, 0  };
#else
static SetUpInfo ShadowingSetup = { FALSE, FALSE, 0, &createSSInstance, ShadowStoreType, 0  };
#endif


ObjectStore* createSSInstance ( const char* locationOfStore )
{
    return new ShadowingStore(locationOfStore);
}

ShadowingStoreSetUp::ShadowingStoreSetUp ()
					 : FileSystemStoreSetUp(ShadowingSetup)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ShadowingStoreSetUp::ShadowingStoreSetUp ()" << endl;
    debug_stream.unlock();
    
#endif

#ifdef GCC_STATIC_INIT_BUG
    if (!ShadowingSetup._setUp)
    {
	ShadowingSetup._new = &createSSInstance;
	ShadowingSetup._tn = ShadowStoreType;
	ShadowingSetup._setUp = TRUE;
	ObjectStore::addToList(&ShadowingSetup);
    }
#endif
}

ShadowingStoreSetUp::~ShadowingStoreSetUp () {}

/*
 * Public constructors and destructor
 */

/*
 * NOTE we use the protected constructor and then call setupStore.
 * This ensures that the correct virtual type operation is called!
 */

ShadowingStore::ShadowingStore ( const char* locationOfStore )
			       : FileSystemStore()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ShadowingStore::ShadowingStore (\"" << locationOfStore
		 << "\")" << endl;
    debug_stream.unlock();
    
#endif

    setupStore(locationOfStore);
}

ShadowingStore::~ShadowingStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ShadowingStore::~ShadowingStore()" << endl;
    debug_stream.unlock();
    
#endif
}

/*
 * Public functions
 */

/*
 * Determine current state of object. Assumes that genPathName allocates enough
 * extra space to allow extra chars to be added.
 * State search is ordered OS_SHADOW, OS_HIDDEN, OS_ORIGINAL, OS_HIDDEN
 */

ObjectStore::StateStatus ShadowingStore::currentState ( const Uid& objUid,
							const TypeName tName )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ShadowingStore::currentState(" 
		 << objUid << ", " << tName << ")" << endl;
    debug_stream.unlock();
    
#endif

    ObjectStore::StateStatus theState = ObjectStore::OS_UNKNOWN;
    
    if (storeValid())
    {
	char *path = genPathName(objUid, tName, ObjectStore::OS_SHADOW);
	int len = ::strlen(path);

	if (exists(path))
	    theState = ObjectStore::OS_UNCOMMITTED;
	else
	{
	    path[len++] = HIDDINGCHAR;
	    path[len] = '\0';
	    
	    if (exists(path))
		theState = ObjectStore::OS_UNCOMMITTED_HIDDEN;
	    else
	    {
#ifndef GCC_ARRAY_DELETE_BUG
		delete [] path;
#else
		delete path;
#endif
		path = genPathName(objUid, tName, ObjectStore::OS_ORIGINAL);
		len = ::strlen(path);
		
		if (exists(path))
		    theState = ObjectStore::OS_COMMITTED;
		else
		{
		    path[len++] = HIDDINGCHAR;
		    path[len] = '\0';
		    
		    if (exists(path))
			theState = ObjectStore::OS_COMMITTED_HIDDEN;
		}
	    }
	}
#ifndef GCC_ARRAY_DELETE_BUG
	delete [] path;
#else
	delete path;
#endif
    }

    return theState;
}

/*
 * The following operation commits a previous write_state operation which
 * was made with the SHADOW StateType argument. This is achieved by
 * renaming the shadow and removing the hidden version.
 */

Boolean ShadowingStore::commit_state ( const Uid& objUid,
				       const TypeName tName )
{
    
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ShadowingStore::commit_state(" << objUid << ", " << tName
		 << ")" << endl;
    debug_stream.unlock();
    
#endif
	
    int result = 0;

    /* Bail out if the object store is not set up */

    if (!storeValid())
	return FALSE;

    if (tName)
    {
	char *filename;
	char *shadow;
	int len = 0;
	ObjectStore::StateStatus state = currentState(objUid, tName);

	if ((state == OS_UNCOMMITTED_HIDDEN) || (state == OS_UNCOMMITTED))
	{
	    shadow = genPathName(objUid, tName, ObjectStore::OS_SHADOW);
	    filename = genPathName(objUid, tName, ObjectStore::OS_ORIGINAL);
	    
	    if (state == OS_UNCOMMITTED_HIDDEN)
	    {
		/* maintain hidden status on rename */
		
		len = ::strlen(shadow);
		shadow[len++] = HIDDINGCHAR;
		shadow[len] = '\0';

		len = ::strlen(filename);
		filename[len++] = HIDDINGCHAR;
		filename[len] = '\0';
	    }
	    
	    result = ::rename(shadow, filename);

	    /* Free up memory allocated by genPathName calls */
	
	    delete [] shadow;
	    delete [] filename;
	}
    }
    else
    {
	cerr << "object with uid "
	     << objUid << " has no TypeName\n";
	result = -1;
    }
    
    if (result == -1)
	return(FALSE);
    else
	return(TRUE);
}

/*
 * The following operations hide/reveal an object regardless of state.
 * Hidden objects cannot be read but they can be written (Crash recovery
 * needs this)
 */

Boolean ShadowingStore::hide_state ( const Uid& objUid,
				     const TypeName tName )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ShadowingStore::hide_state(" 
		 << objUid << ", " << tName << ")" << endl;
    debug_stream.unlock();
    
#endif
	
    Boolean hiddenOk = TRUE;

    /* Bail out if the object store is not set up */

    if (storeValid())
    {
	ObjectStore::StateStatus state = currentState(objUid, tName);
	char *path1 = 0;
	char *path2 = 0;
	int len = 0;
	
	switch (state)
	{
	case ObjectStore::OS_UNCOMMITTED_HIDDEN:
	case ObjectStore::OS_COMMITTED_HIDDEN:
	    break;
	case ObjectStore::OS_COMMITTED:
	    {
		path1 = genPathName(objUid, tName, ObjectStore::OS_ORIGINAL);
		len = ::strlen(path1);
		path2 = new char[len + 2];
		::strcpy(path2, path1);

		path2[len++] = HIDDINGCHAR;
		path2[len] = '\0';
	    
		::rename(path1, path2);
		break;
	    }
	 case ObjectStore::OS_UNCOMMITTED:
	    {
		path1 = genPathName(objUid, tName, ObjectStore::OS_SHADOW);
		len = ::strlen(path1);
		path2 = new char[len + 2];
		::strcpy(path2, path1);

		path2[len++] = HIDDINGCHAR;
		path2[len] = '\0';
	    
		::rename(path1, path2);
		break;
	    }
	default:
	    hiddenOk = FALSE;
	}
	
	if (path1)
#ifndef GCC_ARRAY_DELETE_BUG
	    delete [] path1;
#else
	    delete path1;
#endif
	
	if (path2)
#ifndef GCC_ARRAY_DELETE_BUG
	    delete [] path2;
#else
	    delete path2;
#endif

    }
    else
	hiddenOk = FALSE;

    return hiddenOk;
}

Boolean ShadowingStore::reveal_state ( const Uid& objUid,
				       const TypeName tName )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ShadowingStore::reveal_state(" 
		 << objUid << ", " << tName << ")" << endl;
    debug_stream.unlock();
    
#endif
	
    Boolean revealedOk = TRUE;

    if (storeValid())
    {
	ObjectStore::StateStatus state = currentState(objUid, tName);
	char *path1 = 0;
	char *path2 = 0;
	int len = 0;
	
	switch (state)
	{
	case ObjectStore::OS_UNCOMMITTED_HIDDEN:
	    {
		path1 = genPathName(objUid, tName, ObjectStore::OS_SHADOW);
		len = ::strlen(path1);
		path2 = new char[len + 2];
		::strcpy(path2, path1);

		path2[len++] = HIDDINGCHAR;
		path2[len] = '\0';

		::rename(path2, path1);
		break;
	    }
	    case ObjectStore::OS_COMMITTED_HIDDEN:
	    {
		path1 = genPathName(objUid, tName, ObjectStore::OS_ORIGINAL);
		len = ::strlen(path1);
		path2 = new char[len + 2];
		::strcpy(path2, path1);

		path2[len++] = HIDDINGCHAR;
		path2[len] = '\0';

		::rename(path2, path1);
		break;
	    }	
	case ObjectStore::OS_COMMITTED:
	case ObjectStore::OS_UNCOMMITTED:
	    break;
	default:
	    revealedOk = FALSE;
	}
	
	if (path1)
#ifndef GCC_ARRAY_DELETE_BUG
	    delete [] path1;
#else
	    delete path1;
#endif
	
	if (path2)
#ifndef GCC_ARRAY_DELETE_BUG
	    delete [] path2;
#else
	    delete path2;
#endif

    }
    else
	revealedOk = FALSE;

    return revealedOk;
}

ShadowingStore::ShadowingStore ( )
			       : FileSystemStore()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ShadowingStore::ShadowingStore ()" << endl;
    debug_stream.unlock();
    
#endif
}


/*
 * Function to return the file name for the state of the object
 * identified by the Uid and TypeName. If the StateType argument
 * is OS_SHADOW then the Uid part of the name includes # characters.
 * Return char * is dynamically allocated and must be freed by caller
 * Builds on lower level genPathName which allocates enough slop to accomodate
 * the extra chars
 */

char* ShadowingStore::genPathName ( const Uid& objUid,
				    const TypeName tName,
				    ObjectStore::StateType ft ) const
{
    char *fname = FileSystemStore::genPathName(objUid, tName, ft);

    if (ft == OS_SHADOW)
    {
	char *cPtr = fname + ::strlen(fname);
    
	*cPtr++ = SHADOWCHAR;
    
	*cPtr = '\0';
    }
    
    return fname;
}

/*
 * Private operations
 */

ObjectState* ShadowingStore::read_state ( const Uid& objUid,
					  const TypeName tName,
					  ObjectStore::StateType ft )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "ShadowingStore::read_state(" << objUid << ", "
	         << tName << ", "
                 << (ft == ObjectStore::OS_ORIGINAL ? "ORIGINAL" : "SHADOW") << ")" << endl;
    debug_stream.unlock();
    
#endif

    ObjectState* new_image = (ObjectState *) 0;

    if (!storeValid())
	return new_image;

    if (tName)
    {
	ObjectStore::StateStatus state = currentState(objUid, tName);

	if ((state == OS_COMMITTED) || (state == OS_UNCOMMITTED))
	{
	    int fd;	
	    char *fname = genPathName(objUid, tName, ObjectStore::OS_ORIGINAL);
    
	    if ((fd = openAndLock(fname, O_RDONLY | SYNC, F_RDLCK)) != -1)
	    {
		off_t imageSize = ::lseek(fd, 0, SEEK_END);
		char *buffer = new char[(int)imageSize];
	    
#ifdef DEBUG
		debug_stream.lock();
		
		debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
		debug_stream << "\treading " << fname
			     << "(" << imageSize << " bytes)" << endl;
		debug_stream.unlock();
		
#endif

		::lseek(fd, 0, SEEK_SET);

		/* now try to read the actual image out of the store */

		if ((buffer != 0) &&
		    (::read(fd, buffer, (int)imageSize) == imageSize))
		{
		    new_image = new ObjectState(objUid, tName, (int)imageSize,
						buffer);
  
		}
		else
		    cerr <<  "ShadowingStore::read_state() failed\n";

		if ((new_image == 0) && (buffer != 0))
		{
#ifdef GCC_ARRAY_DELETE_BUG
		    delete buffer;
#else
		    delete [] buffer;
#endif
		}
		
		if (!closeAndUnlock(fd))
		{
		    cerr << "Unlock or close of " << fname
			 << "failed " << flush;
		}
	    }
#ifdef GCC_ARRAY_DELETE_BUG
	    delete fname;
#else
	    delete [] fname;
#endif
	}
	
    }
    else
	cerr << "Object with uid "
	     << objUid << " has no TypeName\n";

    return new_image;
}


/*
 * Remove the ObjectState/file from the object store.
 */

Boolean ShadowingStore::remove_state ( const Uid& objUid,
				       const TypeName name, 
				       ObjectStore::StateType ft )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "ShadowingStore::remove_state(" << objUid << ", " << name << ", "
                 << (ft == ObjectStore::OS_ORIGINAL ? "ORIGINAL" : "SHADOW") << ")" << endl;
    debug_stream.unlock();
    
#endif
    
    Boolean removeOk = TRUE;

    if (!storeValid())
	return FALSE;
    
    if (name)
    {
	ObjectStore::StateStatus state = currentState(objUid, name);

	if ((state == OS_COMMITTED) || (state == OS_UNCOMMITTED))
	{
	    char *fname = genPathName(objUid, name, ft);

	    if (::access(fname, 2) == -1)
	    {
		removeOk = FALSE;
		if (ft == ObjectStore::OS_ORIGINAL)
		    cerr << "ShadowingStore::remove_state() - access problems on "
			 << objUid << "\n";
	    }
	    else
		if (::unlink(fname) == -1)
		{
		    removeOk = FALSE;
		    if (ft == OS_ORIGINAL)
			cerr << "ShadowingStore::remove_state() - unlink failed on "
			     << fname << "\n";
		}

	    /* clean up memory usage of genPathName() */

	    delete [] fname;
	}
	else
	{
	    removeOk = FALSE;
	    cerr << "ShadowingStore::remove_state() attempted removal of ";
	    if (state == OS_UNKNOWN)
		cerr << "unknown ";
	    else
		cerr << "hidden ";
	    cerr << "state for object with uid " << objUid << endl;
	}
    }
    else
    {
	removeOk = FALSE;
	cerr << "type() operation of object with uid "
	     << objUid << " returns NULL";
    }
    
    return removeOk;
}


/*
 * write_state saves the ObjectState in a file named by the type and Uid
 * of the ObjectState. If the second argument is SHADOW, then the file name
 * is different so that a subsequent commit_state invocation will rename
 * the file.
 */

Boolean ShadowingStore::write_state ( const Uid& objUid,
				      const TypeName tName,
				      const ObjectState& state,
				      ObjectStore::StateType ft ) 
{
#ifdef DEBUG
    debug_stream.lock();
    
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "ShadowingStore::write_state(" << objUid << ", "
	         << tName << ", "<< (void *)&state << ", "
                 << (ft == ObjectStore::OS_ORIGINAL ? "ORIGINAL" : "SHADOW") << ")" << endl;
    debug_stream.unlock();
    
#endif

    if (!storeValid())
	return FALSE;
    
    if (tName)
    {
	char *fname = genPathName(objUid, tName, ft);
 	size_t imageSize = state.size();
	
	int fd = openAndLock(fname, O_CREAT|O_WRONLY|SYNC, F_WRLCK);
	
	if (fd == -1)
	{
	    cerr << "ShadowingStore::write_state() - openAndLock "
		 << "failed for " << fname << "\n";
	    delete [] fname;
	    return(FALSE);
	}

	if (imageSize > 0)
	{
#ifdef DEBUG
	    debug_stream.lock();
	    
	    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	    debug_stream << "\twriting " << fname
			 << "(" << imageSize << " bytes)" << endl;
	    debug_stream.unlock();
	    
#endif

	    if (::write(fd, (char *)state.buffer(), (int) imageSize) != imageSize)
	    {
		cerr << "ShadowingStore::write_state() - write failed "
		     << "for " << fname << "\n";
		delete [] fname;
		return(FALSE);
	    }
#ifdef DO_FSYNC
	    else
		if (::fsync(fd) == -1)
		{
		    cerr << "ShadowingStore::write_state() - fsync failed "
			 << "for " << fname << "\n";
		    delete [] fname;
		    return(FALSE);
		}
#endif
	}
	
	if (!closeAndUnlock(fd))
	{
	    cerr << "Unlock or close of " << fname
		 << "failed " << flush;
	}
	
	delete [] fname;
	return(TRUE);
    }
    else 
	cerr << "Object with uid "
	     << objUid << " has no TypeName\n";

    return (FALSE);
}


/*
 * Private non-virtual functions
 */

/*
 * Unlock and close the file. Not that if the unlock fails we set
 * the return value to FALSE to indicate an error but rely on the
 * close to really do the unlock (which the manual says it will do).
 */

Boolean ShadowingStore::closeAndUnlock ( int filedesc ) const
{
    Boolean closedOk = unlock(filedesc);

    if (::close(filedesc) == -1)
	closedOk = FALSE;

    return closedOk;
}

int ShadowingStore::openAndLock ( const char* fname, int flags, int lmode) const
{
    int fd = ::open((char *)fname, flags, 0666);

    if ((fd == -1) && (errno == ENOENT))
    {
	if (createHierarchy(fname, flags))
	    fd = ::open((char *)fname, flags, 0666);
    }

    if (fd != -1)
    {
	if (!lock(fd,lmode))
	{
	    ::close(fd);
	    fd = -1;
	}
    }
    
    return (fd);
}

#ifdef NO_INLINES
#  define SHADOWSTORE_CC_
#  include "ObjectStore/ShadowStore.n"
#  undef SHADOWSTORE_CC_
#endif
