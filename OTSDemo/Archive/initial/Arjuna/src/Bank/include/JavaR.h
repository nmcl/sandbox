/*
 * Copyright (C) 1993
 *
 * Department of Computing Science,
 * The University,
 * Newcastle upon Tyne,
 * UK.
 *
 * $Id: JavaR.h,v 1.5 1995/02/23 15:09:31 nmcl Exp $
 */

/*
 *
 * Java Record Class
 *
 */

#ifndef JAVAR_H_
#define JAVAR_H_

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

/*
 * Pass in the socket which the Arjuna process uses for
 * communicating with the outside (Java) world. We will
 * also use this for sending the outcome of the action.
 * If the Java user crashes then we either abort, or
 * store the outcome locally and assume he will try again
 * later, at which point we can resend him the outcome.
 *
 * Each transaction has a unique tid and we assume the user
 * will retransmit this upon crash recovery.
 */

class JavaRecord : public AbstractRecord
{
public:
    /* Constructors and destructor */

    JavaRecord (const Uid& javaUid, const TypeName tn, long amount,
		const char* name, int sock);
    ~JavaRecord ();

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
    JavaRecord ();

private:
    virtual void merge (const AbstractRecord *);
    virtual void alter (AbstractRecord *);

    virtual Boolean shouldAdd (const AbstractRecord *) const;
    virtual Boolean shouldAlter (const AbstractRecord *) const;
    virtual Boolean shouldMerge (const AbstractRecord *) const;
    virtual Boolean shouldReplace (const AbstractRecord *) const;

    Boolean transmitOutcome (long);
    
    /* private state variables */
    
    char* id;
    long cashAmount;
    int sock;
    Uid tid;
};


#endif





























