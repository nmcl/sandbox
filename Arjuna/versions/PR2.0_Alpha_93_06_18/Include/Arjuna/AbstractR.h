/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: AbstractR.h,v 1.11 1993/08/11 13:50:36 ndbi Exp $
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

    const Uid& ordering () const;
    const Uid& getUidOfObject () const;
    const TypeName getTypeOfObject () const;

    Boolean operator== (const AbstractRecord *ar) const;
    Boolean operator< (const AbstractRecord *ar) const;
    Boolean operator> (const AbstractRecord *ar) const;

    /*
     * These routines are used by RecoveryRecord::restore_state to overcome
     * a bug in g++
     */

#ifdef __GNUG__
    void setUidOfObject (const Uid &);
    void setTypeOfObject (const TypeName);
#endif

    /* virtual member functions and operators */

    /*
     * Many functions are declared pure virtual to enforce definition
     * in a derived class
     */

    virtual void merge (const AbstractRecord *ar) = 0;
    virtual void alter (AbstractRecord *ar) = 0;

    virtual Boolean should_add (const AbstractRecord *ar) const = 0;
    virtual Boolean should_merge (const AbstractRecord *ar) const = 0;
    virtual Boolean should_replace (const AbstractRecord *ar) const = 0;
    virtual Boolean should_alter (const AbstractRecord *ar) const = 0;
    virtual RecordType type_is () const = 0;
    virtual AnyType value () const = 0;
    virtual void setValue (AnyType newValue) = 0;

    /* atomic action interface - one operation per two-phase commit state */

    virtual void nested_abort () = 0;
    virtual void nested_commit () = 0;
    virtual PrepareOutcome nested_prepare () = 0;
    virtual void top_level_abort () = 0;
    virtual void top_level_commit () = 0;    
    virtual PrepareOutcome top_level_prepare () = 0;
    
    /* inherited public virtual functions */

    virtual ostream& print (ostream & strm) const;
    virtual Boolean restore_state (ObjectState& os, ObjectType ot) = 0;
    virtual Boolean save_state (ObjectState& os, ObjectType ot) = 0;
    virtual const TypeName type () const;

protected:
    /* Constructors and destructor */
    
    AbstractRecord (const Uid& storeUid, const TypeName objectTName, ObjectType ot);
    AbstractRecord (const Uid& storeUid);
    AbstractRecord (); // used by crash recovery


private:
    /*
     * Abstract records can be on lists but we only allow the
     * RecordList class to manipulate the linkage fields
     */

    AbstractRecord *getNext () const;    
    AbstractRecord *getPrevious () const;
    void setNext (AbstractRecord *ar);
    void setPrevious (AbstractRecord *ar);
   
    /* private state variables */

    AbstractRecord *next;
    AbstractRecord *previous;

    Uid uidOfObject;
    TypeName typeOfObject;
    
};

#endif
