/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecoveryR.h,v 1.5 1996/07/17 09:31:55 nmcl Exp $
 */

#ifndef RECOVERYR_H_
#define RECOVERYR_H_

/*
 *
 * Recovery Record Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef RECORDT_H_
#  include <Arjuna/RecordT.h>
#endif

#ifndef ABSTRACTR_H_
#  include <Arjuna/AbstractR.h>
#endif

class ObjectState;
class StateManager;
class Uid;
class ostream;

class RecoveryRecord : public AbstractRecord
{
    friend class RecoverySetUp;

public:
    /* Constructors and destructor */

    RecoveryRecord (ObjectState *, StateManager *);
    virtual ~RecoveryRecord ();

    /* inherited functions */

    virtual RecordType typeIs () const;
    virtual AnyType value () const;
    virtual void setValue (AnyType);

    virtual Boolean nestedAbort ();
    virtual Boolean nestedCommit ();
    virtual PrepareOutcome nestedPrepare ();
    virtual Boolean topLevelAbort ();
    virtual Boolean topLevelCommit ();
    virtual PrepareOutcome topLevelPrepare ();

    virtual Boolean restore_state (ObjectState& os, ObjectType ot);
    virtual Boolean save_state (ObjectState& os, ObjectType ot);

    virtual ostream& print (ostream& strm) const;
    virtual const TypeName type () const;

    Boolean doSave () const;

protected:
    RecoveryRecord (); // used by crash recovery
    
    StateManager * const getObjectAddr () const;
    ObjectState * const getState() const;

private:

    virtual void merge (const AbstractRecord *);
    virtual void alter (AbstractRecord *);

    virtual Boolean shouldAdd (const AbstractRecord *) const;
    virtual Boolean shouldMerge (const AbstractRecord *) const;
    virtual Boolean shouldReplace (const AbstractRecord *) const;
    virtual Boolean shouldAlter (const AbstractRecord *) const;

    /* private state variables */

    StateManager * const objectAddr;
    ObjectState * state;

};

class RecoverySetUp
{
public:
    RecoverySetUp ();
    ~RecoverySetUp ();

    static AbstractRecord* create ();
    static void remove (AbstractRecord*&);

private:
    static Boolean setUp;
};

static RecoverySetUp RecSetUp;


#include "RecoveryR.n"

#endif
































