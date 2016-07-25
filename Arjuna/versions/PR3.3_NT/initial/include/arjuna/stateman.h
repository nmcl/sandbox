/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: StateMan.h,v 1.15 1995/03/07 14:59:17 ngdp Exp $
 */

#ifndef STATEMAN_H_
#define STATEMAN_H_

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

#ifndef ERROR_H_
#  include <Common/Error.h>
#endif

#ifndef ERRORLIST_H_
#  include <Common/ErrorList.h>
#endif

#ifndef OBJSTORE_H_
#  include <ObjectStore/ObjStore.h>
#endif

#ifdef RESTRICTED_ARJUNA
#define LocalStateManager StateManager
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

    // @NoRemote
    Boolean activate (const char * = 0);
    // @NoRemote
    Boolean deactivate (const char * = 0, Boolean commit = TRUE);
    // @NoRemote
    Boolean resetState (ObjectStatus newState);
    // @NoRemote
    ObjectStatus status () const;

    // @NoRemote
    void *operator new (size_t amount);
    // @NoRemote
    void operator delete (void *memPtr);

    Uid get_uid () const;

    // @NoRemote
    ErrorList& getErrorList ();
    // @NoRemote
    void clearErrorList ();

    /* virtual member functions */

    // @NoRemote
    virtual void destroy ();
    // @NoRemote
    virtual void disable ();

    // @NoRemote
    virtual ostream& print (ostream& strm) const;

    // @NoRemote
    const char* getStoreRoot () const;

    virtual Boolean restore_state (ObjectState& os, ObjectType ot) = 0;
    virtual Boolean save_state (ObjectState& os, ObjectType ot) = 0;   
    virtual const TypeName type () const;

protected:
/*  Constructors & destructor */

    StateManager (ObjectType ot = RECOVERABLE, Boolean isShareable = FALSE);
    StateManager (const Uid& objUid, ObjectType ot = ANDPERSISTENT);
    virtual ~StateManager ();

    virtual void terminate ();

    Boolean addError (Error *);

    virtual Boolean hasRemoteState () const;
    virtual Boolean useStoreLocation () const;

    void setStoreInformation (char**, int);

    /* non-virtual member functions */

    void modified ();
    void persist ();
    void setupStore (const char* = 0);
    
private:
    /* non-virtual member functions */
    
    void cleanup (Boolean = TRUE);
    void resetStore ();

    /* private state variables */

    Boolean activated;
    ObjectStatus currentStatus;
    ObjectType myType;
    ObjectStore *objectStore;	       /* where this object should be stored */
    char *storeRoot;
    const Uid objectUid;
    Boolean shareable;
    Uid usingAction;		       /* this is for nested single level */
				       /* actions */    
    Boolean currentRemoteState;

    char** locationsOfStores;
    int storeNumbers;

    ErrorList lastError;

#ifdef WIN32
	static const char* defaultObjectStore;
#endif
};

extern ostream& operator<< (ostream& strm, const StateManager& sm);

#endif
