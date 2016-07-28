/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FSStore.cc,v 1.11 1995/07/03 14:29:25 ngdp Exp $
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

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef FSSTORE_H_
#  include <ObjectStore/FSStore.h>
#endif

#ifndef STORELIST_H_
#  include "StoreList.h"
#endif

#ifndef CONFIGURE_H_
#  include <Config/Configure.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

/*
 * These values should be determined via something like pathconf
 */

static const int MAXPNAMELEN = 255;
static const int MAXNAMELENGTH = 255;
static const int SLOP = 9;

FileSystemStoreSetUp::~FileSystemStoreSetUp () {}


/*
 * Public constructors and destructor
 */

FileSystemStore::FileSystemStore ( const char* locationOfStore )
				 : fullStoreName(0),
				   localStoreName(0),
				   isValid(TRUE)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "FileSystemStore::FileSystemStore (\"" << locationOfStore
		 << "\")" << endl;
    debug_stream.unlock();
    
#endif

    setupStore(locationOfStore);
}

FileSystemStore::~FileSystemStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "FileSystemStore::~FileSystemStore()" << endl;
    debug_stream.unlock();
    
#endif
    
    if (localStoreName)
#ifndef GCC_ARRAY_DELETE_BUG
	::delete [] localStoreName;
#else
        ::delete localStoreName;
#endif    

    if (fullStoreName)
#ifdef GCC_ARRAY_DELETE_BUG
	delete fullStoreName;
#else
    	delete [] fullStoreName;
#endif
}

/*
 * Public functions
 */

/*
 * Given a type name return an objectstate that contains all of the
 * uids of objects of that type
 */

Boolean FileSystemStore::allObjUids ( const TypeName tName,
				      ObjectState& state,
				      ObjectStore::StateStatus match  )
{
    DIR *dir = 0;
    char directory[MAXPNAMELEN+1];
    char *cPtr = tName;

    /* fullStoreName ALWAYS has a trailing '/' */

    ::strcpy(directory, fullStoreName);

    /* Does typename start with a '/' if so skip over */

    if (tName && (tName[0] == '/'))
	cPtr++;
    
    ::strcat(directory, cPtr);

    dir = ::opendir(directory);
    if (dir)
    {
	dirent *entry;

	while ((entry = ::readdir(dir)))
	{
	    if (isdigit(entry->d_name[0]))
	    {
		Uid aUid(entry->d_name, TRUE);
		
		if ((aUid != NIL_UID) &&
		    ((match == ObjectStore::OS_UNKNOWN) ||
		     (isType(aUid, tName, match))))
		    aUid.pack(state);
	    }
	}

	::closedir(dir);
	NIL_UID.pack(state);

    }

    return TRUE;
}
 
Boolean FileSystemStore::allTypes ( ObjectState& foundTypes ) const
{
    Boolean result = TRUE;
    char directory[MAXPNAMELEN+1];
    char *cPtr = 0;
    DIR *dir = 0;

    ::strcpy(directory, fullStoreName);

    cPtr = directory + strlen(directory);

    dir = ::opendir(directory);

    if (dir)
    {
	dirent *entry;
	struct stat statbuf;
	
	while ((entry = ::readdir(dir)))
	{
	    if (!supressEntry(entry->d_name))
            {
                ::strcpy(cPtr,entry->d_name);
            
                if ((::stat(directory, &statbuf) == 0) &&
                    S_ISDIR(statbuf.st_mode))
		{
		    foundTypes.packNonMappedString(entry->d_name);
		    result = allTypes(foundTypes, cPtr);
		}
	    }
	}
	
	::closedir(dir);
    }
    
    foundTypes.packMappedString("");
    return result;
}

Boolean FileSystemStore::packInto (Buffer& buff) const
{
    return buff.packNonMappedString(localStoreName);
}

Boolean FileSystemStore::unpackFrom (Buffer& buff)
{
    Boolean result;
    char* tmpName = 0;
    
    result = buff.unpack(tmpName);

    if (result)
    {
	setupStore(tmpName);

	if (tmpName)
	{
#ifndef GCC_ARRAY_DELETE_BUG
	    delete [] tmpName;
#else
	    delete tmpName;
#endif
	}
    }
    return result;
}

FileSystemStore::FileSystemStore ( )
				 : ObjectStore(),
				   fullStoreName(0),
				   localStoreName(0),
				   isValid(TRUE)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "FileSystemStore::FileSystemStore ()" << endl;
    debug_stream.unlock();
    
#endif
}

 
Boolean FileSystemStore::allTypes ( ObjectState& foundTypes,
				    const char *root ) const
{
    Boolean result = TRUE;
    char directory[MAXPNAMELEN+1];
    char *cPtr = 0;
    char *rPtr = 0;
    DIR *dir = 0;

    ::strcpy(directory, fullStoreName);
    rPtr = directory + strlen(directory);
    
    ::strcat(directory, root);
    ::strcat(directory, "/");
    cPtr = directory + strlen(directory);
    
    dir = ::opendir(directory);

    if (dir)
    {
	dirent *entry;
	struct stat statbuf;
	
	while ((entry = ::readdir(dir)))
	{
	    if (!supressEntry(entry->d_name))
            {
                ::strcpy(cPtr,entry->d_name);
            
                if ((::stat(directory, &statbuf) == 0) &&
                    S_ISDIR(statbuf.st_mode))
		{
		    foundTypes.packNonMappedString(rPtr);
		    result = allTypes(foundTypes, rPtr);
		}
	    }
	}
	
	::closedir(dir);
    }
    
    return result;
}

