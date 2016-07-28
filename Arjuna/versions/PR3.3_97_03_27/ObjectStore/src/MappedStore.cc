/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: MappedStore.cc,v 1.2 1995/06/26 10:29:58 ngdp Exp $
 */

#ifndef STDLIB_H_
#  include <System/stdlib.h>
#endif

#ifndef UNISTD_H_
#  include <System/unistd.h>
#endif

#ifndef FCNTL_H_
#  include <System/fcntl.h>
#endif

#ifndef SYS_STAT_H_
#  include <System/sys/stat.h>
#endif

#ifndef ERRNO_H_
#  include <System/errno.h>
#endif

#ifndef PROTOS_H_
#  include <System/protos.h>
#endif

#ifndef STRSTREAM_H_
#  include <System/strstream.h>
#endif

#ifndef STRING_H_
#  include <System/string.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#if defined(DEBUG) && !defined(DEBUG_H_)
#  include <Common/Debug.h>
#endif

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef UTILITY_H_
#  include <Common/Utility.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef MAPPEDSTORE_H_
#  include <ObjectStore/MappedStore.h>
#endif

#ifndef SINGLETYPEMS_H_
#  include <ObjectStore/SingleTypeMS.h>
#endif

struct MappedStoreData
{
    MappedStoreData (const char *, const TypeName, Boolean);
    ~MappedStoreData();
    
    SingleTypeMappedStore *actualStore;
    TypeName typeStored;
    
    MappedStoreData *next;
};

static osCreatorFunc createMSInstance;

#ifdef STRUCT_INIT_BUG
static SetUpInfo MStoreSetup = { 0, 0, 0, &createMSInstance, MappedStoreType, 0 };
#else
static SetUpInfo MStoreSetup = { FALSE, FALSE, 0, &createMSInstance, MappedStoreType, 0 };
#endif

ObjectStore* createMSInstance ( const char* root )
{
    return new MappedStore(root);
}


MappedStoreSetUp::MappedStoreSetUp () : FileSystemStoreSetUp(MStoreSetup)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "MappedStoreSetUp::MappedStoreSetUp ()" << endl;
    debug_stream.unlock();
    
#endif

#ifdef GCC_STATIC_INIT_BUG
    if (!MStoreSetup._setUp)
    {
	MStoreSetup._new = &createMSInstance;
	MStoreSetup._tn = MappedStoreType;
	MStoreSetup._setUp = TRUE;
	ObjectStore::addToList(&MStoreSetup);
    }
#endif 
}

MappedStoreSetUp::~MappedStoreSetUp ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "MappedStoreSetUp::~MappedStoreSetUp ()" << endl;
    debug_stream.unlock();
    
#endif    
}

MappedStoreData::MappedStoreData ( const char *root,
				   const TypeName theType,
				   Boolean doShadow )
				 : actualStore(new SingleTypeMappedStore(root, theType, doShadow)),
				   typeStored(clone(theType)),
				   next(0)
{
}

MappedStoreData::~MappedStoreData ( )
{
    if (actualStore)
	delete actualStore;

    if (typeStored)
    {
#ifdef GCC_DELETE_BUG
	delete typeStored;
#else
	delete [] typeStored;
#endif
    }
}

/*
 * Mapped object store implementation. Makes use of memory mapped files to
 * store object states.
 * If doShadow is FALSE we NEVER write shadow states - this may be a problem
 * but is here for efficiency...
 */

MappedStore::MappedStore ( const char *root, Boolean doShadow )
			 : FileSystemStore(),
			   head(0),
			   writeShadow(doShadow)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "MappedStore::MappedStore ( "
                 << ((doShadow == TRUE) ? "Shadows" : "NoShadows") << " )" << endl;
    debug_stream.unlock();
    
#endif

    /* this will create the core directory structure */
    
    setupStore(root);
}

MappedStore::~MappedStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "MappedStore::~MappedStore ()\n" << flush;
    debug_stream.unlock();
    
#endif
    
    deleteList();
}

void MappedStore::deleteList ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PRIVATE;
    debug_stream << "void MappedStore::deleteList ()\n" << flush;
    debug_stream.unlock();
    
#endif

    if (head)
    {
	MappedStoreData *ptr = head->next;

	while (head)
	{
	    delete head;
	    head = ptr;
	    if (ptr)
		ptr = ptr->next;
	}
    }
}

ObjectStore *MappedStore::lookFor ( const TypeName tn, Boolean create )
{
    ObjectStore *theStore = 0;
    MappedStoreData *ptr = head;

    if (!tn)
	return 0;
    
    while (ptr)
    {
	if (strcmp(ptr->typeStored, tn) == 0)
	    return ptr->actualStore;
	else
	    ptr = ptr->next;
    }

    /* If here the type was not found */
    
    if (create)
    {
	ptr = new MappedStoreData(getStoreName(), tn, writeShadow);

	if (ptr)
	{
	    ptr->next = head;
	    head = ptr;
	    theStore = ptr->actualStore;
	}
    }

    return theStore;
}
    

