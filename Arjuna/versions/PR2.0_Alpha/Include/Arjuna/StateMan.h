/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StateMan.h,v 1.16 1993/03/22 09:37:07 ngdp Exp $
 */

#ifndef STATEMAN_H_

/*
 * WARNING - ROAD WORKS AHEAD!
 *
 * Stub Generation Nasties...
 *
 * Various cpp tricks are used in this file to massage the name of
 * the class defined herein. Be warned!
 *
 * Certain Arjuna classes need to see ONLY the local definition of
 * StateManager (this file) while user classes need to see the stub
 * generated version (StateMan_stub.h). Problems can arise if this
 * file is included since it prevents inclusion of StateMan_stub.h
 * Short-circuit this by incuding the stub file by default.
 */

#ifndef STUB
#  define STATEMANAGER_SERVER
#  include <Arjuna/StateMan_stub.h>
#else

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
#  include <Arjuna/ObjStore.h>
#endif

// @NoRemote, @UserMarshall
class ObjectState;

// @NoRemote, @NoMarshall
class ostream;

enum ObjectStatus
{
    PASSIVE, PASSIVE_NEW, ACTIVE, ACTIVE_NEW
};

enum ObjectType
{
    RECOVERABLE, ANDPERSISTENT, NEITHER
};

extern ostream& operator<< (ostream& strm, ObjectType ot);
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
    void resetState (ObjectStatus newState);

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

    StateManager (ObjectType ot = RECOVERABLE, Boolean isShareable = FALSE);
    StateManager (const Uid& objUid, ObjectType ot = ANDPERSISTENT);
    virtual ~StateManager ();

    virtual Boolean HasRemoteState () const;
    virtual const char *StoreLocation () const;
    virtual void terminate ();

    /* non-virtual member functions */

    void modified ();
    void persist ();


private:
    /* non-virtual member functions */

    void setupStore (ObjectStore * newStore);

    /* private state variables */

    Boolean shareable;
    ObjectStatus currentStatus;
    ObjectType myType;
    ObjectStore *objectStore;	       /* where this object should be stored */
    const Uid objectUid;
    Uid usingAction;		       /* this is for nested single level */
				       /* actions */    
};

extern ostream& operator<< (ostream& strm, const StateManager& sm);

#endif

#endif
