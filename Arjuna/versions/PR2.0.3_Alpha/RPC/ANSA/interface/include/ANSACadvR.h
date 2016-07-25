/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 */


#ifndef ANSACADVR_H_
#define ANSACADVR_H_

/*
 * $Id: ANSACadvR.h,v 1.2 1993/03/18 15:07:58 nmcl Exp $
 *
 * ANSACadaver Record Class
 *
 */

#ifndef ABSTRACT_H_
#  include <Arjuna/AbstractR.h>
#endif

#ifndef ANSA_CLANSAACT_H_
#  include <RPC/ANSA/ClANSAAct.h>
#endif

class ANSACadaverRecord : public AbstractRecord
{
public:
    /* Constructors and destructor */

    ANSACadaverRecord (const Uid&, ClientANSAAction_rpc*);
    ~ANSACadaverRecord ();

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
