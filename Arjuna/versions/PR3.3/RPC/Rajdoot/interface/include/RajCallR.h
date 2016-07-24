/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef RAJCALLR_H_
#define RAJCALLR_H_

/*
 * $Id: RajCallR.h,v 1.3 1993/12/14 11:38:59 nmcl Exp $
 *
 * RajdootCallRecord Class
 *
 */

#ifndef ABSTRACT_H_
#  include <Arjuna/AbstractR.h>
#endif

#ifndef RAJDOOT_CLRAJACT_H_
#  include <RPC/Rajdoot/ClRajAct.h>
#endif

class RajdootCallRecord : public AbstractRecord
{
    friend class RajCallSetUp;

public:
    /* Constructors and destructor */
    
    RajdootCallRecord (const Uid&, ClientRajdootAction_rpc*);
    ~RajdootCallRecord ();
    
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

    Boolean doSave () const;

    /* inherited functions from StateManager  */
    
    virtual ostream& print (ostream&) const;

protected:
    RajdootCallRecord ();

private:
    
    virtual void merge (const AbstractRecord *);
    virtual void alter (AbstractRecord *);

    virtual Boolean shouldAdd (const AbstractRecord *) const;
    virtual Boolean shouldAlter (const AbstractRecord *) const;
    virtual Boolean shouldMerge (const AbstractRecord * ) const;
    virtual Boolean shouldReplace (const AbstractRecord *) const;

    ClientRajdootAction_rpc* rpc_interface;
};

class RajCallSetUp
{
public:
    RajCallSetUp ();
    ~RajCallSetUp ();

    static AbstractRecord* create ();
    static void remove (AbstractRecord*&);

private:
    static Boolean setUp;
};

static RajCallSetUp RajClSetUp;

#endif
