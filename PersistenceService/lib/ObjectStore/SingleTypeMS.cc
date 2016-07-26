/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: SingleTypeMS.cc,v 1.9 1995/09/19 10:24:19 ngdp Exp $
 */

#ifndef STDLIB_H_
#  include <os/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <os/unistd.h>
#endif

#ifndef FCNTL_H_
#  include <os/fcntl.h>
#endif

#ifndef SYS_STAT_H_
#  include <os/sys/stat.h>
#endif

#ifndef ERRNO_H_
#  include <os/errno.h>
#endif

#ifndef PERSISTENCE_SERVICE

#ifndef PROTOS_H_
#  include <os/protos.h>
#endif

#endif

#ifndef STRSTREAM_H_
#  include <os/strstream.h>
#endif

#ifndef STRING_H_
#  include <os/string.h>
#endif

#ifndef SYS_PARAM_H_
#  include <os/sys/param.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef OBJSTATE_H_
#  include <Common/ObjState.h>
#endif

#ifndef SINGLETYPEMS_H_
#  include <ObjectStore/SingleTypeMS.h>
#endif

#ifndef PROCESSMUTEX_H_
#  include <Common/ProcessMutex.h>
#endif

struct SingleStoreData
{
    SingleStoreData ();

    Uid stateID;
    size_t stateSize;
    off_t offSet;
    Boolean isOriginal;
    Boolean isHidden;

    Boolean pack (Buffer&) const;
    Boolean unpack (Buffer&);

    SingleStoreData *next;
};

static Uid mapFileNames[MAX_STORES] = { "0:0:0:1", "0:0:0:2","0:0:0:3", "0:0:0:4", "0:0:0:5", "0:0:0:6", "0:0:0:7"};

SingleStoreData::SingleStoreData ()
				 : stateID(NIL_UID),
				   stateSize(0),
				   offSet(0),
				   isOriginal(TRUE),
				   isHidden(FALSE),
				   next(0)
{
}

Boolean SingleStoreData::pack ( Buffer& buff ) const
{
    Boolean result = buff.pack(stateSize) && buff.pack(offSet) && buff.pack(isOriginal) && buff.pack(isHidden);

    if (result)
	result = stateID.pack(buff);

    return result;
}

Boolean SingleStoreData::unpack ( Buffer& buff )
{
    Boolean result = buff.unpack(stateSize) && buff.unpack(offSet) && buff.unpack(isOriginal) && buff.unpack(isHidden);

    if (result)
	result = stateID.unpack(buff);

    return result;
}

/*
 * Mapped object store implementation. Makes use of memory mapped files to
 * store object states.
 * If doShadow is FALSE we NEVER write shadow states - this may be a problem
 * but is here for efficiency...
 */

SingleTypeMappedStore::SingleTypeMappedStore ( const char *root,
					       const TypeName typesHeld,
					       Boolean doShadow )
					     : FileSystemStore(),
					       typeStored(clone(typesHeld)),
					       head(0),
					       mutex(0),
					       currentStore(0),
					       stateCount(0),
					       writeShadow(doShadow)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "SingleTypeMappedStore::SingleTypeMappedStore ( "
                 << ((doShadow == TRUE) ? "Shadows" : "NoShadows") << " )"
		 << endl;
    debug_stream.unlock();
#endif
    
    char *storeName = locateStore(root);
    
    mutex = ProcessMutex::create(typeToKey(storeName));

    /* this will create the directory structure */
    
    setupStore(root);

    for (int i = 0; i < MAX_STORES; i++)
    {
	mappedFiles[i] = 0;
    }

    if (storeName)
#ifdef GCC_ARRAY_DELETE_BUG
        delete storeName;
#else
        delete [] storeName;
#endif    
}

SingleTypeMappedStore::~SingleTypeMappedStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "SingleTypeMappedStore::~SingleTypeMappedStore ()\n"
		 << flush;
    debug_stream.unlock();
