/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecoveryR.h,v 1.4 1994/01/20 12:51:13 ngdp Exp $
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
#  include <Arjuna/AbstR.h>
#endif


class ObjectState;
class StateManager;
class Uid;
class ostream;

class RecoveryRecord : public AbstractRecord
{
public:
    /* Constructors and destructor */

    RecoveryRecord (StateManager*);
    virtual ~RecoveryRecord ();

	void setState (ObjectState*);

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

#include "RecovryR.n"

#endif
































