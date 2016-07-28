/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: NamingR.h,v 1.2 1993/12/14 11:42:50 nmcl Exp $
 */

#ifndef NAMINGR_H_
#define NAMINGR_H_

/*
 *
 * Naming Record Class
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
class NamingRecord : public AbstractRecord
{
    friend class NamingSetUp;

public:
    /* Constructors and destructor */

    NamingRecord (const Uid&, const TypeName, pid_t, char*);
    ~NamingRecord ();

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
      NamingRecord ();

private:
    virtual void merge (const AbstractRecord *);
    virtual void alter (AbstractRecord *);

    virtual Boolean shouldAdd (const AbstractRecord *) const;
    virtual Boolean shouldAlter (const AbstractRecord *) const;
    virtual Boolean shouldMerge (const AbstractRecord *) const;
    virtual Boolean shouldReplace (const AbstractRecord *) const;

    Boolean activateDatabase ();

    /* private state variables */

    Uid                            *groupUid;
    class NSInterface              *NS;
    TypeName                       objName;
    pid_t                          processID;
    char                           *hostName;
    Boolean                        haveReleased;
};


//@NoRemote, @NoMarshall
class NamingSetUp
{
public:
    NamingSetUp ();
    ~NamingSetUp ();

    static AbstractRecord* create ();
    static void remove (AbstractRecord*&);

private:
    static Boolean setUp;
};

static NamingSetUp NameSetUp;

#endif





























