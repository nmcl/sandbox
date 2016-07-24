/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: LockR.h,v 1.1 1993/11/03 12:28:58 nmcl Exp $
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
#  include <Arjuna/AbstractR.h>
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

    LockRecord (LockManager *lm, Boolean rdOnly = FALSE);
    ~LockRecord ();

    /* non-virtual member functions */

    Boolean isReadOnly () const;
    
    /* virtual functions inherited from AbstractRecord */

    virtual void merge (const AbstractRecord *ar);
    virtual void alter (AbstractRecord *ar);

    virtual Boolean should_add (const AbstractRecord *ar) const;
    virtual Boolean should_merge (const AbstractRecord *ar) const;
    virtual Boolean should_replace (const AbstractRecord *ar) const;
    virtual Boolean should_alter (const AbstractRecord *ar) const;
    virtual RecordType type_is () const;
    virtual AnyType value () const;
    virtual void setValue (AnyType newValue);

    virtual void nested_abort ();    
    virtual void nested_commit ();
    virtual PrepareOutcome nested_prepare ();
    virtual void top_level_abort ();
    virtual void top_level_commit ();
    virtual PrepareOutcome top_level_prepare ();

    /* virtual functions inherited from StateManager through */
    /* AbstractRecord */

    virtual ostream& print (ostream & strm) const;
    virtual Boolean restore_state (ObjectState& os, ObjectType ot);    
    virtual Boolean save_state (ObjectState& os, ObjectType ot);
    virtual const TypeName type () const;

private:
    /* private state variables */

    LockManager * const managerAddress; /* needed to be able to release */
					/* locks or propagate them on commit */
    Boolean readOnly;			/* only READ locks set */    
};

#include "LockR.n"

#endif