/*
 * Function to return the file name for the state of the object
 * identified by the Uid and TypeName. If the StateType argument
 * is OS_SHADOW then the Uid part of the name includes # characters.
 * The magic number SLOP below is the number of extra characters needed
 * to make up the entire path.
 * Return char * is dynamically allocated and must be freed by caller
 */

char* FileSystemStore::genPathName ( const Uid& objUid,
				     const TypeName tName,
				     ObjectStore::StateType ) const
{
    char *storeName = locateStore(getStoreName());
    char uidBuff[MAXPNAMELEN+1];
    ostrstream os(uidBuff, MAXPNAMELEN+1, ios::out);
    char *fname;
    char *cPtr = tName;

    /* generate string version of Uid - BEWARE need trailing null!!*/

    os << objUid << ends;

    if (!cPtr)
	cPtr = "";

    fname = new char[strlen(storeName) + strlen(cPtr) +
		     strlen(uidBuff) + SLOP];

    ::strcpy(fname, storeName);

    if (cPtr[0] == '/')
	cPtr++;
    
    ::strcat(fname, cPtr);

    if (fname[::strlen(fname)-1] != '/')
	::strcat(fname, "/");

    ::strcat(fname, uidBuff);

    cPtr = fname + ::strlen(fname);
    
    *cPtr = '\0';

    if (storeName)
#ifdef GCC_ARRAY_DELETE_BUG
        delete storeName;
#else
        delete [] storeName;
#endif    

    return fname;
}

Boolean FileSystemStore::lock ( int fd, int lmode ) const
{
    if (fd != -1)
    {
	struct flock arg;

	arg.l_whence = SEEK_SET;
	arg.l_start = 0;
	arg.l_len = 0;
	arg.l_type = lmode;
	
	if (::fcntl(fd, F_SETLKW, &arg) != -1)
	    return TRUE;
	
    }
    return FALSE;
}

Boolean FileSystemStore::unlock ( int fd ) const
{
    if (fd != -1)
    {
	struct flock arg;

	arg.l_whence = SEEK_SET;
	arg.l_start = 0;
	arg.l_len = 0;
	arg.l_type = F_UNLCK;
	
	if (::fcntl(fd, F_SETLK, &arg) != -1)
	    return TRUE;
	
    }
    return FALSE;
}

Boolean FileSystemStore::setupStore ( const char* location )
{
    isValid = TRUE;
    
    if (location == 0)
	location = "";

    if (localStoreName)
#ifdef GCC_ARRAY_DELETE_BUG
	::delete  localStoreName;
#else
        ::delete [] localStoreName;
#endif

    if (fullStoreName)
#ifdef GCC_ARRAY_DELETE_BUG
	::delete  fullStoreName;
#else
        ::delete [] fullStoreName;
#endif
    
    localStoreName = ::new char[::strlen(location)+1];
    ::strcpy(localStoreName, location);
    
    fullStoreName = locateStore(localStoreName);

    ::umask(0111);
    
    /* The root of the objectstore must exist and be writable */

    if ((fullStoreName == 0) ||
	!createHierarchy(fullStoreName,O_RDONLY) ||
	(::access(fullStoreName, W_OK) != 0))
     {
	error_stream << FATAL << "Cannot access root of object store: "
		     << fullStoreName << "\n";
	isValid = FALSE;
    }

    return isValid;
}

Boolean FileSystemStore::supressEntry ( const char *name ) const
{
    if ((::strcmp(name, ".") == 0) ||
        (::strcmp(name, "..") == 0))
        return TRUE;
    else
        return FALSE;
}

/*
 * Attempt to build up the object store in the file system dynamically.
 * This creates directories as required as new types are added to the
 * store. Note that we made sure the root object store was created and
 * writable at construction time
 */

Boolean FileSystemStore::createHierarchy ( const char *path, int flags ) const
{
    int accMode = (flags & O_RDONLY ? X_OK : W_OK);
    Boolean createOk = TRUE;
    char *slashPoint = 0;
    char *tempPathName = new char[strlen(path) + 1];
    char *startPoint = tempPathName;

    strcpy(tempPathName, path);

    /* Skip up path checking directories exist */
    
    while (createOk && (slashPoint = strrchr(tempPathName, '/')))
    {
	/* chop of the tail of the path */
	
	*slashPoint = 0;

	/* Check its not exhausted */
	
	if (::strlen(tempPathName) == 0)
	    break;
		
	if (::access(tempPathName, accMode) != 0)
	{
	    /* access failed - if ENOENT maybe go up one level */

	    if (errno == ENOENT)
	    {
		continue;
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

    if (createOk)
    {
	/* Create sub-directories as required */

	while ((slashPoint = strchr(startPoint, '/')))
	{
	    *slashPoint = 0;
	    
	    if (::strlen(tempPathName) != 0)
	    {
		if ((::mkdir(tempPathName, 0777) != 0) &&
		    (errno != EEXIST))
		{
		    createOk = FALSE;
		    break;
		}
		chmod(tempPathName, 0777);
	    }
	    
	    *slashPoint = '/';
	    startPoint = slashPoint + 1;
	} 
    }

    delete [] tempPathName;

    return createOk;
}

Boolean FileSystemStore::exists ( const char *path ) const
{
    if (::access((char*)path, F_OK) == 0)
	return TRUE;
    else
	return FALSE;
}

#ifdef NO_INLINES
#  define FSSTORE_CC_
#  include <ObjectStore/FSStore.n>
#  undef FSSTORE_CC_
#endif
