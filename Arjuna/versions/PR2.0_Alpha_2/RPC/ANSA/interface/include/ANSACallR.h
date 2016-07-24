/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef ANSACALLR_H_
#define ANSACALLR_H_

/*
 * $Id: ANSACallR.h,v 1.1 1993/11/03 14:39:04 nmcl Exp $
 *
 * ANSA CallRecord Class
 *
 */

#ifndef ABSTRACT_H_
#  include <Arjuna/AbstractR.h>
#endif

#ifndef ANSA_CLANSAACT_H_
#  include <RPC/ANSA/ClANSAAct.h>
#endif

class ANSACallRecord : public AbstractRecord
{
public:
    /* Constructors and destructor */
    
    ANSACallRecord (const Uid&, ClientANSAAction_rpc*);
    ~ANSACallRecord ();
    
    /* inherited functions from AbstractRecord */
    
    virtual void merge (const AbstractRecord *);
    virtual void alter (AbstractRecord *);

    virtual Boolean should_add (const AbstractRecord *) const;
    virtual Boolean should_merge (const AbstractRecord * ) const;
    virtual Boolean should_replace (const AbstractRecord *) const;
    virtual Boolean should_alter (const AbstractRecord *) const;
    virtual RecordType type_is () const;
    virtual AnyType value () const;
    virtual void setValue (AnyType);
    
    virtual void nested_abort ();
    virtual void nested_commit ();
    virtual PrepareOutcome nested_prepare ();
    virtual void top_level_abort ();
    virtual void top_level_commit ();
    virtual PrepareOutcome top_level_prepare ();
    
    /* inherited functions from AbstractRecord */
    
    virtual Boolean restore_state (ObjectState&, ObjectType);
    virtual Boolean save_state (ObjectState&, ObjectType);
    virtual const TypeName type () const;
    
    /* inherited functions from ???  */
    
    virtual ostream& print (ostream&) const;
    
private:
    ClientANSAAction_rpc* rpc_interface;
};

#endif