Boolean MappedStore::allObjUids ( const TypeName tn, ObjectState& s,
				  ObjectStore::StateStatus stat )
{   
    ObjectStore *ptr = lookFor(tn, TRUE);

    if (ptr)
	return ptr->allObjUids(tn,s,stat);
    else
	return FALSE;
}

Boolean MappedStore::hide_state ( const Uid& objUid, const TypeName tn )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectState* MappedStore::hide_state ( " << objUid << ", "
                 << tn << " )" << endl;
    debug_stream.unlock();
    
#endif
    
    ObjectStore *ptr = lookFor(tn, TRUE);

    if (ptr)
	return ptr->hide_state(objUid, tn);
    else
	return FALSE;
}

Boolean MappedStore::reveal_state ( const Uid& objUid, const TypeName tn )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectState* MappedStore::reveal_state ( " << objUid << ", "
                 << tn << " )" << endl;
    debug_stream.unlock();
    
#endif
       
    ObjectStore *ptr = lookFor(tn, TRUE);

    if (ptr)
	return ptr->reveal_state(objUid, tn);
    else
	return FALSE; 

}


ObjectStore::StateStatus MappedStore::currentState ( const Uid& objUid,
						     const TypeName tn )
{
    ObjectStore *ptr = lookFor(tn, TRUE);

    if (ptr)
	return ptr->currentState(objUid, tn);
    else
	return ObjectStore::OS_UNKNOWN;
}

Boolean MappedStore::commit_state (const Uid& objUid, const TypeName tn)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean MappedStore::commit_state ( " << objUid << ", "
                 << tn << " )" << endl;
    debug_stream.unlock();
    
#endif
    
    ObjectStore *ptr = lookFor(tn, TRUE);

    if (ptr)
	return ptr->commit_state(objUid, tn);
    else
	return FALSE;
}

Boolean MappedStore::fullCommitNeeded () const
{
    return FALSE;
}

ObjectState* MappedStore::read_committed ( const Uid& objUid,
					   const TypeName tn )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectState* MappedStore::read_committed ( "
		 << objUid << ", "
                 << tn 
		 << " )" << endl;
    debug_stream.unlock();
    
#endif
    
    ObjectStore *ptr = lookFor(tn, TRUE);

    if (ptr)
	return ptr->read_committed(objUid, tn);
    else
	return 0;
}

ObjectState* MappedStore::read_uncommitted ( const Uid& objUid,
					     const TypeName tn )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ObjectState* MappedStore::read_uncommitted ( "
		 << objUid << ", "
                 << tn 
		 << " )" << endl;
    debug_stream.unlock();
    
#endif
    
    ObjectStore *ptr = lookFor(tn, TRUE);

    if (ptr)
	return ptr->read_uncommitted(objUid, tn);
    else
	return 0;
}

Boolean MappedStore::remove_committed (const Uid& objUid, const TypeName name ) 
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean MappedStore::remove_committed ( " << objUid << ", "
                 << name << " )" << endl;
    debug_stream.unlock();
    
#endif
    
    ObjectStore *ptr = lookFor(name, TRUE);

    if (ptr)
	return ptr->remove_committed(objUid, name);
    else
	return FALSE;
}

Boolean MappedStore::remove_uncommitted (const Uid& objUid, const TypeName name ) 
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean MappedStore::remove_uncommitted ( " << objUid << ", "
                 << name << " )" << endl;
    debug_stream.unlock();
    
#endif
    
    ObjectStore *ptr = lookFor(name, TRUE);

    if (ptr)
	return ptr->remove_uncommitted(objUid, name);
    else
	return FALSE;
}

Boolean MappedStore::write_committed ( const Uid& objUid, const TypeName tn,
				       const ObjectState& state )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean MappedStore::write_committed " << objUid << ", "
                 << tn << ", " << state << ", " 
		 << " )" << endl;
    debug_stream.unlock();
    
#endif
    
    ObjectStore *ptr = lookFor(tn, TRUE);

    if (ptr)
	return ptr->write_committed(objUid, tn,state);
    else
	return 0;
}

Boolean MappedStore::write_uncommitted ( const Uid& objUid, const TypeName tn,
					 const ObjectState& state )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "Boolean MappedStore::write_uncommitted " << objUid << ", "
                 << tn << ", " << state << ", " 
		 << " )" << endl;
    debug_stream.unlock();
    
#endif
    
    ObjectStore *ptr = lookFor(tn, TRUE);

    if (ptr)
	return ptr->write_uncommitted(objUid, tn, state);
    else
	return 0;
}

#ifdef NO_INLINES
#  define MAPPEDSTORE_CC_
#  include "ObjectStore/MappedStore.n"
#  undef MAPPEDSTORE_CC_
#endif
