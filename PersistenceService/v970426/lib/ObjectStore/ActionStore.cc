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

#ifndef STRING_H_
#  include <os/string.h>
#endif

#ifndef OBJSTATE_H_
#  include <Common/ObjState.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifndef SHADOWSTORE_H_
#  include <ObjectStore/ShadowStore.h>
#endif

#ifndef ACTIONSTORE_H_
#  include <ObjectStore/ActionStore.h>
#endif

#ifndef STORELIST_H_
#  include <ObjectStore/StoreList.h>
#endif

static osCreatorFunc createASInstance;

#ifdef STRUCT_INIT_BUG
static SetUpInfo ActionSetup = { 0, 0, 0, &createASInstance, ActionStoreType, 0  };
#else
static SetUpInfo ActionSetup = { FALSE, FALSE, 0, &createASInstance, ActionStoreType, 0  };
#endif


ObjectStore* createASInstance ( const char* locationOfStore )
{
    return new ActionStore(locationOfStore);
}

ActionStoreSetUp::ActionStoreSetUp ()
				   : ShadowingStoreSetUp(ActionSetup)
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ActionStoreSetUp::ActionStoreSetUp ()" << endl;
    debug_stream.unlock();
    
#endif
    
#ifdef GCC_STATIC_INIT_BUG
    if (!ActionSetup._setUp)
    {
	ActionSetup._new = &createASInstance;
	ActionSetup._tn = ActionStoreType;
	ActionSetup._setUp = TRUE;
	ObjectStore::addToList(&ActionSetup);
    }
#endif 
}

ActionStoreSetUp::~ActionStoreSetUp () {}

/*
 * Public constructors and destructor
 */

ActionStore::ActionStore ( const char* locationOfStore )
			 : ShadowingStore()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << CONSTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ActionStore::ActionStore (\"" << locationOfStore
		 << "\")" << endl;
    debug_stream.unlock();
    
#endif
    setupStore(locationOfStore);
}

ActionStore::~ActionStore ()
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << DESTRUCTORS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ActionStore::~ActionStore()" << endl;
    debug_stream.unlock();
    
#endif
}

/*
 * Public functions
 */

/*
 * Determine current state of object. Assumes that genPathName allocates enough
 * extra space to allow extra chars to be added.
 * Action stores only store committed objects
 */

ObjectStore::StateStatus ActionStore::currentState ( const Uid& objUid,
						     const TypeName tName )
{
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ActionStore::currentState(" 
		 << objUid << ", " << tName << ")" << endl;
    debug_stream.unlock();
    
#endif

    ObjectStore::StateStatus theState = ObjectStore::OS_UNKNOWN;
    
    if (storeValid())
    {
	char *path = genPathName(objUid, tName, ObjectStore::OS_ORIGINAL);

	if (exists(path))
	    theState = ObjectStore::OS_COMMITTED;

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

Boolean ActionStore::commit_state ( const Uid& objUid,
				    const TypeName tName )
{
    
#ifdef DEBUG
    debug_stream.lock();
    
    debug_stream << FUNCTIONS << FAC_OBJECT_STORE << VIS_PUBLIC;
    debug_stream << "ActionStore::commit_state(" << objUid << ", " << tName
		 << ")" << endl;
    debug_stream.unlock();
    
#endif
	
    Boolean result = FALSE;

    /* Bail out if the object store is not set up */

    if (!storeValid())
	return FALSE;

    if (currentState(objUid, tName) == OS_COMMITTED)
	result = TRUE;
    
    return result;
}

#ifdef NO_INLINES
#  define ACTIONSTORE_CC_
#  include <ObjectStore/ActionStore.n>
#  undef ACTIONSTORE_CC_
#endif
