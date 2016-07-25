/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActiveR.h,v 1.6 1993/03/22 09:15:58 ngdp Exp $
 */

#ifndef ACTIVER_H_
#define ACTIVER_H_

/*
 *
 * Activation Record Class
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

class ostream;
class StateManager;

class ActivationRecord : public AbstractRecord
{
public:
    /* Constructors and destructor */

    ActivationRecord (ObjectStatus st, StateManager *sm);
    ~ActivationRecord ();

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

    StateManager *const objectAddr;
    ObjectStatus state;    
};

#endif





























