/*
 * Copyright (C) 1995-1997,
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

#include <Arjuna/AbstractR.h>

#include "Opcode.h"

class TCPConnection;
class Applet;

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

    JavaRecord (const Uid& tranUid, int amount, Bank::Outcome outcome,
		TCPConnection* port);
    JavaRecord (const Uid& tranUid, TCPConnection* port);    
    virtual ~JavaRecord ();

    virtual RecordType typeIs () const;
    virtual AnyType value () const;
    virtual void setValue (AnyType);

    virtual Outcome nestedAbort ();
    virtual Outcome nestedCommit ();
    virtual Outcome nestedPrepare ();
    virtual Outcome topLevelAbort ();
    virtual Outcome topLevelCommit ();    
    virtual Outcome topLevelPrepare ();

    virtual void merge (const AbstractRecord *);
    virtual void alter (AbstractRecord *);

    virtual Boolean shouldAdd (const AbstractRecord *) const;
    virtual Boolean shouldAlter (const AbstractRecord *) const;
    virtual Boolean shouldMerge (const AbstractRecord *) const;
    virtual Boolean shouldReplace (const AbstractRecord *) const;

    virtual Boolean save_state (ObjectState&, ObjectType ot);
    virtual Boolean restore_state (ObjectState&, ObjectType ot);
    virtual const TypeName type () const;

    Boolean saveState (BankActionStatus state);
    Boolean removeState ();
    Boolean restoreState (BankActionStatus& state);
    
private:
    char* getName () const;
    
    /* private state variables */
    
    int cashAmount;
    Uid tid;
    Applet* applet;
    Boolean aborted;  // for crash recovery purposes
};

#endif
