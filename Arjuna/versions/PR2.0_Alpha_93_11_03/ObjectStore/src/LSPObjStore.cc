/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * LSPObjStore.cc,v
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
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef LSPOBJSTORE_H_
#include <ObjectStore/LSPObjStore.h>
#endif

#ifndef CONFIGURE_H_
#  include <Configure.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

/*
 * These values should be determined via something like pathconf
 */

static const int MAXPNAMELEN = 255;
static const int MAXNAMELENGTH = 255;

Boolean LSPObjStoreSetUp::setUp = FALSE;
StoreList* LSPObjStoreSetUp::headOfList = 0;


LSPObjStoreSetUp::LSPObjStoreSetUp ()
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LSPObjStoreSetUp::LSPObjStoreSetUp ()" << endl;
#endif

    if (!setUp)
    {
	ObjectStore::addToList(LSPObjStoreSetUp::create, LSPObjStoreSetUp::destroy, "LocalSimplePersistentObjectStore");
	setUp = TRUE;
    }
}

LSPObjStoreSetUp::~LSPObjStoreSetUp () {}

ObjectStore* LSPObjStoreSetUp::create (const char* locationOfStore)
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
	toAdd->instance = new LocalSimplePersistentObjectStore(locationOfStore);

	if (headOfList == 0)
	    headOfList = toAdd;
	else
	    slptr->next = toAdd;

	slptr = toAdd;
    }

    slptr->useCount++;
    return slptr->instance;
}

void LSPObjStoreSetUp::destroy (const char* locationOfStore)
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

LocalSimplePersistentObjectStore::LocalSimplePersistentObjectStore () 
                                                                    : storeValid(TRUE),
								      storeName(OBJECTSTOREDIR_Q)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LocalSimplePersistentObjectStore::LocalSimplePersistentObjectStore()\n" << flush;
#endif

    setupStore();
}

LocalSimplePersistentObjectStore::LocalSimplePersistentObjectStore (const char* locationOfStore)
                                                                                               : storeValid(TRUE),
												 storeName(locationOfStore)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LocalSimplePersistentObjectStore::LocalSimplePersistentObjectStore (const char* locationOfStore)" << endl;
#endif

    setupStore();
}

LocalSimplePersistentObjectStore::~LocalSimplePersistentObjectStore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LocalSimplePersistentObjectStore::~LocalSimplePersistentObjectStore()\n" << flush;
#endif
}

/*
 * Public non-virtual functions
 */

const TypeName LocalSimplePersistentObjectStore::type () const
{
    return "LocalSimplePersistentObjectStore";
}

/*
 * read a committed instance of ObjectState out of the object store. The instance
 * is identified by the unique id and type
 */

ObjectState* LocalSimplePersistentObjectStore::read_state ( const Uid& storeUid,
						       	   const TypeName tName)
{
    return read_state(storeUid, tName, ORIGINAL);
}

Boolean LocalSimplePersistentObjectStore::remove_state ( const Uid& storeUid,
							const TypeName tName )
{
    return remove_state(storeUid, tName, ORIGINAL);
}

Boolean LocalSimplePersistentObjectStore::write_state ( const Uid& storeUid,
						       const TypeName tName,
						       const ObjectState &state )
{
    return write_state(storeUid, tName, state, SHADOW);
}


/*
 * Given a type name return an objectstate that contains all of the
 * uids of objects of that type
 */

