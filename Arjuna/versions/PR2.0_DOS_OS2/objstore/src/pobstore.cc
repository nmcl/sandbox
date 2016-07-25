/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * LSPObjStore.cc,v 1.5 1993/11/26 12:30:54 nmcl Exp $
 */

// for Borland to get ::access
#ifdef MSDOS
#  include <io.h>
#  include <fcntl.h>
#  include <dir.h>
#  include <share.h>

#ifndef SYS_TYPES_H_
#  include <System/sys/types.h>
#endif

#endif

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
#  include <ObjStore/ObjStore.h>
#endif

#ifndef LSPOBJSTORE_H_
#  include <ObjStore/PObStore.h>
#endif


/*
 * Where possible we try to keep the format of various
 * paths the same, so directory names are still separated
 * by /. However, in MSDOS we need to change these to \
 * to make sense. The following routine does this for use
 */

#ifdef MSDOS

char* convertPath (char* toConvert)
{
    char* index = NULL;
	do
    {
		index = strpbrk(toConvert, "/");

		if (index)
			*index = '\\';
	} while (index);

	return toConvert;
}

#endif


/*     
 * These values should be determined via something like pathconf
 */

static const int MAXPNAMELEN = 255;
static const int MAXNAMELEN = 255;

char* LocalSimplePersistentObjectStore::rootOfStore = (char*) 0;


/*
 * Public constructors and destructor
 */

LocalSimplePersistentObjectStore::LocalSimplePersistentObjectStore () 
                                                                    : storeValid(TRUE), storeName(0),
objState(0)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LocalSimplePersistentObjectStore::LocalSimplePersistentObjectStore()" << endl;
#endif

    setupStore();
}

LocalSimplePersistentObjectStore::LocalSimplePersistentObjectStore (const char* locationOfStore)
                                                                                               : storeValid(TRUE),
												 storeName((char*) locationOfStore)
{
#ifdef DEBUG
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LocalSimplePersistentObjectStore::LocalSimplePersistentObjectStore (const char* locationOfStore)" << endl;
#endif

    objState = 0;
    setupStore();
}

