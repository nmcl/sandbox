/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ObjStore.h,v 1.23 1993/08/23 10:31:21 nmcl Exp $
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

class ostream;

class Buffer;
class ObjectState;
class Uid;

// @NoRemote, @NoMarshall
class ObjectStoreList;

enum FileType { SHADOW, ORIGINAL, HIDDEN };

/*
 * This is the base class from which all object store types are derived.
 * Note that because object store instances are stateless, to improve
 * efficiency we try to only create one instance of each type per process.
 * Therefore, the create and destroy methods are used to instead of new
 * and delete. If an object store is accessed via create it *must* be
 * deleted using destroy. Of course it is still possible to make use of
 * new and delete directly and to create instances on the stack.
 */

// @NoRemote, @NoMarshall
class ObjectStore
{
public:
    virtual ~ObjectStore ();

    /* The real interface */
    virtual Boolean commit_state (const Uid&, const TypeName) = 0;
    virtual ObjectState *read_state (const Uid&, const TypeName) = 0;
    virtual ObjectState *read_uncommitted (const Uid&, const TypeName) = 0;
    virtual Boolean remove_state (const Uid&, const TypeName) = 0;
    virtual Boolean remove_uncommitted (const Uid&, const TypeName) = 0;
    virtual Boolean write_committed (const Uid&, const TypeName, const ObjectState&) = 0;
    virtual Boolean write_state (const Uid&, const TypeName, const ObjectState&) = 0;
    virtual ObjectState *allObjUids (const TypeName) = 0;

    virtual const char* getStoreName () const = 0;
    virtual const TypeName type () const = 0;

    /* These methods only make sense for remote object store */
    virtual void storeLocation (const char*);
    virtual void useStoreLocation (const Boolean);

    /* The following are used for dynamic linking of object stores */
    static void addToList (ObjectStore* (*func1)(const char*), void (*func2)(const char*), const TypeName);
    static ObjectStore* create (const TypeName, const char* = 0);
    static void destroy (ObjectStore*&);
    
protected:
    /* Constructors & destructor */
    
    ObjectStore ();

    /* List of types of object stores */

    static ObjectStoreList* headOfList;
};

#endif