#endif

    Semaphore::Status semStat = Semaphore::SEM_UNLOCKED;
    
    if (mutex)
    {
	if ((semStat = mutex->wait()) != Semaphore::SEM_LOCKED)
	{
	    cerr << "SingleTypedMappedStore::~SingleTypedMappedStore - mutex wait failed." << endl;
	}
    }
    
    deleteList();
    
    for (int i = 0; i < MAX_STORES; i++)
    {
	if (mappedFiles[i] != 0)
	{
	    if (semStat == Semaphore::SEM_LOCKED)
	    {
		currentStore = mappedFiles[i];
		if (readStates() && (stateCount == 0))
		    mappedFiles[i]->truncate(sizeof(stateCount));

		deleteList();
	    }
	    
	    delete mappedFiles[i];
	}
    }

    if (typeStored)
    {
#ifdef GCC_DELETE_BUG
        delete typeStored;
#else
        delete [] typeStored;
#endif
    }

    if (mutex)
    {
	if (semStat == Semaphore::SEM_LOCKED)
	    mutex->signal();
	ProcessMutex::destroy(mutex);
    }    
}

void SingleTypeMappedStore::deleteList ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "void SingleTypeMappedStore::deleteList ()\n" << flush;
    debug_stream.unlock();
#endif

    stateCount = 0;
    
    if (head)
    {
	SingleStoreData *ptr = head->next;

	while (head)
	{
	    delete head;
	    head = ptr;
	    if (ptr)
		ptr = ptr->next;
	}
    }
}

Boolean SingleTypeMappedStore::allTypes ( ObjectState& foundTypes ) const
{
    foundTypes.packMappedString(typeStored);
    foundTypes.packMappedString("");
    return TRUE;
}

Boolean SingleTypeMappedStore::allTypes (ObjectState& foundTypes, const char * root) const
{
    return FileSystemStore::allTypes(foundTypes, root);
}

Boolean SingleTypeMappedStore::allObjUids ( const TypeName,
					    ObjectState& found,
					    ObjectStore::StateStatus state )
{
    for (int i = 0; i < MAX_STORES; i++)
    {
	SingleStoreData *marker = 0;
	
	currentStore = mappedFiles[i];
	
	if (currentStore == 0)
	{
	    char *path = genPathName(mapFileNames[i], typeStored, OS_ORIGINAL);

	    if (path)
	    {
		currentStore = mappedFiles[i] = new MappedFile(path, 2*dataSegment());
	    }
	    
#ifdef GCC_DELETE_BUG
	    delete path;
#else
	    delete [] path;
#endif
	}
	
	if (currentStore != 0)
	{
	    if (lock())
	    {
		if (readStates())
		{
		    marker = head;

		    while (marker != 0)
		    {
			if (state == ObjectStore::OS_UNKNOWN)
			    marker->stateID.pack(found);
			else
			    switch (state)
			    {
			    case OS_COMMITTED:
				if (marker->isOriginal)
				    marker->stateID.pack(found);
				break;
			    case OS_UNCOMMITTED:
				if (!marker->isOriginal)
				    marker->stateID.pack(found);
				break;
			    case OS_COMMITTED_HIDDEN:
				if ((marker->isOriginal) && (marker->isHidden))
				    marker->stateID.pack(found);
				break;
			    case OS_UNCOMMITTED_HIDDEN:
				if ((!marker->isOriginal) && (marker->isHidden))
				    marker->stateID.pack(found);
				break;
			    case OS_HIDDEN:
				if (marker->isHidden)
				    marker->stateID.pack(found);
				break;
			    default:
				break;
			    }
			
			marker = marker->next;
		    }
		}
	    
		unlock();
	    }
	}
    }
    
    NIL_UID.pack(found);

    return TRUE;
}

Boolean SingleTypeMappedStore::hide_state ( const Uid& objUid,
					    const TypeName tn )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectState* SingleTypeMappedStore::hide_state ( "
		 << objUid << ", " << tn << " )" << endl;
    debug_stream.unlock();
#endif
    
    Boolean found = FALSE;
    SingleStoreData *marker = 0, *ptr = 0;
    unsigned int hashval = objUid.hash() % MAX_STORES;
    Boolean done = FALSE;
    
    currentStore = mappedFiles[hashval];
    
    if (currentStore == 0)
    {
	char *path = genPathName(mapFileNames[hashval], typeStored, OS_ORIGINAL);

	if (path)
	{
	    currentStore = mappedFiles[hashval] = new MappedFile(path, 2*dataSegment());

#ifdef GCC_DELETE_BUG
	    delete path;
#else
	    delete [] path;
#endif
	}
    }
 
    if (currentStore != 0)
    {
	if (lock())
	{
	    if (readStates())
	    {
		marker = head;
		ptr = marker;
		
		while ((!found) && (marker != 0))
		{
		    if ((marker->stateID == objUid))
		    {
			marker->isHidden = TRUE;
		    }
		    else
			marker = marker->next;
		}
		
		if (writeStates())
		    done = TRUE;
	    }
	    
	    unlock();
	}
    }

    return done;
}

