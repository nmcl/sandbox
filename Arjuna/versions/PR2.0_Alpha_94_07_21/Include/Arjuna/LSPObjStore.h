/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * LSPObjStore.h,v
 */

#ifndef LSPOBJSTORE_H_
#define LSPOBJSTORE_H_

#ifdef STUB
#  pragma @NoRename
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#include <Common/Uid.h>
#endif

#ifndef OBJSTORE_H_
#include <Arjuna/ObjStore.h>
#endif

#ifndef STORELIST_H_
#  include <Arjuna/StoreList.h>
#endif


// @NoRemote, @NoMarshall
class LocalSimplePersistentObjectStore : public ObjectStore
{
public:
    LocalSimplePersistentObjectStore ();
    LocalSimplePersistentObjectStore (const char*);
    virtual ~LocalSimplePersistentObjectStore ();

    Boolean commit_state (const Uid&, const TypeName);
    ObjectState *read_state (const Uid&, const TypeName); 
    ObjectState *read_uncommitted (const Uid&, const TypeName); 
    Boolean remove_state (const Uid&, const TypeName);
    Boolean remove_uncommitted (const Uid&, const TypeName);
    Boolean write_committed (const Uid&, const TypeName, const ObjectState&);
    Boolean write_state (const Uid&, const TypeName, const ObjectState&);
    ObjectState *allObjUids (const TypeName);

    Boolean createHierarchy (const char*, int) const;
    char *genPathName (const Uid&, const TypeName, FileType) const;
    const TypeName type () const;

    const char *getStoreName () const;

private:   
    Boolean closeAndUnlock (int) const;
    int openAndLock (const char*, int, int) const;

    Boolean remove_state (const Uid&, const TypeName, FileType) const;
    ObjectState *read_state (const Uid&, const TypeName, FileType) const;
    Boolean write_state (const Uid&, const TypeName, const ObjectState&, FileType) const;

    void setupStore ();

    Boolean storeValid;
    const char *storeName;
};


// @NoRemote, @NoMarshall
class LSPObjStoreSetUp
{
public:
    LSPObjStoreSetUp ();
    ~LSPObjStoreSetUp ();

    static ObjectStore* create (const char*);
    static void destroy (const char*);

private:
    static Boolean setUp;
    static StoreList* headOfList;
};


static LSPObjStoreSetUp LSPSetUp;

#include <Arjuna/LSPObjStore.n>

#endif
