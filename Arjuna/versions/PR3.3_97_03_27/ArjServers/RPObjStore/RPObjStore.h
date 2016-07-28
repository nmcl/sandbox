/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */

/*
 * $Id: RPObjStore.h,v 1.5 1995/03/03 13:48:23 ngdp Exp $
 */

#ifndef RPOBJSTORE_H_
#define RPOBJSTORE_H_

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef BUFFER_H_
#  include <Common/Buffer.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#include <Config/Configure.h>


const int RPOBJSTOREUID = 9999;


// @NoRemote, @NoMarshall
class StoreManagerList
{
public:
    StoreManagerList (ObjectStore*);
    ~StoreManagerList ();

    ObjectStore*  ptr;
    StoreManagerList* next;
};

    
class RPObjStore
{
public:
    RPObjStore (int&);
    virtual ~RPObjStore ();

    Boolean allObjUids (const TypeName, ObjectState&, int, const char*, int&);
    int currentState (const Uid&, const TypeName, const char*, int&);

    Boolean commit_state (const Uid&, const TypeName, const char*, int&);

    Boolean hide_state (const Uid&, const TypeName, const char*, int&);
    Boolean reveal_state (const Uid&, const TypeName, const char*, int&);

    //@Delete
    ObjectState* read_committed (const Uid&, const TypeName, const char*, int&);

    //@Delete
    ObjectState* read_uncommitted (const Uid&, const TypeName, const char*, int&);    

    Boolean remove_committed (const Uid&, const TypeName, const char*, int&);
    Boolean remove_uncommitted (const Uid&, const TypeName, const char*, int&);

    Boolean write_committed (const Uid&, const TypeName, const ObjectState&, const char*, int&);
    Boolean write_uncommitted (const Uid&, const TypeName, const ObjectState&, const char*, int&);
    
private:
    ObjectStore* addToList (const char*);

    ObjectState* buff;
    ObjectStore* currentStore;

    static StoreManagerList* headOfList;
};

#endif
