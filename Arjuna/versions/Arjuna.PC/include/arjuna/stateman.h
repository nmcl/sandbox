/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StateMan.h,v 1.6 1993/11/19 10:51:25 nmcl Exp $
 */

#ifndef STATEMAN_H_
#define STATEMAN_H_

/*
 * The remainder of this file processed only by the stub generator.
 */

#ifndef STDDEF_H_
#  include <System/stddef.h>
#endif

#ifndef BOOLEAN_H_
#  include <Common/Boolean.h>
#endif

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjStore/ObjStore.h>
#endif

// @NoRemote, @UserMarshall
class ObjectState;

// @NoRemote, @NoMarshall
class ostream;

enum ObjectStatus
{
    PASSIVE, PASSIVE_NEW, ACTIVE, ACTIVE_NEW
};

// changes for OS/2 and MSDOS (mainly gcc)

#define ObjectType int
#define RECOVERABLE 0
#define ANDPERSISTENT 1
#define NEITHER 2


extern ostream& operator<< (ostream& strm, ObjectStatus os);

// @Remote, @NoMarshall
class StateManager
{
public:
    /* non-virtual member functions and operators */

    // @NoClient
    Boolean activate (ObjectStore * store = 0);
    // @NoClient
    Boolean deactivate (ObjectStore * store = 0, Boolean commit = TRUE);
    // @NoClient
    Boolean resetState (ObjectStatus newState);
    // @NoClient
    ObjectStatus status () const;

    // @NoClient
    void *operator new (size_t amount);
    // @NoClient
    void operator delete (void *memPtr);

    Uid get_uid () const;

    /* virtual member functions */

    // @NoClient
    virtual void destroy ();
    // @NoClient
    virtual void disable ();

    // @NoClient
    virtual ostream& print (ostream& strm) const;

    virtual Boolean restore_state (ObjectState& os, ObjectType ot) = 0;
    virtual Boolean save_state (ObjectState& os, ObjectType ot) = 0;   
    virtual const TypeName type () const = 0;

protected:
/*  Constructors & destructor */

    StateManager (ObjectType ot = RECOVERABLE, Boolean isShareable = TRUE);
    StateManager (const Uid& objUid, ObjectType ot = ANDPERSISTENT);
    virtual ~StateManager ();

    virtual void terminate ();

    virtual Boolean hasRemoteState () const;
    virtual Boolean useStoreLocation () const;

    void setStoreInformation (char**, int);

    /* non-virtual member functions */

    void modified ();
    void persist ();

private:
    /* non-virtual member functions */

    void setupStore (ObjectStore * newStore);
    void resetStore ();

    /* private state variables */

    ObjectStatus currentStatus;
    Boolean deleteStore;
    ObjectType myType;
    ObjectStore *objectStore;	       /* where this object should be stored */
    const Uid objectUid;
    Boolean shareable;
    Uid usingAction;		       /* this is for nested single level */
				       /* actions */    
    Boolean currentRemoteState;

    char** locationsOfStores;
    int storeNumbers;
};

extern ostream& operator<< (ostream& strm, const StateManager& sm);

#endif
