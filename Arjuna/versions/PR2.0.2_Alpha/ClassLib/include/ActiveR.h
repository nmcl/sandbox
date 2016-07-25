/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ActiveR.h,v 1.3 1993/12/14 11:28:46 nmcl Exp $
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
    friend class ActiveSetUp;

public:
    /* Constructors and destructor */

    ActivationRecord (ObjectStatus, StateManager *);
    ~ActivationRecord ();

    /* inherited functions */

    virtual RecordType typeIs () const;
    virtual AnyType value () const;
    virtual void setValue (AnyType newValue);

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

protected:
    ActivationRecord ();

private:
    virtual void merge (const AbstractRecord *);
    virtual void alter (AbstractRecord *);

    virtual Boolean shouldAdd (const AbstractRecord *) const;
    virtual Boolean shouldAlter (const AbstractRecord *) const;
    virtual Boolean shouldMerge (const AbstractRecord *) const;
    virtual Boolean shouldReplace (const AbstractRecord *) const;


    /* private state variables */

    StateManager *const objectAddr;
    ObjectStatus state;
};


class ActiveSetUp
{
public:
    ActiveSetUp ();
    ~ActiveSetUp ();

    static AbstractRecord* create ();
    static void remove (AbstractRecord*&);

private:
    static Boolean setUp;
};

static ActiveSetUp ActSetUp;



#endif





























