/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef RAJINITR_H_
#define RAJINITR_H_

/*
 * $Id: RajInitR.h,v 1.3 1993/12/14 11:39:00 nmcl Exp $
 *
 * RajdootInitiate Record Class
 *
 */

#ifndef ABSTRACT_H_
#  include <Arjuna/AbstractR.h>
#endif

#ifndef RAJDOOT_CLRAJACT_H_
#  include <RPC/Rajdoot/ClRajAct.h>
#endif

class RajdootInitiateRecord : public AbstractRecord
{
    friend class RajInitSetUp;

public:
    /* Constructors and destructor */

    RajdootInitiateRecord (const Uid&, ClientRajdootAction_rpc*);
    ~RajdootInitiateRecord ();

    /* inherited functions from AbstractRecord */

    virtual RecordType typeIs () const;
    virtual AnyType value () const;
    virtual void setValue (AnyType);
    
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

    /* inherited functions from StateManger */

    ostream& print (ostream&) const;

protected:
    RajdootInitiateRecord ();

private: 
   
    virtual void merge (const AbstractRecord *);
    virtual void alter (AbstractRecord *);

    virtual Boolean shouldAdd (const AbstractRecord *) const;
    virtual Boolean shouldAlter (const AbstractRecord *) const;
    virtual Boolean shouldMerge (const AbstractRecord * ) const;
    virtual Boolean shouldReplace (const AbstractRecord *) const;

    ClientRajdootAction_rpc* rpc_interface;
};


class RajInitSetUp
{
public:
    RajInitSetUp ();
    ~RajInitSetUp ();

    static AbstractRecord* create ();
    static void remove (AbstractRecord*&);

private:
    static Boolean setUp;
};

static RajInitSetUp RajInSetUp;

#endif
