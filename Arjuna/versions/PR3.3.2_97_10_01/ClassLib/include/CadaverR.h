/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: CadaverR.h,v 1.10 1995/06/20 10:23:23 ngdp Exp $
 */

#ifndef CADAVERR_H_
#define CADAVERR_H_

/*
 *
 * Cadaver Record Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef RECORDT_H_
#  include <Arjuna/RecordT.h>
#endif

#ifndef PERSISTR_H_
#  include "PersistR.h"
#endif

class ObjectState;
class ObjectStore;
class StateManager;
class Uid;
class ostream;

class CadaverRecord : public PersistenceRecord
{
    friend class CadaverSetUp;

public:
    /* Constructors & destructor */

    CadaverRecord (ObjectState *, ObjectStore *, StateManager *);
    ~CadaverRecord ();

    /* inherited public virtual functions from PersistenceRecord */

    virtual Boolean propagateOnAbort () const;

    virtual RecordType typeIs () const;

    virtual Boolean nestedAbort ();
    virtual PrepareOutcome nestedPrepare ();
    virtual Boolean topLevelAbort ();
    virtual Boolean topLevelCommit ();
    virtual PrepareOutcome topLevelPrepare ();

    virtual ostream& print (ostream&) const;
    virtual const TypeName type () const;

    virtual Boolean doSave () const;
    
protected:
    CadaverRecord ();

private:
    virtual void merge (const AbstractRecord *);
    virtual Boolean shouldMerge (const AbstractRecord *) const;
    virtual Boolean shouldReplace (const AbstractRecord *) const;

    Boolean newStateIsValid;
    ObjectState *oldState;
    RecordType oType;
    ObjectStore* store;
};


class CadaverSetUp
{
public:
    CadaverSetUp ();
    ~CadaverSetUp ();

    static AbstractRecord* create ();
    static void remove (AbstractRecord*&);

private:
    static Boolean setUp;
};

static CadaverSetUp CadSetUp;


#endif




