Boolean SingleTypeMappedStore::reveal_state ( const Uid& objUid,
					      const TypeName tn )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectState* SingleTypeMappedStore::reveal_state ( "
		 << objUid << ", " << tn << " )" << endl;
    debug_stream.unlock();
#endif
    
    Boolean found = FALSE;
    SingleStoreData *marker = 0, *ptr = 0;
    unsigned int hashval = objUid.hash() % MAX_STORES;
    Boolean done = FALSE;
    
    currentStore = mappedFiles[hashval];
    
    if (currentStore == 0)
    {
	char *path = genPathName(mapFileNames[hashval], typeStored, OS_ORIGINAL);

	if (path)
	{
	    currentStore = mappedFiles[hashval] = new MappedFile(path, 2*dataSegment());

#ifdef GCC_DELETE_BUG
	    delete path;
#else
	    delete [] path;
#endif
	}
    }

    if (currentStore != 0)
    {
	if (lock())
	{
	    if (readStates())
	    {
		marker = head;
		ptr = marker;
		
		while ((!found) && (marker != 0))
		{
		    if ((marker->stateID == objUid))
		    {
			marker->isHidden = FALSE;
		    }
		    else
			marker = marker->next;
		}
		
		if (writeStates())
		    done = TRUE;
	    }

	    unlock();
	}
    }

    return done;
}

off_t SingleTypeMappedStore::headerSize ()
{
    static off_t size = 0;

    if (size == 0)
    {
	Buffer buff;
	SingleStoreData temp;

	temp.pack(buff);
	size = buff.length();
    }

    return size;
}

char *SingleTypeMappedStore::genPathName ( const Uid& u, const TypeName tn,
					   ObjectStore::StateType st ) const   
{
    char *path = FileSystemStore::genPathName(u, tn, st);

    if (path)
    {
	if (!exists(path))
	    createHierarchy(path, O_RDWR);
    }

    return path;
}
 
Boolean SingleTypeMappedStore::lock ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean SingleTypeMappedStore::lock ()\n" << flush;
    debug_stream.unlock();
#endif

    if (mutex)
    {
	if (mutex->wait() == Semaphore::SEM_LOCKED)
	    return TRUE;
    }
    
    return FALSE;
}

Boolean SingleTypeMappedStore::unlock ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean SingleTypeMappedStore::unlock ()\n" << flush;
    debug_stream.unlock();
#endif

    if (mutex)
    {
        if (mutex->signal() == Semaphore::SEM_UNLOCKED) 
	    return TRUE;
    }

    return FALSE;
}

Boolean SingleTypeMappedStore::writeStates ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean SingleTypeMappedStore::writeStates ()\n" << flush;
    debug_stream.unlock();
#endif

    Boolean result = FALSE;

    if (currentStore != 0)
    {
	SingleStoreData* marker = head;
	long location = sizeof(stateCount);

	if ((currentStore->sizeUp(location) &&
	     currentStore->put((char*)&stateCount, 0, sizeof(stateCount))))
	{
	    if (stateCount > 0)
	    {
		Buffer buff;
		size_t buffSize = 0;
	    
		/* Store object store management information first. */

		for (int i = 0; (i < stateCount) && (marker->pack(buff)); i++)
		{
		    marker = marker->next;
		}
		
		buffSize = buff.length();

		if (currentStore->sizeUp(location+sizeof(buffSize)))
		{
		    if (currentStore->put((char*)&buffSize, location, sizeof(buffSize)))
		    {
			location += sizeof(buffSize);
			currentStore->put(buff.buffer(), location, buffSize);
		    }
		}
	    }
	}
    }

    /* Any of the sizeUp calls could trash the mappedMemory pointer on error */
    
    if ((currentStore == 0) || (!currentStore->isValid()))
    {
	cerr << "SingleTypeMappedStore::writeStates - write failed" << endl;
    }
    else
    {
	currentStore->sync();
	result = TRUE;
    }
    
    return result;
}

