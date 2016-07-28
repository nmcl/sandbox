/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AbstractR.h,v 1.8 1997/01/06 12:16:59 nmcl Exp $
 */

#ifndef ABSTRACTR_H_
#define ABSTRACTR_H_

/*
 *
 * Abstract Record Class
 *
 */

#ifndef COMMONT_H_
#  include <Common/CommonT.h>
#endif

#ifndef UID_H_
#  include <Common/Uid.h>
#endif

#ifndef STATEMAN_H_
#  include <Arjuna/StateMan.h>
#endif

#ifndef RECORDT_H_
#  include <Arjuna/RecordT.h>
#endif

#ifndef PREPOUT_H_
#  include <Arjuna/PrepOut.h>
#endif

class AbstractRecordList;

/*
 * We deliberately derive abstract records from the stub mangled
 * class name to ensure that we can never accidentally
 * create an abstract record that has a remote statemanager component
 * If we are running the stub generator over this file however use cpp
 * to temporarily put the 'real' name back.
 */

#ifdef STUB
#  define LocalStateManager StateManager
#endif

// @NoRemote, @NoMarshall

class AbstractRecord : public LocalStateManager
{
    friend class RecordList;
    friend class RecordListI;

public:
    /* AtomicAction needs to be able to destroy AbstractRecords */

    virtual ~AbstractRecord ();

    /* non-virtual member functions and operators */

    const Uid& order () const;
    const TypeName getTypeOfObject () const;

    Boolean operator== (const AbstractRecord *) const;
    Boolean operator< (const AbstractRecord *) const;
    Boolean operator> (const AbstractRecord *) const;

    /*
     * These routines are used by RecoveryRecord::restore_state to overcome
     * a bug in g++
     */

#ifdef __GNUG__
    void setUidOfObject (const Uid &);
    void setTypeOfObject (const TypeName);
#endif

    /* virtual member functions and operators */

    virtual Boolean propagateOnAbort () const;
    
    virtual RecordType typeIs () const = 0;
    virtual AnyType value () const = 0;
    virtual void setValue (AnyType) = 0;

    /* atomic action interface - one operation per two-phase commit state */

    virtual Boolean nestedAbort () = 0;
    virtual Boolean nestedCommit () = 0;
    virtual PrepareOutcome nestedPrepare () = 0;
    virtual Boolean topLevelAbort () = 0;
    virtual Boolean topLevelCommit () = 0;    
    virtual PrepareOutcome topLevelPrepare () = 0;

    /* special cleanup function */

    virtual Boolean topLevelCleanup ();
    
    /* inherited public virtual functions */

    virtual ostream& print (ostream & strm) const;
    virtual Boolean restore_state (ObjectState& os, ObjectType ot);
    virtual Boolean save_state (ObjectState& os, ObjectType ot);
    virtual const TypeName type () const;

    virtual Boolean doSave () const;

    /* The following are used for dynamic linking of abstract records */

    static void addToList (AbstractRecord* (*func1)(), void (*func2)(AbstractRecord*&), const TypeName);
    static AbstractRecord* create (const TypeName);
    static void remove (AbstractRecord*&);
    
protected:
    /* Constructors and destructor */
    
    AbstractRecord (const Uid&, const TypeName, ObjectType);
    AbstractRecord (const Uid&);
    AbstractRecord (); // used by crash recovery

private:
    /*
     * Many functions are declared pure virtual to enforce definition
     * in a derived class
     */

    virtual void merge (const AbstractRecord *) = 0;
    virtual void alter (AbstractRecord *) = 0;

    virtual Boolean shouldAdd (const AbstractRecord *) const = 0;
    virtual Boolean shouldAlter (const AbstractRecord *) const = 0;
    virtual Boolean shouldMerge (const AbstractRecord *) const = 0;
    virtual Boolean shouldReplace (const AbstractRecord *) const = 0;


    /*
     * Abstract records can be on lists but we only allow the
     * RecordList class to manipulate the linkage fields
     */

    AbstractRecord *getNext () const;    
    AbstractRecord *getPrevious () const;
    void setNext (AbstractRecord *);
    void setPrevious (AbstractRecord *);
   
    /* private state variables */

    AbstractRecord *next;
    AbstractRecord *previous;

    Uid uidOfObject;
    TypeName typeOfObject;

    /* List of types of abstract records */

    static AbstractRecordList* headOfList;
};

#endif
