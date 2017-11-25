/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjStore.h,v 1.21 1995/03/28 15:34:00 ngdp Exp $
 */

#ifndef OBJSTORE_H_
#define OBJSTORE_H_

#ifdef STUB
#  pragma @NoRename
#endif

/*
 *
 * Base Object Store Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef OBJECTSTORENAMES_H_
#  include <ObjectStore/ObjectStoreNames.h>
#endif

class ostream;

class Buffer;
class ObjectState;
class Uid;
class StoreList;
class ObjectStoreSetUp;

// @NoRemote, @NoMarshall
struct SetUpInfo;

/*
 * This is the base class from which all object store types are derived.
 * Note that because object store instances are stateless, to improve
 * efficiency we try to only create one instance of each type per process.
 * Therefore, the create and destroy methods are used instead of new
 * and delete. If an object store is accessed via create it *must* be
 * deleted using destroy. Of course it is still possible to make use of
 * new and delete directly and to create instances on the stack.
 */

// @NoRemote, @NoMarshall
class ObjectStore
{
public:
    enum StateStatus { OS_UNKNOWN = 0,
		       OS_COMMITTED = 1, OS_UNCOMMITTED = 2,
		       OS_HIDDEN = 4,
		       OS_COMMITTED_HIDDEN = OS_COMMITTED | OS_HIDDEN,
		       OS_UNCOMMITTED_HIDDEN = OS_UNCOMMITTED | OS_HIDDEN };
    
    enum StateType { OS_SHADOW, OS_ORIGINAL, OS_INVISIBLE };
    
    virtual ~ObjectStore ();
    
    virtual Boolean allObjUids (const TypeName, ObjectState&, StateStatus = OS_UNKNOWN ) = 0;
    virtual Boolean allTypes (ObjectState&) const = 0;
    
    virtual StateStatus currentState (const Uid&, const TypeName) = 0;
    virtual const char *getStoreName () const = 0;
    
    virtual char* locateStore (const char*) const;
    
    virtual Boolean commit_state (const Uid&, const TypeName) = 0;
    virtual Boolean fullCommitNeeded () const;
    
    virtual Boolean hide_state (const Uid&, const TypeName) = 0;
    virtual Boolean reveal_state (const Uid&, const TypeName) = 0;
    
    virtual ObjectState *read_committed (const Uid&, const TypeName) = 0;
    virtual ObjectState *read_uncommitted (const Uid&, const TypeName) = 0;
    
    virtual Boolean remove_committed (const Uid&, const TypeName) = 0;
    virtual Boolean remove_uncommitted (const Uid&, const TypeName) = 0;

    virtual Boolean write_committed (const Uid&, const TypeName,
				     const ObjectState&) = 0;
    virtual Boolean write_uncommitted (const Uid&, const TypeName,
				       const ObjectState&) = 0;

    virtual const TypeName type () const = 0;

    Boolean isType (const Uid&, const TypeName, StateStatus);

    /* These methods only make sense for remote object stores */

    virtual void storeLocation (char**, int = 0);
    virtual void useStoreLocation (const Boolean);
    virtual void setObjectData (const Uid&, const TypeName);

    virtual Boolean packInto (Buffer&) const;
    virtual Boolean unpackFrom (Buffer&);

    /* The following are used for dynamic linking of object stores */

    static void addToList (SetUpInfo *);
    static ObjectStore* create (const TypeName, const char* = "");
    static void destroy (ObjectStore*&);
    static Boolean reference (ObjectStore*);

    static void printList (ostream&);
    
protected:
    
    ObjectStore ();

    virtual Boolean supressEntry (const char *) const = 0;
    
private:
    static SetUpInfo* headOfList;	/* List of types of object stores */ 
};

typedef ObjectStore* (osCreatorFunc) (const char *);
typedef osCreatorFunc* osCreator;

// @NoRemote, @NoMarshall
struct SetUpInfo
{
    Boolean    _setUp;			/* initialised? */
    Boolean    _alwaysCreate;		/* ignore use count on create */
    StoreList  *_headOfList;		/* list of store instances */
    osCreator  _new;			/* creation operations */
    TypeName   _tn;			/* store type */
    SetUpInfo *_next;			/* link to next store type */
};

/* NOTE : This class MUST NOT HAVE ANY VIRTUAL FUNCTIONS */

// @NoRemote, @NoMarshall
class ObjectStoreSetUp
{
public:
    ~ObjectStoreSetUp ();
protected:
    ObjectStoreSetUp (SetUpInfo&);
private:
    SetUpInfo& setupInf;
};

#include <ObjectStore/ObjStore.n>

#endif