Boolean SingleTypeMappedStore::readStates ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean SingleTypeMappedStore::readStates ()\n" << flush;
    debug_stream.unlock();
#endif
    
    off_t location = sizeof(stateCount);
    Boolean result = FALSE;
    SingleStoreData* marker = 0;
    
    if (currentStore != 0)
    {
        deleteList();
	
	if (currentStore->get((char*)&stateCount, 0, sizeof(stateCount)))
	{
	    if (stateCount > 0)
	    {
		size_t stateSize = 0;
		
		if (currentStore->get((char *)&stateSize, location, sizeof(stateSize)) &&
		    (currentStore->sizeUp(stateSize)))
		{
		    char *buffer_data = ::new char[stateSize];
		    
		    location += sizeof(stateSize);
		    
		    if (currentStore->get(buffer_data, location, stateSize))
		    {
			Buffer buff(stateSize, buffer_data);
			
			for (unsigned int i = 0; i < stateCount; i++)
			{
			    if (!head)
			    {
				head = new SingleStoreData;
				marker = head;
			    }
			    else
			    {
				marker->next = new SingleStoreData;
				marker = marker->next;
			    }
			    
			    marker->unpack(buff);
			}
		    }   
		}
	    }
	}
    }
    
    if ((currentStore != 0) && currentStore->isValid())
	result = TRUE;
    else
    {
	cerr << "SingleTypeMappedStore::readStates - read failed" << endl;
    }
    
    return result;
}

void SingleTypeMappedStore::rename ( const Uid& objUid )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "void SingleTypeMappedStore::rename ( " << objUid << " )"
		 << endl;
    debug_stream.unlock();
#endif
    
    SingleStoreData* marker = head;
    
    while (marker != 0)
    {
	if (marker->stateID == objUid)
	{
	    if (marker->isOriginal == FALSE)
	    {
		marker->isOriginal = TRUE;
		return;
	    }
	}

	marker = marker->next;
    }
}

void SingleTypeMappedStore::removeElement ( SingleStoreData *& trail,
					    SingleStoreData *& toRemove )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "void SingleTypeMappedStore::removeElement ()" << endl;
    debug_stream.unlock();
#endif

    stateCount--;
    if (stateCount == 0)
	head = 0;
    else
    {
	if (trail != toRemove)
	    trail->next = toRemove->next;
	else
	    head = toRemove->next;
    }
    
    delete toRemove;
    toRemove = 0;
}

Boolean SingleTypeMappedStore::findOldState ( SingleStoreData *& marker,
					      const Uid& objUid,
					      Boolean fileType,
					      const ObjectState& state )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean SingleTypeMappedStore::findOldState ( " << objUid
		 << ", "
                 << ((fileType == TRUE) ? "ORIGINAL" : "SHADOW") << " )"
		 << endl;
    debug_stream.unlock();
#endif

    Boolean found = FALSE, sizeOK = FALSE;
    SingleStoreData* ptr = head;

    marker = ptr;
    
    if (head == 0)
	return FALSE;
    
    for (unsigned int i = 0; (!found) && (marker != 0); i++)
    {
	/*
	 * Check to see if old state is available to write over
	 * (must be correct size!)
	 */

	if ((marker->stateID == objUid) && (marker->isOriginal == fileType))
	{
	    found = TRUE;

	    if (marker->stateSize >= state.size())
	        sizeOK = TRUE;
	    else
	    {
		/*
		 * The size if not correct, but can we extend this slot
		 * to accomodate the state?
		 */

		if (i < stateCount -1)
		{
		    if (marker->offSet + state.size() <= marker->next->offSet)
		        sizeOK = TRUE;
		}
	    }

	    /* found, but cannot re-use, so "zero" it. */

	    if (!sizeOK)
	        removeElement(ptr, marker);
	}

	if (!found)
	{
	    ptr = marker;
	    marker = marker->next;
	}
    }

    return sizeOK;
}

Boolean SingleTypeMappedStore::findMemory ( SingleStoreData*& marker,
					    const ObjectState& state,
					    off_t& off_set )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "Boolean SingleTypeMappedStore::findMemory ()" << endl;
    debug_stream.unlock();
