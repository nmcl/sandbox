/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: ReplicationR.h,v 1.5 1995/02/23 15:09:31 nmcl Exp $
 */

#ifndef REPLICATIONR_H_
#define REPLICATIONR_H_

/*
 *
 * Replication Record Class
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


//@NoRemote, @NoMarshall
class ostream;

//@NoRemote, @NoMarshall
class RepObjStoreInterface;


//@NoRemote, @NoMarshall
class ReplicationRecord : public AbstractRecord
{
    friend class ReplnSetUp;

public:
    /* Constructors and destructor */

    ReplicationRecord (RepObjStoreInterface*, const Uid&,
		       const TypeName, Boolean usingStore);
    ~ReplicationRecord ();

    /* inherited functions */

    Boolean propagateOnAbort () const;

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

    virtual Boolean doSave () const;

    virtual ostream& print (ostream& strm) const;
    virtual const TypeName type () const;

protected:
    ReplicationRecord ();

private:
    virtual void merge (const AbstractRecord *);
    virtual void alter (AbstractRecord *);

    virtual Boolean shouldAdd (const AbstractRecord *) const;
    virtual Boolean shouldAlter (const AbstractRecord *) const;
    virtual Boolean shouldMerge (const AbstractRecord *) const;
    virtual Boolean shouldReplace (const AbstractRecord *) const;

    /* private state variables */

    Uid                            *groupUid;
    RepObjStoreInterface           *objectAddr;
    TypeName                       objName;
    Boolean                        haveDeactivated;
};


//@NoRemote, @NoMarshall
class ReplnSetUp
{
public:
    ReplnSetUp ();
    ~ReplnSetUp ();

    static AbstractRecord* create ();
    static void remove (AbstractRecord*&);

private:
    static Boolean setUp;
};

static ReplnSetUp RepRecSetUp;

#endif





























