/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: FSStore.h,v 1.3 1995/04/03 09:04:17 ngdp Exp $
 */

#ifndef FSSTORE_H_
#define FSSTORE_H_

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

class StoreList;

class FileSystemStore : public ObjectStore
{
public:
    FileSystemStore (const char*);
    virtual ~FileSystemStore ();

    virtual Boolean allObjUids (const TypeName, ObjectState&,
                                ObjectStore::StateStatus);
    virtual Boolean allTypes (ObjectState&) const;
 
    virtual const char *getStoreName () const;
    
    virtual ObjectState *read_committed (const Uid&, const TypeName); 
    virtual ObjectState *read_uncommitted (const Uid&, const TypeName);
    
    virtual Boolean remove_committed (const Uid&, const TypeName);
    virtual Boolean remove_uncommitted (const Uid&, const TypeName);
    
    virtual Boolean write_committed (const Uid&, const TypeName,
				     const ObjectState&);
    virtual Boolean write_uncommitted (const Uid&, const TypeName,
				       const ObjectState&);
    
    virtual Boolean packInto (Buffer&) const;

    virtual Boolean unpackFrom (Buffer&);

protected:
    FileSystemStore ();
    
    virtual Boolean allTypes (ObjectState&, const char *) const;
    virtual char *genPathName (const Uid&, const TypeName,
			       ObjectStore::StateType) const;    

    virtual Boolean lock (int, int) const;
    virtual Boolean unlock (int) const;

    virtual Boolean setupStore (const char*);
    virtual Boolean supressEntry (const char *) const;

    Boolean createHierarchy (const char*, int) const;
    Boolean exists (const char*) const;
    Boolean storeValid () const;
    
private:
    virtual ObjectState *read_state (const Uid&, const TypeName,
				     ObjectStore::StateType) = 0;    
    virtual Boolean remove_state (const Uid&, const TypeName,
				  ObjectStore::StateType) = 0;
    virtual Boolean write_state (const Uid&, const TypeName,
				 const ObjectState&, ObjectStore::StateType) = 0;
    
    char *fullStoreName;
    char *localStoreName;
    Boolean isValid;

};

// @NoRemote, @NoMarshall
class FileSystemStoreSetUp : public ObjectStoreSetUp
{
public:
    ~FileSystemStoreSetUp ();
protected:
     FileSystemStoreSetUp (SetUpInfo&);
};

#include <ObjectStore/FSStore.n>

#endif
