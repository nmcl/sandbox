/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef RAJCADVR_H_
#define RAJCADVR_H_

/*
 * $Id: RajCadvR.h,v 1.4 1993/12/14 11:38:57 nmcl Exp $
 *
 * RajdootCadaver Record Class
 *
 */

#ifndef ABSTRACT_H_
#  include <Arjuna/AbstractR.h>
#endif

#ifndef RAJDOOT_CLRAJACT_H_
#  include <RPC/Rajdoot/ClRajAct.h>
#endif

class RajdootCadaverRecord : public AbstractRecord
{
    friend class RajCadvSetUp;

public:
    /* Constructors and destructor */

    RajdootCadaverRecord (const Uid&, ClientRajdootAction_rpc*);
    ~RajdootCadaverRecord ();

    /* inherited functions from AbstractRecord */

    virtual RecordType typeIs () const;
    virtual AnyType value () const;
    virtual void setValue (AnyType);
    
    virtual Boolean propagateOnAbort () const;
    
    virtual Boolean nestedAbort ();
    virtual Boolean nestedCommit ();
    virtual PrepareOutcome nestedPrepare ();
    virtual Boolean topLevelAbort ();
    virtual Boolean topLevelCommit ();
    virtual PrepareOutcome topLevelPrepare ();

    /* inherited functions from AbstractRecord */

    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;

    /* inherited functions from StateManager */

    ostream& print (ostream&) const;

protected:
    RajdootCadaverRecord ();

private:

    virtual void merge (const AbstractRecord *);
    virtual void alter (AbstractRecord *);

    virtual Boolean shouldAdd (const AbstractRecord *) const;
    virtual Boolean shouldAlter (const AbstractRecord *) const;
    virtual Boolean shouldMerge (const AbstractRecord * ) const;
    virtual Boolean shouldReplace (const AbstractRecord *) const;

    ClientRajdootAction_rpc* rpc_interface;
};


class RajCadvSetUp
{
public:
    RajCadvSetUp ();
    ~RajCadvSetUp ();

    static AbstractRecord* create ();
    static void remove (AbstractRecord*&);

private:
    static Boolean setUp;
};

static RajCadvSetUp RajCvSetUp;

#endif