ObjectState *LocalSimplePersistentObjectStore::allObjUids ( const TypeName tName )
{
    ObjectState *objstate = 0;
    DIR *dir = 0;
    char directory[MAXPNAMELEN+1];
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
 * was made with the SHADOW FileType argument. This is achieved by
 * renaming the shadow and removing the hidden version.
 */

Boolean LocalSimplePersistentObjectStore::commit_state ( const Uid& objUid, const TypeName tName )
{
    int result = -1;

    /* Bail out if the object store is not set up */

    if (!storeValid)
	return FALSE;

    if (tName)
    {
	char *filename = genPathName(objUid, tName, ORIGINAL);
	char *shadow = genPathName(objUid, tName, SHADOW);
	
#ifdef DEBUG
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
	debug_stream << "LocalSimplePersistentObjectStore::commit_state(" << objUid << ", " << tName
	    << ")\n" << flush;
#endif
	
	result = ::rename(shadow, filename);

	remove_state(objUid, tName, HIDDEN);
	
	/* Free up memory allocated by genPathName calls */
	
	delete [] shadow;
	delete [] filename;
	
    } else {
	error_stream << FATAL << "object with uid "
	    << objUid << " has no TypeName\n";
	ArjunaFatal();
    }
    
    if (result == -1)
	return(FALSE);
    else
	return(TRUE);
}

/*
 * Private non-virtual functions
 */

void LocalSimplePersistentObjectStore::setupStore ()
{
    /* The root of the objectstore must exist and be writable */

    if (::access(storeName, W_OK) != 0)
    {
	error_stream << FATAL << "Cannot access root of object store: "
	    << storeName << "\n";
	storeValid = FALSE;
    }
}


/*
 * Unlock and close the file. Not that if the unlock fails we set
 * the return value to FALSE to indicate an error but rely on the
 * close to really do the unlock (which the manual says it will do).
 */

Boolean LocalSimplePersistentObjectStore::closeAndUnlock ( int filedesc ) const
{
    Boolean closedOk = TRUE;
    struct flock arg;

    arg.l_whence = SEEK_SET;
    arg.l_len = 0;
    arg.l_start = 0;
    arg.l_type = F_UNLCK;

    if (::fcntl(filedesc, F_SETLK, (int)&arg) == -1)
	closedOk = FALSE;

    if (::close(filedesc) == -1)
	closedOk = FALSE;

    return closedOk;
}

/*
 * Attempt to build up the object store in the file system dynamically.
 * This creates directories as required as new types are added to the
 * store. Note that we made sure the root object store was created and
 * writable at construction time
 */

Boolean LocalSimplePersistentObjectStore::createHierarchy ( const char *path,
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

    delete [] tempPathName;

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

char* LocalSimplePersistentObjectStore::genPathName ( const Uid& objUid, const TypeName tName,
						      FileType ft ) const
{
    char uidBuff[MAXPNAMELEN+1];
    ostrstream os(uidBuff, MAXPNAMELEN+1, ios::out);
    char *fname;
    register char *cPtr;
    register int slen;
    char wrapper = (ft == SHADOW ? '#' : '!');

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
    if (ft != ORIGINAL)
	*cPtr++ = wrapper;

    slen = strlen(uidBuff);
    memcpy(cPtr, uidBuff, slen);
    cPtr += slen;
    if (ft != ORIGINAL)
	*cPtr++ = wrapper;
    *cPtr = '\0';

    return fname;
}

int LocalSimplePersistentObjectStore::openAndLock ( const char* fname, int flags, int lmode) const
{
    int fd = ::open(fname, flags, 0666);

    if ((fd == -1) && (errno == ENOENT))
    {
	if (createHierarchy(fname, flags))
	    fd = ::open(fname, flags, 0666);
    }

    if (fd != -1)
    {
	struct flock arg;

	if (flags & O_CREAT)
	    ::chmod(fname, 0666);

	arg.l_whence = SEEK_SET;
	arg.l_start = 0;
	arg.l_len = 0;
	arg.l_type = lmode;
	
	if (::fcntl(fd, F_SETLKW, (int)&arg) != -1)
	    return(fd);
	else
	    ::close(fd);
    }
	
    return (-1);
}

/*
 * Remove the ObjectState/file from the object store.
 */

Boolean LocalSimplePersistentObjectStore::remove_state ( const Uid& objUid, const TypeName name, 
							 FileType ft ) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "LocalSimplePersistentObjectStore::remove_state(" << objUid << ", " << name << ", "
                 << (ft == ORIGINAL ? "ORIGINAL" : "SHADOW") << ")\n" << flush;
#endif
    
    Boolean removeOk = TRUE;

    if (!storeValid)
	return FALSE;
    
    if (name)
    {
	char *fname = genPathName(objUid, name, ft);

	if (::access(fname, 2) == -1)
	{
	    removeOk = FALSE;
	    if (ft == ORIGINAL)
		error_stream << WARNING
		    << "LocalSimplePersistentObjectStore::remove_state() - access problems on "
		    << objUid << "\n";
	}
	else
	    if (::unlink(fname) == -1)
	    {
		removeOk = FALSE;
		if (ft == ORIGINAL)
		    error_stream << WARNING 
			<< "LocalSimplePersistentObjectStore::remove_state() - unlink failed on "
			    << fname << "\n";
	    }

	/* clean up memory usage of genPathName() */

	delete [] fname;

    } else
    {
	removeOk = FALSE;
	error_stream << WARNING << "type() operation of object with uid "
	             << objUid << " returns NULL";
    }

    return removeOk;
}

ObjectState* LocalSimplePersistentObjectStore::read_state ( const Uid& objUid, const TypeName tName,
							    FileType ft ) const
{
#ifdef DEBUG
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "LocalSimplePersistentObjectStore::read_state(" << objUid << ", "
	         << tName << ", "
                 << (ft == ORIGINAL ? "ORIGINAL" : "SHADOW") << ")\n" << flush;
#endif

    ObjectState* new_image = (ObjectState *) 0;

    if (!storeValid)
	return new_image;

    if (tName)
    {
	int fd;	
	char *fname = genPathName(objUid, tName, ORIGINAL);
    
	if ((fd = openAndLock(fname, O_RDONLY, F_RDLCK)) != -1)
	{
	    off_t imageSize = ::lseek(fd, 0, SEEK_END);
	    
#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	    debug_stream << "\treading " << fname
	                 << "(" << imageSize << " bytes)\n" << flush;
#endif

	    /* create new objectstate to read the image into */

	    new_image = new ObjectState(objUid, tName, (int)imageSize,
					new char[(int)imageSize]);

	    ::lseek(fd, 0, SEEK_SET);

	    /* now try to read the actual image out of the store */

	    if (::read(fd, new_image->buffer(), (int)imageSize) != imageSize)
	    {
		error_stream << WARNING
		             <<  "LocalSimplePersistentObjectStore::read_state() failed\n";
		delete new_image;
		new_image = 0;
	    }

	    if (!closeAndUnlock(fd))
	    {
		error_stream << WARNING << "Unlock or close of " << fname
		    << "failed " << flush;
	    }
	}

	delete [] fname;
    } else
	error_stream << WARNING << "Object with uid "
	             << objUid << " has no TypeName\n";

    return new_image;
}


/*
 * write_state saves the ObjectState in a file named by the type and Uid
 * of the ObjectState. If the second argument is SHADOW, then the file name
 * is different so that a subsequent commit_state invocation will rename
 * the file. If writting a shadow the original file is hidden.
 */

Boolean LocalSimplePersistentObjectStore::write_state ( const Uid& objUid, const TypeName tName,
						        const ObjectState& state, FileType ft ) const
{
#ifdef DEBUG
    /* 
     * The void * cast in the next statement should really be to
     * a const void * but there is no ostream.operator<<(const void *)
     * in iostream.h (as of Cfront 3.0). Thus we have to put up with
     * a warning message or else 3.0 will not compile the code!
     */

    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "LocalSimplePersistentObjectStore::write_state(" << objUid << ", "
	         << tName << ", "<< (void *)&state << ", "
                 << (ft == ORIGINAL ? "ORIGINAL" : "SHADOW") << ")\n" << flush;
#endif

    if (!storeValid)
	return FALSE;
    
    if (state.notempty() == FALSE)
    {
	error_stream << WARNING << "LocalSimplePersistentObjectStore::write_state() - empty ObjectState!!";
	return(FALSE);
    }

    if (tName)
    {
	char *fname = genPathName(objUid, tName, ft);

	int fd = openAndLock(fname, O_CREAT|O_WRONLY, F_WRLCK);
	
	if (fd == -1)
	{
	    error_stream << WARNING 
	                 << "LocalSimplePersistentObjectStore::write_state() - openAndLock "
		         << "failed for " << fname << "\n";
	    delete [] fname;
	    return(FALSE);
	}

#ifdef DEBUG   
	debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	debug_stream << "\twriting " << fname
	             << "(" << state.size() << " bytes)\n" << flush;
#endif

	if (::write(fd, state.buffer(), (int) state.size()) != state.size())
	{
	    error_stream << WARNING 
	                 << "LocalSimplePersistentObjectStore::write_state() - write failed "
		         << "for " << fname << "\n";
	    delete [] fname;
	    return(FALSE);
	}
	else
	    if (::fsync(fd) == -1)
	    {
		error_stream << WARNING
		             << "LocalSimplePersistentObjectStore::write_state() - fsync failed "
			     << "for " << fname << "\n";
		delete [] fname;
		return(FALSE);
	    }

	if (ft == SHADOW)
	{
	    /* hide original file */

	    char *hidden = genPathName(objUid, tName, HIDDEN);
	    char *original = genPathName(objUid, tName, ORIGINAL);

	    ::rename(original, hidden);

	    delete [] hidden;
	    delete [] original;
	}
	
	if (!closeAndUnlock(fd))
	{
	    error_stream << WARNING << "Unlock or close of " << fname
		<< "failed " << flush;
	}

	delete [] fname;
	return(TRUE);
    } else
	error_stream << WARNING << "Object with uid "
	             << objUid << " has no TypeName\n";
    return (FALSE);
}


#ifdef NO_INLINES
#  define LSPOBJSTORE_CC_
#  include "Arjuna/LSPObjStore.n"
#  undef LSPOBJSTORE_CC_
#endif
