/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockR.h,v 1.3 1993/12/14 11:28:52 nmcl Exp $
 */

#ifndef LOCKR_H_
#define LOCKR_H_

/*
 *
 * Lock Record Class Interface
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

#ifndef OBJSTATE_H_
#  include <Arjuna/ObjState.h>
#endif

#ifndef UID_H_
#  include <Arjuna/Uid.h>
#endif

#ifndef LOCKMAN_H_
#  include <Arjuna/LockMan.h>
#endif


class Lockstore;

class LockRecord : public AbstractRecord
{
public:
    /* Constructors & destructor */

    LockRecord (LockManager *, Boolean rdOnly = FALSE);
    ~LockRecord ();

    /* non-virtual member functions */

    Boolean isReadOnly () const;
    
    /* virtual functions inherited from AbstractRecord */

    virtual RecordType typeIs () const;
    virtual AnyType value () const;
    virtual void setValue (AnyType newValue);

    virtual Boolean nestedAbort ();    
    virtual Boolean nestedCommit ();
    virtual PrepareOutcome nestedPrepare ();
    virtual Boolean topLevelAbort ();
    virtual Boolean topLevelCommit ();
    virtual PrepareOutcome topLevelPrepare ();

    /* virtual functions inherited from StateManager through */
    /* AbstractRecord */

    virtual ostream& print (ostream & strm) const;
    virtual Boolean restore_state (ObjectState& os, ObjectType ot);    
    virtual Boolean save_state (ObjectState& os, ObjectType ot);
    virtual const TypeName type () const;

protected:
    LockRecord ();

private:
    virtual void merge (const AbstractRecord *);
    virtual void alter (AbstractRecord *);

    virtual Boolean shouldAdd (const AbstractRecord *) const;
    virtual Boolean shouldAlter (const AbstractRecord *) const;
    virtual Boolean shouldMerge (const AbstractRecord *) const;
    virtual Boolean shouldReplace (const AbstractRecord *) const;


    /* private state variables */

    LockManager * const managerAddress; /* needed to be able to release */
					/* locks or propagate them on commit */
    Boolean readOnly;			/* only READ locks set */    
};


#include "LockR.n"

#endif
