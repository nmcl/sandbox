/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: RecoveryR.h,v 1.6 1993/03/22 09:16:15 ngdp Exp $
 */

#ifndef RECOVERYR_H_
#define RECOVERYR_H_

/*
 *
 * Recovery Record Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/ComonT.h>
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
public:
    /* Constructors and destructor */

    RecoveryRecord (ObjectState *os, StateManager *sm);
    ~RecoveryRecord ();

    /* inherited functions */

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

    virtual Boolean restore_state (ObjectState& os, ObjectType ot);
    virtual Boolean save_state (ObjectState& os, ObjectType ot);

    virtual ostream& print (ostream& strm) const;
    virtual const TypeName type () const;

protected:
    /* protected state variables */

    StateManager * const objectAddr;
    ObjectState * const state;    
};

#endif
