#endif

    SingleStoreData* trail = 0;
    Boolean found = FALSE;
    off_t prevSize = dataSegment();

    marker = head;
    
    if (head == 0)
	return TRUE;

    if (stateCount <= MAX_STATES)
    {
	for (int i = 0; (!found) && (marker != 0); i++)
	{
	    if (marker->offSet - prevSize >= state.size())
	    {
		marker = trail;
		off_set = prevSize;
		found = TRUE;
	    }
	    else
	    {
		if (marker->next != 0)
		{
		    if (marker->next->offSet - marker->offSet - marker->stateSize >= state.size())
		    {
			off_set = marker->offSet + marker->stateSize;
			found = TRUE;
		    }
		}
		else
		{
		    found = TRUE;
		    off_set = marker->offSet + marker->stateSize;
		}
	    }

	    if (!found)
	    {
		prevSize = marker->offSet + marker->stateSize;
		trail = marker;
		marker = marker->next;
	    }
	}
    }
    else
	cerr << "SingleTypeMappedStore - store full." << endl;

    return found;
}

ObjectStore::StateStatus SingleTypeMappedStore::currentState ( const Uid& objUid,
							       const TypeName tn )
{
    ObjectStore::StateStatus status = ObjectStore::OS_UNKNOWN;
    SingleStoreData *marker = 0;
    Boolean hidden = FALSE;
    unsigned int hashval = objUid.hash() % MAX_STORES;
    
    currentStore = mappedFiles[hashval];
    
    if (currentStore == 0)
    {
	char *path = genPathName(mapFileNames[hashval], typeStored, OS_ORIGINAL);

	if (path)
	{
	    currentStore = mappedFiles[hashval] = new MappedFile(path, 2*dataSegment());

#ifdef GCC_DELETE_BUG
	    delete path;
#else
	    delete [] path;
#endif
	    
	}
    }
    
    if ((currentStore == 0) || !tn)
	return ObjectStore::OS_UNKNOWN;

    if (!lock())
	return ObjectStore::OS_UNKNOWN;

    if (!readStates())
    {
        unlock();
	return ObjectStore::OS_UNKNOWN;
    }

    marker = head;

    while (marker != 0)
    {
	if (marker->stateID == objUid)
	{
	    if (marker->isHidden)
		hidden = TRUE;
	    
	    if (!marker->isOriginal)
	    {
		status = ObjectStore::OS_UNCOMMITTED;
		break;
	    }
	    status = ObjectStore::OS_COMMITTED;
	}
	else
	    marker = marker->next;
    }

    unlock();

    if (hidden)
    {
	if (status == ObjectStore::OS_UNCOMMITTED)
	    status = ObjectStore::OS_UNCOMMITTED_HIDDEN;
	else
	    status = ObjectStore::OS_COMMITTED_HIDDEN;
    }
    
    return status;
}

ObjectState* SingleTypeMappedStore::read_state (const Uid& objUid,
						const TypeName tn,
						ObjectStore::StateType ft)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectState* SingleTypeMappedStore::read_state ( "
		 << objUid << ", "
                 << tn << ", "
		 << ((ft == ObjectStore::OS_ORIGINAL) ? "ORIGINAL" : "SHADOW")
		 << " )" << endl;
    debug_stream.unlock();
#endif
    
    Boolean found = FALSE, fileType = ((ft == ObjectStore::OS_ORIGINAL) ? TRUE : FALSE);
    SingleStoreData *marker = 0, *ptr = 0;
    unsigned int hashval = objUid.hash() % MAX_STORES;
    ObjectState *returnedState =0;
    
    currentStore = mappedFiles[hashval];
    
    if (currentStore == 0)
    {
	char *path = genPathName(mapFileNames[hashval], typeStored, OS_ORIGINAL);

	if (path)
	{
	    currentStore = mappedFiles[hashval] = new MappedFile(path, 2*dataSegment());
	
#ifdef GCC_DELETE_BUG
	    delete path;
#else
	    delete [] path;
#endif    
	}
    }
 
    if (currentStore != 0)
    {
	if (tn)	
	{
	    if ((ft == ObjectStore::OS_ORIGINAL) ||
		((ft == ObjectStore::OS_SHADOW) && (writeShadow)))
	    {
		if (lock())
		{
		    if (readStates())
		    {
			marker = head;
			ptr = marker;

			for (int i = 0; (!found) && (marker != 0); i++)
			{
			    if ((marker->stateID == objUid) && (marker->isOriginal == fileType))
			    {
				found = TRUE;
				ptr = marker;
			    }
			    else
				marker = marker->next;
			}
			
			if (found)
			{
			    char *buffer = ::new char[(int)ptr->stateSize];
			    
			    if (buffer != 0)
			    {
				if (currentStore->get(buffer,ptr->offSet, ptr->stateSize))
				{
				    returnedState = new ObjectState(objUid, tn, (int)ptr->stateSize, buffer);
				}
				else
				{
#ifndef GCC_DELETE_BUG
				    delete [] buffer;
#else
				    delete buffer;
#endif
				}   
			    }
			}
		    }
		    
		    unlock();
		}
	    }
	    else
	    {
		cerr << "SingleTypeMappedStore::read_state - Attempt to read shadow from non-shadowed store." << endl;
	    }
	}
	else
	{
	    cerr << "SingleTypeMappedStore::read_state - Object with uid "
		 << objUid << " has no TypeName\n";
	}
    }
    
    return returnedState;
}