LocalSimplePersistentObjectStore::~LocalSimplePersistentObjectStore ()
{
#ifdef DEBUG
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "LocalSimplePersistentObjectStore::~LocalSimplePersistentObjectStore()" << endl;
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
 * Given a type name return an objectstate that contains all of the
 * uids of objects of that type
 */


ObjectState *LocalSimplePersistentObjectStore::allObjUids ( const TypeName tName )
{
    ObjectState *objstate = 0;
    DIR *dir = 0;
	char directory[MAXPNAMELEN+1];
	char fname[MAXNAMELEN+1];

    ::strcpy(directory, storeName);
		       
    if (tName[0] != '/')
	::strcat(directory, "/");
    
    ::strcat(directory, tName);

#ifdef MSDOS
	char* temp = convertPath(directory);
	strcpy(directory, temp);
#endif

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
				 << ")" << endl;
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

    if (!storeName)
    {
	if (LocalSimplePersistentObjectStore::rootOfStore == (char*) 0)
	{
	    char* storeRoot = ::getenv("OBJECTSTORE_ROOT");
	    if (!storeRoot)
	    {
	   	error_stream << FATAL << "OBJECTSTORE_ROOT not defined" << endl;
		ArjunaFatal();
	    }

	    LocalSimplePersistentObjectStore::rootOfStore = ::new char[::strlen(storeRoot)+1];
	    ::memset(LocalSimplePersistentObjectStore::rootOfStore, '\0', ::strlen(storeRoot)+1);
	    ::memcpy(LocalSimplePersistentObjectStore::rootOfStore, storeRoot, ::strlen(storeRoot));
	}

	storeName = ::new char[::strlen(LocalSimplePersistentObjectStore::rootOfStore)+1];
	::memset(storeName, '\0', ::strlen(LocalSimplePersistentObjectStore::rootOfStore)+1);
	::memcpy(storeName, LocalSimplePersistentObjectStore::rootOfStore, ::strlen(rootOfStore));
    }

	// modified for MSDOS and OS/2
    if (::access(storeName, 2) != 0)
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

#ifndef MSDOS
    struct flock arg;

    arg.l_whence = SEEK_SET;
    arg.l_len = 0;
    arg.l_start = 0;
    arg.l_type = F_UNLCK;

    if (::fcntl(filedesc, F_SETLK, (int)&arg) == -1)
	closedOk = FALSE;
#endif

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
	// modified for MSDOS and OS/2
    int accMode = (flags & O_RDONLY ? 0 : 2);
    Boolean createOk = TRUE;
    char *startPoint = 0;
    char *slashPoint = 0;
    char *tempPathName = new char[strlen(path) + 1];

	strcpy(tempPathName, path);

#ifdef MSDOS
	tempPathName = convertPath(tempPathName);
#endif

	/* Skip up path checking directories exist */

#ifndef MSDOS
	while (createOk && (slashPoint = strrchr(tempPathName, '/')))
#else
	while (createOk && (slashPoint = strrchr(tempPathName, '\\')))
#endif
    {
	/* chop off the tail of the path */
	
	*slashPoint = 0;

	if (::access(tempPathName, accMode) != 0)
	{
	    /* access failed - if ENOENT maybe go up one level */

		if ((errno == ENOENT) || (errno == EBADF))
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

#ifndef MSDOS
	while (slashPoint = strchr(startPoint, '/'))
#else
	while (slashPoint = strchr(startPoint, '\\'))
#endif
	{
		*slashPoint = 0;

		// changed for MSDOS
#ifndef MSDOS
	    if ((::mkdir(tempPathName, 0777) != 0) &&
		(errno != EEXIST))
#else
		if ((::mkdir(tempPathName) != 0) && (errno != EEXIST))
#endif
	    {
		createOk = FALSE;
		break;
	    }

#ifndef MSDOS
		*slashPoint = '/';
#else
		*slashPoint = '\\';
#endif
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
#ifdef MSDOS
    ::memset(fname, '\0', strlen(storeName)+strlen(tName)+
		strlen(uidBuff)+5);
#endif

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

#ifndef MSDOS
    if (ft != ORIGINAL)
	*cPtr++ = wrapper;

    slen = strlen(uidBuff);
    memcpy(cPtr, uidBuff, slen);
    cPtr += slen;
    if (ft != ORIGINAL)
	*cPtr++ = wrapper;
    *cPtr = '\0';
#else
    memcpy(cPtr, uidBuff, strlen(uidBuff));
    cPtr += strlen(uidBuff);
    if (ft != ORIGINAL)
	*cPtr++ = wrapper;
#endif

#ifndef MSDOS
	return fname;
#else
	return convertPath(fname);
#endif
}

// changed most of this for MSDOS
int LocalSimplePersistentObjectStore::openAndLock ( const char* fname, int flags, int lmode) const
{
#ifndef MSDOS
	int fd = ::open(fname, flags, 0666);
#else
	int fd = ::open(fname, flags | lmode | O_BINARY, 0666); // use this for locking
#endif

    if ((fd == -1) && ((errno == ENOENT) || (errno == EIO)))
	{
	if (createHierarchy(fname, flags))
#ifndef MSDOS
		fd = open(fname, flags, 0666);  // had to remove :: because of Borland bug
#else
		fd = open(fname, flags | lmode | O_BINARY, 0666);
#endif
    }

	if (fd != -1)
	{
#ifndef MSDOS
	struct flock arg;

	arg.l_whence = SEEK_SET;
	arg.l_start = 0;
	arg.l_len = 0;
	arg.l_type = lmode;
	
	if (::fcntl(fd, F_SETLKW, (int)&arg) != -1)
	    return(fd);
	else
		::close(fd);
#else
	return fd;  // we use open to do the locking for MSDOS
#endif
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
                 << (ft == ORIGINAL ? "ORIGINAL" : "SHADOW") << ")" << endl;
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
                 << (ft == ORIGINAL ? "ORIGINAL" : "SHADOW") << ")" << endl;
#endif

    ObjectState* new_image = (ObjectState *) 0;

    if (!storeValid)
		return new_image;

    if (tName)
    {
	int fd;	
	char *fname = genPathName(objUid, tName, ORIGINAL);

// modified for MSDOS
#ifndef MSDOS    
	if ((fd = openAndLock(fname, O_RDONLY, F_RDLCK)) != -1)
#else
	if ((fd = openAndLock(fname, O_RDONLY, SH_DENYRW)) != -1)
#endif
	{
		off_t imageSize = ::lseek(fd, 0, SEEK_END);

#ifdef DEBUG
	    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
	    debug_stream << "\treading " << fname
	                 << "(" << imageSize << " bytes)" << endl;
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

void LocalSimplePersistentObjectStore::setState (const ObjectState* state)
{
	objState = (ObjectState*) state;
}

Boolean LocalSimplePersistentObjectStore::write_state ( const Uid& objUid, const TypeName tName,
						        FileType _ft ) const
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
			 << tName << ", " << (_ft == ORIGINAL ? "ORIGINAL" : "SHADOW") << ")" << endl;
#endif

    if (!storeValid)
	return FALSE;
    
    if (!objState)
	return FALSE;

    if (objState->notempty() == FALSE)
    {
	error_stream << WARNING << "LocalSimplePersistentObjectStore::write_state() - empty ObjectState!!";
	return(FALSE);
    }

    if (tName)
    {
	char *fname = genPathName(objUid, tName, _ft);

	// modified for MSDOS
#ifndef MSDOS
	int fd = openAndLock(fname, O_CREAT|O_WRONLY, F_WRLCK);
#else
	int fd = openAndLock(fname, O_CREAT|O_WRONLY, SH_DENYWR);
#endif

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
	             << "(" << objState->size() << " bytes)" << endl;
#endif

	if (::write(fd, objState->buffer(), (int) objState->size()) != objState->size())
	{
	    error_stream << WARNING 
	                 << "LocalSimplePersistentObjectStore::write_state() - write failed "
					 << "for " << fname << "\n";
	    delete [] fname;
	    return(FALSE);
	}

	if (_ft == SHADOW)
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
#  include "ObjStore/PObStore.n"
#  undef LSPOBJSTORE_CC_
#endif
