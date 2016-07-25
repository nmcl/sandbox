/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: PersistR.h,v 1.6 1995/06/19 13:56:04 ngdp Exp $
 */

#ifndef PERSISTR_H_
#define PERSISTR_H_

/*
 *
 * Persistence Record Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef RECORDT_H_
#  include <Arjuna/RecordT.h>
#endif

#ifndef RECOVERYR_H_
#  include "RecoveryR.h"
#endif

class ObjectState;
class ObjectStore;
class StateManager;
class Uid;

class PersistenceRecord : public RecoveryRecord
{
    friend class PersistSetUp;

public:
    /* Constructors and destructor */

    PersistenceRecord(ObjectState *os, ObjectStore *objStore, StateManager *sm);

    virtual ~PersistenceRecord ();

    /* inherited member functions */

    virtual RecordType typeIs () const;

    virtual Boolean topLevelAbort ();
    virtual Boolean topLevelCommit ();
    virtual PrepareOutcome topLevelPrepare ();

    virtual Boolean topLevelCleanup ();
    
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual Boolean save_state (ObjectState&, ObjectType);

    virtual ostream& print (ostream&) const;
    virtual const TypeName type () const;

    Boolean doSave () const;

protected:
    PersistenceRecord(); // used by crash recovery

    Boolean shadowForced ();

private:

    Boolean shadowMade;
    ObjectStore *store;
    ObjectState *topLevelState;
};


class PersistSetUp
{
public:
    PersistSetUp ();
    ~PersistSetUp ();

    static AbstractRecord* create ();
    static void remove (AbstractRecord*&);

private:
    static Boolean setUp;
};

static PersistSetUp PerSetUp;

#endif