Boolean SingleTypeMappedStore::commit_state ( const Uid& objUid,
					      const TypeName tn )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean SingleTypeMappedStore::commit_state ( " << objUid << ", "
                 << tn << " )" << endl;
    debug_stream.unlock();
#endif

    SingleStoreData *marker = 0, *ptr = 0;
    Boolean found = FALSE;
    unsigned int hashval = objUid.hash() % MAX_STORES;
    Boolean done = FALSE;

    currentStore = mappedFiles[hashval];
    
    if (currentStore == 0)
    {
	char *path = genPathName(mapFileNames[hashval], typeStored, OS_ORIGINAL);

	if (path)
	{
	    currentStore = mappedFiles[hashval] = new MappedFile(path, 2*dataSegment());
	  
#ifdef GCC_DELETE_BUG
	    delete path;
#else
	    delete [] path;
#endif
	}
    }
 
    if (currentStore != 0)
    {
	if (tn)
	{
	    if (writeShadow)
	    {
		if (lock())
		{
		    if (readStates())
		    {
			marker = head;
			ptr = marker;

			while ((marker != 0) && (!found))
			{
			    /* Need to remove "old" original. */
			    
			    if ((marker->stateID == objUid) && (marker->isOriginal == TRUE))
				found = TRUE;
			    else
			    {
				ptr = marker;
				marker = marker->next;
			    }
			}

			/* If found == TRUE then this is an "old" object. */
			
			if (found)
			    removeElement(ptr, marker);

			rename(objUid);

			if (writeStates())
			    done = TRUE;
		    }

		    unlock();
		}
	    }
	    else
		done = TRUE;		/* no shadows -> always commits! */
	}
	else
	{
	    cerr << "SingleTypeMappedStore::commit_state - object with uid "
		 << objUid << " has no TypeName\n";
	}
    }

    return done;
}

Boolean SingleTypeMappedStore::remove_state ( const Uid& objUid,
					      const TypeName name, 
					      ObjectStore::StateType ft )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean SingleTypeMappedStore::remove_state ( "
		 << objUid << ", "
                 << name << ", "
		 << ((ft == ObjectStore::OS_ORIGINAL) ? "ORIGINAL" : "SHADOW")
		 << " )" << endl;
    debug_stream.unlock();
#endif

    Boolean found = FALSE, fileType = ((ft == ObjectStore::OS_ORIGINAL) ? TRUE : FALSE);
    SingleStoreData *marker = 0, *ptr = 0;
    unsigned int hashval = objUid.hash() % MAX_STORES;
    Boolean done = FALSE;
    
    currentStore = mappedFiles[hashval];
    
    if (currentStore == 0)
    {
	char *path = genPathName(mapFileNames[hashval], typeStored, OS_ORIGINAL);

	if (path)
	{
	    currentStore = mappedFiles[hashval] = new MappedFile(path, 2*dataSegment());
	    
#ifdef GCC_DELETE_BUG
	    delete path;
#else
	    delete [] path;
#endif

	}
    }
 
    if (currentStore != 0)
    {
	if (name)
	{
	    if ((ft == ObjectStore::OS_ORIGINAL) ||
		((ft == ObjectStore::OS_SHADOW) && (writeShadow)))
	    {
		if (lock())
		{
		    if (readStates())
		    {
			marker = head;
			ptr = marker;
			
			for (int i = 0; (!found) && (marker != 0); i++)
			{
			    if ((marker->stateID == objUid) && (marker->isOriginal == fileType))
			    {
				removeElement(ptr, marker);
				found = TRUE;
			    }
			    else
			    {
				ptr = marker;
				marker = marker->next;
			    }
			}

			if (writeStates())
			    done = TRUE;
		    }
		    
		    unlock();
		}
	    }
	}
	else
	{
	    cerr << "SingleTypeMappedStore::remove_state - type() operation of object with uid "
		 << objUid << " returns NULL";
	}
    }

    return done;
}

