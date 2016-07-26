/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: HashedStore.cc,v 1.10 1995/07/03 14:29:27 ngdp Exp $
 */

#ifndef STRSTREAM_H_
#  include <os/strstream.h>
#endif

#ifndef PERSISTENCE_SERVICE

#ifndef MEMORY_H_
#  include <os/memory.h>
#endif

#endif

#ifndef STRING_H_
#  include <os/string.h>
#endif

#ifndef SYS_PARAM_H_
#  include <os/sys/param.h>
#endif

#ifndef SYS_STAT_H_
#  include <os/sys/stat.h>
#endif

#ifndef DIRENT_H_
#  include <os/dirent.h>
#endif

#ifndef OBJSTATE_H_
#  include <Common/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef HASHEDSTORE_H_
#  include <ObjectStore/HashedStore.h>
#endif

#ifndef STORELIST_H_
#  include <ObjectStore/StoreList.h>
#endif

static osCreatorFunc createHSInstance;

#ifdef STRUCT_INIT_BUG
static SetUpInfo LHSetup = { 0, 0, 0, &createHSInstance, HashedStoreType, 0 };
#else
static SetUpInfo LHSetup = { FALSE, FALSE, 0, &createHSInstance, HashedStoreType, 0 };
#endif

static const NUMBEROFDIRECTORIES = 255;


ObjectStore* createHSInstance ( const char* locationOfStore )
{
    return new HashedStore(locationOfStore);
}

HashedStoreSetUp::HashedStoreSetUp ()
				   : FragmentedStoreSetUp(LHSetup)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "HashedStoreSetUp::HashedStoreSetUp ()" << endl;
    debug_stream.unlock();
    
#endif
  
#ifdef GCC_STATIC_INIT_BUG
    if (!LHSetup._setUp)
    {
	LHSetup._new = &createHSInstance;
	LHSetup._tn = HashedStoreType;
	LHSetup._setUp = TRUE;
	ObjectStore::addToList(&LHSetup);
    }
#endif   
}

HashedStoreSetUp::~HashedStoreSetUp () {}

/*
 * Public constructors and destructor
 */

HashedStore::HashedStore ()
			 : FragmentedStore()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "HashedStore::HashedStore()" << endl;
    debug_stream.unlock();
    
#endif
}

HashedStore::HashedStore ( const char* locationOfStore )
			 : FragmentedStore()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "HashedStore::HashedStore (\""
		 << locationOfStore << "\")" << endl;
    debug_stream.unlock();
    
#endif
    setupStore(locationOfStore);
}

HashedStore::~HashedStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "HashedStore::~HashedStore()" << endl;
    debug_stream.unlock();
    
#endif
}

/*
 * Given a type name return an objectstate that contains all of the
 * uids of objects of that type
 */

Boolean HashedStore::allObjUids ( const TypeName tName,
				  ObjectState& state,
				  ObjectStore::StateStatus match )
{
    char *storeName = locateStore(getStoreName());
    DIR *rdir, *dir = 0;
    char directory[MAXPATHLEN+1];
    char *cPtr = tName;
    int slen = 0;

    /* storeName ALWAYS has a trailing '/' */
	
    ::strcpy(directory, storeName);
	
    /* Does typename start with a '/' if so skip over */
    
    if (tName[0] == '/')
	cPtr++;
    
    ::strcat(directory, cPtr);

    slen = ::strlen(directory);

    if (directory[slen-1] != '/')
    {
	slen++;
	::strcat(directory, "/");
    }

    cPtr = directory + slen;
    
    rdir = ::opendir(directory);
    
    if (rdir)
    {
	dirent *entry;
	struct stat statbuf;

	while ((entry = ::readdir(rdir)))
	{
	    /* Only accept dirs with names surrounded by <> */
	    
	    if ((entry->d_name[0] == '<') &&
		(entry->d_name[::strlen(entry->d_name)-1] == '>'))
	    {
		::strcpy(cPtr,entry->d_name);
	    
		if ((::stat(directory, &statbuf) == 0) &&
		    S_ISDIR(statbuf.st_mode) &&
		    (dir = ::opendir(directory)))
		{
		    dirent *subEntry;
		    char *tPtr = directory + ::strlen(directory);

		    while ((subEntry = ::readdir(dir)))
		    {
			::strcpy(tPtr,"/");
			::strcat(directory,subEntry->d_name);
		    
			if ((::stat(directory, &statbuf) == 0) &&
			    S_ISREG(statbuf.st_mode))
			{
			    Uid aUid(subEntry->d_name, TRUE);
			    
			    if ((aUid != NIL_UID) &&
				((match == ObjectStore::OS_UNKNOWN) ||
				 (isType(aUid, tName, match))))
				aUid.pack(state);
			}
		    }
		}
	    
		::closedir(dir);
	    }
	}
    
	::closedir(rdir);
    }
    
    /* terminate list */

    NIL_UID.pack(state);

    if (storeName)
#ifdef GCC_ARRAY_DELETE_BUG
	delete storeName;
#else
    	delete [] storeName;
#endif

    return TRUE;
	
}

/*
 * Function to return the file name for the state of the object
 * identified by the Uid and TypeName. 
 * Return char * is dynamically allocated and must be freed by caller
 */

char* HashedStore::genPathName ( const Uid& objUid, 
				 const TypeName tName,
				 ObjectStore::StateType ) const
{
    char *storeName = locateStore(getStoreName());
    char uidBuff[MAXPATHLEN+1];
    ostrstream os(uidBuff, MAXPATHLEN+1, ios::out);
    char *fname = 0;
    char *cPtr = tName;
    unsigned long uidHash = objUid.hash();
    
    os << "<" << uidHash % NUMBEROFDIRECTORIES << ">/";
    os << objUid << ends;
    
    fname = new char[strlen(storeName) + strlen(tName) + strlen(uidBuff) + 5];

    ::strcpy(fname, storeName);

    if (tName[0] == '/')
	cPtr++;
    
    ::strcat(fname, cPtr);

    if (fname[::strlen(fname)-1] != '/')
	::strcat(fname, "/");

    ::strcat(fname, uidBuff);

    if (storeName)
#ifdef GCC_ARRAY_DELETE_BUG
	delete storeName;
#else
    	delete [] storeName;
#endif
    
    return fname;
}

Boolean HashedStore::supressEntry ( const char *name ) const
{
    if ((name[0] == '<') &&
	(name[::strlen(name)-1] == '>'))
	return TRUE;
    else
	return FragmentedStore::supressEntry(name);
}

#ifdef NO_INLINES
#  define HASHEDSTORE_CC_
#  include "ObjectStore/HashedStore.n"
#  undef HASHEDSTORE_CC_
#endif