Boolean SingleTypeMappedStore::write_state ( const Uid& objUid,
					     const TypeName tn,
					     const ObjectState& state,
					     ObjectStore::StateType ft )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean SingleTypeMappedStore::write_state "
		 << objUid << ", "
                 << tn << ", " << state << ", "
		 << ((ft == ObjectStore::OS_ORIGINAL) ? "ORIGINAL" : "SHADOW")
		 << " )" << endl;
    debug_stream.unlock();
#endif

    Boolean fileType = ((ft == ObjectStore::OS_ORIGINAL) ? TRUE : FALSE);
    Boolean useOld = FALSE;
    off_t off_set = 0;
    SingleStoreData *marker = 0, *ptr = 0;
    unsigned int hashval = objUid.hash() % MAX_STORES;
    Boolean done = FALSE;
    
    currentStore = mappedFiles[hashval];
    
    if (currentStore == 0)
    {
	char *path = genPathName(mapFileNames[hashval], typeStored, OS_ORIGINAL);

	if (path)
	{
	    currentStore = mappedFiles[hashval] = new MappedFile(path, 2*dataSegment());
	    
#ifdef GCC_DELETE_BUG
	    delete path;
#else
	    delete [] path;
#endif
	}
    }
 
    if (currentStore != 0)
    {
	if (tn)
	{
	    if ((ft == ObjectStore::OS_ORIGINAL) ||
		((ft == ObjectStore::OS_SHADOW) && (writeShadow)))
	    {
		if (lock())
		{
		    if (readStates())
		    {
			if (findOldState(marker, objUid, fileType, state))
			{
			    off_set = marker->offSet;
			    useOld = TRUE;
			}
			else
			{
			    /* No appropriate state found, so look for correct sized "hole". */
			    
			    if (findMemory(marker, state, off_set))
			    {
				/*
				 * Found correct location for "new" state.
				 * Now move to data portion of object store.
				 */

				if (off_set == 0)
				    off_set = dataSegment();

				if (!useOld)
				{
				    /* Create element to put state in. */

				    ptr = new SingleStoreData;
				    stateCount++;

				    if (head == 0)
					head = ptr;
				    else
				    {
					/* Goes before current first element. */
					
					if (!marker)
					{
					    ptr->next = head;
					    head = ptr;
					}
					else
					{
					    /* Goes somewhere else in list. */
					    
					    ptr->next = marker->next;
					    marker->next = ptr;
					}
				    }
				    
				    marker = ptr;
				}
				
				/* Now fill in necessary data. */
				
				marker->stateID = objUid;
				marker->stateSize = state.size();
				marker->offSet = off_set;
				marker->isOriginal = fileType;
				
				/* Try to write it in to object store. */
				
				currentStore->put(state.buffer(), marker->offSet, state.size());
				
				if (writeStates())
				    done = TRUE;
			    }
			}
		    }
		}
		
		unlock();
	    }
	}
	else
	{
	    cerr << "SingleTypeMappedStore::remove_state - type() operation of object with uid "
		 << objUid << " returns NULL";
	}
    }

    return done;
    
}

#ifdef NO_INLINES
#  define SINGLETYPEMS_CC_
#  include <ObjectStore/SingleTypeMS.n>
#  undef SINGLETYPEMS_CC_
#endif
